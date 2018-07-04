
#include "Level.h"
#include <fstream>
#include <GameEngine/GameEngineErrors.h>
#include <iostream>
#include <GameEngine/ResourceManager.h>

Level::Level(const std::string &fileName)
{	
	std::ifstream inFile(fileName);
	if (inFile.fail()){
		GameEngine::fatalError("Failed to open " + fileName);
	}

	//Hoppa över första ordet i filen och läs sedan in antalet människor
	std::string temp;
	inFile >> temp >>_numHumans;
	std::getline(inFile, temp);
	
	//Läs in resten av filen i _leveData vektorn
	while (getline(inFile, temp)){
		_levelData.push_back(temp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	//Texture koordinater
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::Color whiteColor = GameEngine::Color(255,255,255,255);

	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			char tile = _levelData[y][x];

			//Position
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);			

			switch (tile)
			{
				case'B':
				case 'R':					
					_spriteBatch.draw(destRect, uvRect, GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
					break;
				case 'G':
					_spriteBatch.draw(destRect, uvRect, GameEngine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
					break;
				case 'L':
					_spriteBatch.draw(destRect, uvRect, GameEngine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
					break;
				case '@':
					_levelData[y][x] = '.'; 
					_playerStartPos.x = x * TILE_WIDTH;
					_playerStartPos.y = y * TILE_WIDTH;
					break;
				case 'Z':
					_levelData[y][x] = '.'; 
					_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;
				case '.':
					break;

				default:
					std::printf("unexpected symbol %c at %d %d\n", tile, x, y);
					break;
			}
		}
	}	

	_spriteBatch.end();
}


Level::~Level()
{
}



void Level::draw()
{
	_spriteBatch.renderBatch();

}