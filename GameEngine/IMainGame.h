#pragma once
#include <memory>
#include "GameEngine.h"
#include "Window.h"
#include "InputManager.h"


namespace GameEngine{
	
	class IGameScreen;
	class ScreenList;

	class IMainGame	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void ExitGame();

		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;
		
		void onSDLEvent(SDL_Event& evnt);
		const float getFps() const { return m_fps; }
		InputManager inputManager;

	protected:
		virtual void update();
		virtual void draw();
		
		bool init();
		bool initSystems();
		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen;
		bool m_isRunning = false;
		float m_fps = 0.0f;
		Window m_window;
		
	};

}