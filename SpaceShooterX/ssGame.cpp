#include "ssCore.h"
#include <random>
#include <assert.h>

using namespace ss;

ssGame::ssGame() {};
ssGame::~ssGame() {};

int w = 0;

void ssGame::Initialize() {
	m_pGraphics = ssGlobal::Instance()->getGraphics();
	
	m_pTestBitmap = m_pGraphics->LoadBitmapFromFile(
		L"C:\\Projects\\SpaceShooterX\\SpaceShooterX\\Debug\\ship.png",
		394, 347
	);
	m_Initialized = true;
}

void ssGame::Tick(unsigned nanoseconds) 
{
	assert(m_Initialized);
	auto pContext = m_pGraphics->GetContext();

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
	pContext->DrawBitmap(m_pTestBitmap);
	pContext->EndDraw();
	m_pGraphics->Present();	

	pBrush->Release();
	pFillBrush->Release();
};