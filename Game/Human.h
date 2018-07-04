#pragma once
#include "Agent.h"

class Zombie;

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 position);
	virtual void update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)override;
	

private:
	int _frames;
	Zombie* getClosestZombie(std::vector<Zombie *> &zombies);
};

