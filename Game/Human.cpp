
#include "Human.h"
#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>
#include "Zombie.h"
#include <GameEngine/GameEngineErrors.h>
#include <iostream>

Human::Human() : _frames(0)
{
}


Human::~Human()
{
}


void Human::init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));
	//F�r att generera en slumpad riktning
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);	
	_textureId = GameEngine::ResourceManager::getTexture("Textures/human.png").id;
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	
	if (_direction.length() == 0) {
		_direction = glm::vec2(1.0f, 0.0f);
	}

	_direction = glm::normalize(_direction);
	_health = 20;
	_speed = speed;
	_position = position;
	_color =GameEngine::Color(255, 255, 255, 255);
}

void Human::update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)
{
	Zombie *closestZombie = getClosestZombie(zombies);
	if (closestZombie != nullptr)
	{
		static std::mt19937 randomEngine(time(nullptr));
		static std::uniform_real_distribution<float> randRotate(-1.0f, 1.0f);
		glm::vec2 zombiePos = closestZombie->getPosition();

		//Om n�rmsta zombie �r tillr�ckligt n�ra s� flyr m�nniskan. 
		if (glm::length(glm::vec2(zombiePos - _position)) < 300){
			_direction = glm::normalize(_position - zombiePos);
		}
		else
		{
			//Efter att m�nniskan r�rt sig i samma riktning tjugo ggr s� ges den en ny r�relseriktning
			if (_frames == 20)
			{
				_direction = glm::rotate(_direction, randRotate(randomEngine));
				_frames = 0;
			}
			else{
				_frames++;
			}
		}

		_position += _direction *_speed * deltaTime;
		if (collideWithLevel(levelData) == true){
			_direction = glm::rotate(_direction, randRotate(randomEngine));
		}
	}	
}

//Identifierar den zombie som �r n�rmast
Zombie* Human::getClosestZombie(std::vector<Zombie *> &zombies)
{
	Zombie *closestZombie = nullptr;
	float closestDistance = 10000;
	for (int i = 0; i < zombies.size(); i++)
	{
		glm::vec2 distVec = zombies[i]->getPosition() - _position;
		float distance = glm::length(distVec);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestZombie = zombies[i];
		}
	}
	return closestZombie;
}