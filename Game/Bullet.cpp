#include "Bullet.h"
#include <GameEngine/ResourceManager.h>
#include "Zombie.h"
#include "Human.h"
#include "Agent.h"
#include "Level.h"



Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, float damage) : 
	_position(position), 
	_direction(direction), 
	_speed(speed), 
	_damage(damage)
{
}


Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string> &levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;	
	return collideWithWorld(levelData);	
}

void Bullet::draw(GameEngine::SpriteBatch &spriteBatch)
{
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/circle.png");
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	GameEngine::Color color = GameEngine::Color(0, 0, 0, 255);
	spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent *agent)
{
	//fungerar så länge alla agents har samma storlek
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	//mitten av de två 'agents' som kollsions-kollas
	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	//avståndet mellan (mitten av) båda 'agents'
	glm::vec2 distVec = centerPosA - centerPosB;

	//length räknar ut hypotenusan av distVec
	float distance = glm::length(distVec);

	//vid en kollision är distance mindre än MIN_DISTANCE
	float collisionDepth = MIN_DISTANCE - distance;

	//Kollision
	if (collisionDepth > 0){
		
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string> &levelData)
{
	glm::ivec2 gridPosition;
	//konverterar coordinaterna för bullet för att kunna kolla i leveldata vektorn
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//if the bullet is outside the world
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}
	
	//om bullet befinner sig på samma plats som något annat än '.' så har den krockat med världen
	return (levelData[gridPosition.y][gridPosition.x] != '.');
		}