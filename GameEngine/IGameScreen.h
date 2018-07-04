#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace GameEngine
{
	class IMainGame;
	enum class ScreenState {
		NONE, 
		RUNNING, 
		EXIT_APPLICATION, 
		CHANGE_NEXT, 
		CHANGE_PREVIOUS
	};

	class IGameScreen
	{
	public:
		friend class ScreenList;
		IGameScreen(){
		//Empty
		}
		virtual ~IGameScreen(){
			//Empty
		}

		//returns the index of the next or previous screen when changning screens
		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		//called at beginning and end of application
		virtual void build() = 0; //<pure virtual function
		virtual void destroy() = 0; //<pure virtual function

		//Called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//called in main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		
		int getScreenIndex()const{ 
			return m_screenIndex; 
		}
		void setRunning(){ 
			m_currentState = ScreenState::RUNNING; 
		}
		ScreenState getState()const{ return m_currentState; }
		void setParentGame(IMainGame* game) { m_game = game; }
	protected:
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_game = nullptr;
		int m_screenIndex = -1;
	};

}	