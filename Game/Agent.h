#pragma once
#include <glm\glm.hpp>
#include <GameEngine/spritebatch.h>
#include <GameEngine/ResourceManager.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;


class Agent
{
public:
	Agent();
	virtual ~Agent();
	void draw(GameEngine::SpriteBatch &spriteBatch);
	virtual void update(const std::vector<std::string> &_levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime) = 0; //pure virtual function
	glm::vec2 getPosition() const { return _position; }

	bool collideWithLevel(const std::vector<std::string> &levelData);
	
	bool collideWithAgent(Agent *agent);

	bool applyDamage(float damage);

protected:
	glm::vec2 _position;
	glm::vec2 _direction = glm::vec2(1.0f, 0.0f);
	float _speed;
	GameEngine::Color _color;

	void checkTilePosition(float x, float y, const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePositions);
	void collideWithTile(glm::vec2 tilePos);
	float _health;
	GLuint _textureId;
};

