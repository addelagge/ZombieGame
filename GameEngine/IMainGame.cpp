#include "IMainGame.h"
#include "Timing.h"
#include "IGameScreen.h"
#include "ScreenList.h"
#include <iostream>

namespace GameEngine {
	IMainGame::IMainGame() 
	{
		m_screenList = std::make_unique<ScreenList>(this); //<creates screen list with 'this' game as the owner game
	}

	IMainGame::~IMainGame()
	{
	}

	void IMainGame::run()
	{
		if (init() == false) return;
		FpsLimiter limiter;
		limiter.setMaxFps(60.0f);
		m_isRunning = true;
		while (m_isRunning){
			limiter.begin();
			inputManager.update();
			update();
			draw();
			m_fps = limiter.end();
			m_window.swapBuffer();
		}
	}

	void IMainGame::ExitGame()
	{
		std::cout << "exitgame";
		m_currentScreen->onExit();
		if (m_screenList){
			m_screenList->destroy();
			m_screenList.reset();
		}
		m_isRunning = false;

	} 

	void IMainGame::onSDLEvent(SDL_Event& evnt)
	{
		switch (evnt.type) {
		case SDL_QUIT:
			ExitGame();
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(evnt.key.keysym.sym);

			break;
		case SDL_KEYUP:
			inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(evnt.button.button);

			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(evnt.button.button);
			break;
		}

	}
	
	bool IMainGame::init()
	{
		GameEngine::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		if(initSystems() == false) return false;
		onInit();
		addScreens();
		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();
		return true;

	}
	bool IMainGame::initSystems()
	{
		m_window.create("Default", 1300, 750, 0);
		return true;
		
	}


	void IMainGame::update()
	{
		if (m_currentScreen != nullptr){
			switch (m_currentScreen->getState()){
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen != nullptr){
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen != nullptr){
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				ExitGame();
				break;
			default:
				break;
			} 
		}
		else{
			ExitGame();
		}

	}
	void IMainGame::draw()
	{
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen != nullptr && m_currentScreen->getState() == ScreenState::RUNNING){
			m_currentScreen->draw();

		}

	}
}