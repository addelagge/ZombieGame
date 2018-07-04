#pragma once
#include "Human.h"
#include <GameEngine/InputManager.h>
#include "Bullet.h"
#include <GameEngine/Camera2D.h>

class Gun;

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, GameEngine::InputManager *inputmanager, GameEngine::Camera2D *camera, std::vector<Bullet> *bullets);
	void update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)override;
	void addGun(Gun *gun);

	
private:
	GameEngine::InputManager *_inputManager;
	std::vector<Gun *> _guns;
	int _currentGunIndex;
	GameEngine::Camera2D *_camera;
	std::vector<Bullet> *_bullets;
};

