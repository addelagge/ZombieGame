#pragma once
#include <vector>

namespace GameEngine{
	
	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();
		IGameScreen* getCurrent();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);
		void destroy();

	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
	};

}