#pragma once

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d3d11_1.h>

#include <d2d1effects.h>
#include <d2d1effecthelpers.h>

#include <dwrite_1.h>

namespace ss {

	class ssLocator {
	public:
	};

	struct ssGraphicsResources {
		IDXGIDevice	         *m_pDXGIDevice { nullptr };
		IDXGISwapChain1      *m_pSwapChain  { nullptr };
		ID3D11Device         *m_pD3DDevice  { nullptr };
		ID2D1Factory1	     *m_pD2DFactory { nullptr };
		ID2D1Device          *m_pD2DDevice  { nullptr };
		ID2D1DeviceContext   *m_pContext    { nullptr };
		ID2D1Bitmap1	     *m_pBackBuffer { nullptr };
		
		void Release() {
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
		};
	};
	
	class ssGraphics {
	public:
		ssGraphics(HWND hwnd);
		~ssGraphics();
		
		void Initialize();
		void WindowResized();

	private:
		HRESULT Create3DDevice();
		void Create2DFactory();
		void Create2DDevice();
		void CreateContext();

		void CreateSwapChain();
		void CreateBuffers();

		void CreateIndependentResources();
		void CreateDependentResources();

		HWND                m_hwnd;

		IDXGIDevice			 *m_pDXGIDevice;
		IDXGISwapChain1      *m_pSwapChain;
		ID3D11Device         *m_pD3DDevice;
		ID2D1Factory1	     *m_pD2DFactory;
		ID2D1Device          *m_pD2DDevice;
		ID2D1DeviceContext   *m_pContext;
		ID2D1Bitmap1	     *m_pBackBuffer;
	};

};
