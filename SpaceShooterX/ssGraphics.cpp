#include "ssCore.h"
#include "DXCommon.h"
#include "wincodec.h"

#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "windowscodecs.lib")

using namespace ss;

ssGraphics::ssGraphics(HWND hwnd) : m_hwnd{ hwnd } {
	m_pD3DDevice  = nullptr;
	m_pD2DFactory = nullptr;
	m_pD2DDevice  = nullptr;
	m_pContext    = nullptr;
	m_pDXGIDevice = nullptr;
	m_pSwapChain  = nullptr;
	m_pBackBuffer = nullptr;
}

ssGraphics::~ssGraphics() {
	if (m_pBackBuffer)
		m_pBackBuffer->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pContext)
		m_pContext->Release();
	if (m_pD2DDevice)
		m_pD2DDevice->Release();
	if (m_pD2DFactory)
		m_pD2DFactory->Release();
	if (m_pD3DDevice)
		m_pD3DDevice->Release();
	if (m_pDXGIDevice)
		m_pDXGIDevice->Release();
}

HRESULT ssGraphics::Create3DDevice() {
	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	return D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		featureLevels, 
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&m_pD3DDevice,
		nullptr,
		nullptr);
}

void ssGraphics::Create2DFactory() {	
	D2D1_FACTORY_OPTIONS options;

	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;

#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	DX::ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		&options,
		(void**)&m_pD2DFactory
	));
	
}

void ssGraphics::Create2DDevice() {
	DX::ThrowIfFailed(
		m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_pDXGIDevice)
	);

	DX::ThrowIfFailed(
		m_pD2DFactory->CreateDevice(m_pDXGIDevice, &m_pD2DDevice)
	);	
}

void ssGraphics::CreateContext() {
	DX::ThrowIfFailed(
		m_pD2DDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_pContext
		)
	);
}

void ssGraphics::CreateIndependentResources() {
	if (FAILED(Create3DDevice()))
		throw "Failed to create 3d device!";

	CreateImagingFactory();

	Create2DFactory();

	Create2DDevice();

	CreateContext();
}

void ssGraphics::CreateSwapChain() {
	IDXGIAdapter *pDXGIAdapter;

	DX::ThrowIfFailed(
		m_pDXGIDevice->GetAdapter(&pDXGIAdapter)
	);

	IDXGIFactory2 *pDXGIFactory;

	DX::ThrowIfFailed(
		pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pDXGIFactory)
	);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	DX::ThrowIfFailed(
		pDXGIFactory->CreateSwapChainForHwnd(
			m_pD3DDevice,
			m_hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&m_pSwapChain)
	);
}

void ssGraphics::CreateBuffers() {
	IDXGISurface *pDXGISurface;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pDXGISurface);

	float dpiX, dpiY;
	m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);

	DX::ThrowIfFailed(
		m_pContext->CreateBitmapFromDxgiSurface(pDXGISurface, &bitmapProperties, &m_pBackBuffer)
	);

	m_pContext->SetTarget(m_pBackBuffer);
}

void ssGraphics::CreateDependentResources() {
	CreateSwapChain();
	CreateBuffers();
}

void ssGraphics::Initialize() {
	CreateIndependentResources();
	CreateDependentResources();	
}

void ssGraphics::WindowResized() {
	CreateDependentResources();
}

void ssGraphics::Present() {
	m_pSwapChain->Present(1, 0);
}

void ssGraphics::CreateImagingFactory() {
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory2,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory2,
		(void**)&m_pImagingFactory
	);
	DX::ThrowIfFailed(hr);
}

ID2D1DeviceContext *ssGraphics::GetContext() {
	return m_pContext;
}

ID2D1Bitmap *ssGraphics::LoadBitmapFromFile(
	LPCWSTR uri,
	UINT destWidth,
	UINT destHeight)
{
	IWICBitmapDecoder     *pDecoder   = NULL;
	IWICBitmapFrameDecode *pSource    = NULL;
	IWICStream            *pStream    = NULL;
	IWICFormatConverter   *pConverter = NULL;
	IWICBitmapScaler      *pScaler    = NULL;
	ID2D1Bitmap           *pResult    = NULL;

	// Create a decoder based on the filename passed to us.
	HRESULT hr = m_pImagingFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	DX::ThrowIfFailed(hr);

	// Assume that the file has a single frame, and we are loading the bitmap
	// from that single frame.
	hr = pDecoder->GetFrame(0, &pSource);
	DX::ThrowIfFailed(hr);

	// Create a FormatConverter... regardless of the format of the image data
	// in the file passed to us, the bitmap we are creating will be in the format
	// 32bppPBGRA
	hr = m_pImagingFactory->CreateFormatConverter(&pConverter);
	DX::ThrowIfFailed(hr);

	// Initialize the format converter.
	pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeMedianCut
	);
	DX::ThrowIfFailed(hr);

	// Create the bitmap
	hr = m_pContext->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&pResult
	);
	DX::ThrowIfFailed(hr);

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	
	return pResult;
}


ID2D1Bitmap *ssGraphics::LoadBitmapFromResource(
	LPCWSTR resourceName,
	LPCWSTR resourceType,
	UINT destWidth,
	UINT destHeight
)
{
	return nullptr;
}


