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
