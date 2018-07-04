
#include "Zombie.h"
#include "Human.h"
#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>
#include <GameEngine/GameEngineErrors.h>


Zombie::Zombie() : _frames(0)
{

}


Zombie::~Zombie()
{
}

void Zombie::update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	Human *closestHuman = getClosestHuman(humans);

	//Jagar efter närmsta human oavsett avstånd
	if (closestHuman != nullptr){
		_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += _direction * _speed *deltaTime;
	} 
	
	if (collideWithLevel(levelData) == true){
		_direction = glm::rotate(_direction, randDir(randomEngine));
	}
}

//Hittar närmaste människan att jaga efter
Human* Zombie::getClosestHuman(std::vector<Human *> &humans)
{
	Human *closestHuman = nullptr;
	float smallestDistance = 100000; 
	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);
		
		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];						
		}		
	}

	return closestHuman;
}


void Zombie::init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	_textureId = GameEngine::ResourceManager::getTexture("Textures/zombie.png").id;

	_health = 150;
	_speed = speed;
	_position = position;
	_color = GameEngine::Color(255, 255, 255, 255);

}