#pragma once
#include <string>
#include <vector>
#include <GameEngine/SpriteBatch.h>
#include <glm\glm.hpp>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string &fileName);
	~Level();

	void draw();	
	
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPos; }
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	int getNumHumans() const { return _numHumans; }
	int getgetWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	glm::vec2 getPlayerStartPos() const { return _playerStartPos; }
	
private:
	std::vector<std::string> _levelData;
	int _numHumans;
	GameEngine::SpriteBatch _spriteBatch;
	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombieStartPos;

};

