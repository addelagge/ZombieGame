#include "GameEngineErrors.h"
#include <cstdlib>
#include <SDL/SDL.h>
#include <iostream>

namespace GameEngine{
	//Skriver ut ett error meddelande och avslutar spelet
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}
}