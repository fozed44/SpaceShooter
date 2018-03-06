#pragma once

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d3d11_1.h>

#include <d2d1effects.h>
#include <d2d1effecthelpers.h>

#include <dwrite_1.h>

#include <wincodec.h>

/*

	ssCore

	Defines the core low level direct x classes.

*/
namespace ss {
	

	class ssGraphics {
	public:
		ssGraphics(HWND hwnd);
		~ssGraphics();
		
		void Initialize();
		void WindowResized();

	public:
		ssGraphics(ssGraphics&)  = delete;
		ssGraphics(ssGraphics&&) = delete;

	public:
		void Present();
		ID2D1DeviceContext *GetContext();

	public:
		ID2D1Bitmap *LoadBitmapFromFile(LPCWSTR uri, unsigned int destWidth, unsigned int destHeight);
		ID2D1Bitmap *LoadBitmapFromResource(LPCWSTR rescourceName, LPCWSTR rescourceType, unsigned int destWidth, unsigned int destHeight);

	private:
		HRESULT Create3DDevice();
		void Create2DFactory();
		void Create2DDevice();
		void CreateContext();

		void CreateSwapChain();
		void CreateBuffers();
		void CreateImagingFactory();

		void CreateIndependentResources();
		void CreateDependentResources();

		HWND                m_hwnd;

		IWICImagingFactory2  *m_pImagingFactory;
		IDXGIDevice			 *m_pDXGIDevice;
		IDXGISwapChain1      *m_pSwapChain;
		ID3D11Device         *m_pD3DDevice;
		ID2D1Factory1	     *m_pD2DFactory;
		ID2D1Device          *m_pD2DDevice;
		ID2D1DeviceContext   *m_pContext;
		ID2D1Bitmap1	     *m_pBackBuffer;

	};

	/*
	//++ ssGame

	ssGame is the core game.

		ssGame::Initialize() initializes the game.

		ssGame::Tick(unsigned nanoSeconds) is the number of nano - seconds
		since the last time Tick was called.This number is managed by the
		class that is calling Tick.In our case ssGameLoop is the container class
		and ssGameLoop::Run is the main gameLoop that repeatedly calls
		ssGame::Tick

	*/
	class ssGame {
		public:
			ssGame();
			~ssGame();

			void Initialize();
			void Tick(unsigned nanoSeconds);
		private:

			bool             m_Initialized{ false };
			ss::ssGraphics  *m_pGraphics{ nullptr };
			ID2D1Bitmap     *m_pTestBitmap{ nullptr };
	};

	/*
	//++ ssGameLoop

		Container for running the game.
	*/
	class ssGameLoop {
	public:
		ssGameLoop();
		~ssGameLoop();

		void Run();
	};
	
	class ssGlobal {
	public:
		~ssGlobal();
	private:
		ssGlobal(HWND hwnd);

	public:
		static ssGlobal* Instance() { return m_pInstance; }
		static void      Initialize(HWND hwnd);
		static void		 SafeRelease();

	public:
		ssGame*     getGame()     { return m_pGame; }
		ssGraphics* getGraphics() { return m_pGraphics; }
		ssGameLoop* getGameLoop() { return m_pGameLoop; }
	private:
		static ssGlobal* m_pInstance;

		ssGame     *m_pGame    { nullptr };
		ssGraphics *m_pGraphics{ nullptr };
		ssGameLoop *m_pGameLoop{ nullptr };
	};

	template <class T> void SafeRelease(T **ppT) {
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

};
