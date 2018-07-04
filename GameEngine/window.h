#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

namespace GameEngine{

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentflags);
		int getScreenWidth(){ return _screenWidth; }
		int getScreenHeight(){ return _screenHeight; }
		void swapBuffer();

	private:
		SDL_Window* _window;
		int _screenHeight;
		int _screenWidth;
	};

}