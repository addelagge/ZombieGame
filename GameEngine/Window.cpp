#include "Window.h"
#include "GameEngineErrors.h"
#include <iostream>
namespace GameEngine{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	//Skapar och initierar ett SDL window
	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentflags)
	{
		_screenHeight = screenHeight;
		_screenWidth = screenWidth;
		Uint32 flags = SDL_WINDOW_OPENGL;		

		//Här sätts olika screens modes
		if (currentflags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
			std::cout << "Invisible mode\n";
		}

		if (currentflags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			std::cout << "Fullscreen mode\n";
		}

		if (currentflags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
			std::cout << "Borderless mode\n";
		}

		//Öppnar ett SDL window
		_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_window == nullptr)	{
			fatalError("SDL window create error");
		}

		//Skapar ett OpenGL context och associerar det med _window
		SDL_GLContext glContext = SDL_GL_CreateContext(_window);
		if (glContext == nullptr)	{
			fatalError("SDL Gl context error");
		}

		//Initiera glew 
		GLenum error = glewInit();
		if (error != GLEW_OK)	{
			fatalError("Glew init error");
		}

		std::printf("*** OpenGL version %s ***\n", glGetString(GL_VERSION));

		//Sätt bakgrundsfärg
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

		//Sätt Vsync att uppdatera omedelbart
		SDL_GL_SetSwapInterval(0);

		//Aktivera alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	//uppdaterar _window
	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_window);
	}
}