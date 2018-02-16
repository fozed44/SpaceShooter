#include "ssCore.h"

using namespace ss;

ssGlobal* ssGlobal::m_pInstance{ nullptr };

ssGlobal::ssGlobal(HWND hwnd) {
	m_pGraphics = new ssGraphics(hwnd);
	m_pGraphics->Initialize();

	m_pGame     = new ssGame();

	m_pGameLoop = new ssGameLoop();
}

ssGlobal::~ssGlobal() {
	if (m_pGameLoop)
		delete m_pGameLoop;
	if (m_pGame)
		delete m_pGame;
	if (m_pGraphics)
		delete m_pGraphics;
}

void ssGlobal::Initialize(HWND hwnd) {
	m_pInstance = new ssGlobal(hwnd);
};

void ssGlobal::SafeRelease() {
	if (m_pInstance)
		delete m_pInstance;
};