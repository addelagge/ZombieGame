#pragma once
#include "Agent.h"

class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();
	virtual void update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)override;
	void init(float speed, glm::vec2 position);
	

private:
	int _frames;
	Human *getClosestHuman(std::vector<Human *> &humans);

};

