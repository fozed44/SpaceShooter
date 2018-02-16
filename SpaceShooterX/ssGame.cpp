#include "ssCore.h"
#include <random>

using namespace ss;

ssGame::ssGame() {};
ssGame::~ssGame() {};

int w = 0;

void ssGame::Tick(unsigned nanoseconds) 
{
	auto pGraphics = ssGlobal::Instance()->getGraphics();
	auto pContext = pGraphics->GetContext();

	D2D1_RECT_F rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 500;
	rect.bottom = 500;

	ID2D1SolidColorBrush* pBrush;
	pContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Enum::White), &pBrush);

	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;

	ID2D1SolidColorBrush* pFillBrush;
	pContext->CreateSolidColorBrush(D2D1::ColorF(r,g,b), &pFillBrush);

	pContext->BeginDraw();
	pContext->Clear(D2D1::ColorF(0));
	pContext->DrawRectangle(rect, pBrush);
	pContext->DrawRectangle(rect, pFillBrush);
	pContext->EndDraw();
	pGraphics->Present();	

	pBrush->Release();
	pFillBrush->Release();
};