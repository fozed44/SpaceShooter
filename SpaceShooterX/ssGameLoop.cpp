#include "ssCore.h"
#include <chrono>
#include "windows.h"
#include "resource.h"

using namespace ss;
using namespace std::chrono_literals;

// use a fixed time step of 1 / ( 60 fps ) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

ssGameLoop::ssGameLoop() {};
ssGameLoop::~ssGameLoop() {};

void ssGameLoop::Run() {

	// Alias the high res clock;
	using clock = std::chrono::high_resolution_clock;

	tagMSG msg;

	auto last_frame_time = clock::now();

	auto pGame = ssGlobal::Instance()->getGame();
	pGame->Initialize();
	while (1)
	{
		auto delta_time = clock::now() - last_frame_time;
		last_frame_time = clock::now();
		
		auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_COMMAND:
			{
				int wmId = LOWORD(msg.wParam);
				// Parse the menu selections:
				switch (wmId)
				{
				case IDM_ABOUT:
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(msg.hwnd);
					break;
				default:
					DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				}
			}
			break;
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(msg.hwnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(msg.hwnd, &ps);
			}
			break;
			case WM_DESTROY:
				PostQuitMessage(0);

				//if (pGraphics)
				//	delete pGraphics;

				ss::ssGlobal::SafeRelease();
				break;
			default:
				DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			}
		}

		if (msg.message == WM_QUIT)
			break;

		pGame->Tick(nanoSeconds.count());
	}

};