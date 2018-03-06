//#pragma once
//
//#include "ssCore.h"
//
///*
//	ssGame.h
//
//	The core files specific to the space shooter game.
//*/
//
///*
//
////++ ssGame
//
//	ssGame is the core game.
//
//	ssGame::Initialize() initializes the game.
//
//	ssGame::Tick(unsigned nanoSeconds) is the number of nano-seconds
//	since the last time Tick was called. This number is managed by the
//	class that is calling Tick. In our case ssGameLoop is the container class
//	and ssGameLoop::Run is the main gameLoop that repeatedly calls
//	ssGame::Tick
//
//*/
//class ssGame {
//public:
//	ssGame();
//	~ssGame();
//
//	void Initialize();
//	void Tick(unsigned nanoSeconds);
//private:
//
//	bool             m_Initialized{ false   };
//	ss::ssGraphics  *m_pGraphics  { nullptr };
//	ID2D1Bitmap     *m_pTestBitmap{ nullptr };
//};
//
///*
////++ ssGameLoop
//
//	Container for running the game.
//*/
//class ssGameLoop {
//public:
//	ssGameLoop();
//	~ssGameLoop();
//
//	void Run();
//};