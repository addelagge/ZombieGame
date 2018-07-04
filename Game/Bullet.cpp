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
	//fungerar s� l�nge alla agents har samma storlek
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	//mitten av de tv� 'agents' som kollsions-kollas
	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	//avst�ndet mellan (mitten av) b�da 'agents'
	glm::vec2 distVec = centerPosA - centerPosB;

	//length r�knar ut hypotenusan av distVec
	float distance = glm::length(distVec);

	//vid en kollision �r distance mindre �n MIN_DISTANCE
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
	//konverterar coordinaterna f�r bullet f�r att kunna kolla i leveldata vektorn
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//if the bullet is outside the world
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}
	
	//om bullet befinner sig p� samma plats som n�got annat �n '.' s� har den krockat med v�rlden
	return (levelData[gridPosition.y][gridPosition.x] != '.');
		}