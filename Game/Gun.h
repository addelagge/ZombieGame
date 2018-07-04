#pragma once
#include <string>
#include <vector>
#include "Bullet.h"
#include <GameEngine/AudioEngine.h>

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage, GameEngine::SoundEffect effect);
	~Gun();

	void update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<Bullet> &bullets, float deltaTime);
	std::string getName(){ return _name; }

private:

	void fire(const glm::vec2 &direction, const glm::vec2 &position, std::vector<Bullet> &bullets);

	std::string _name;
	int _fireRate; //Hur snabbt det går att ladda om
	int _bulletsPerShot; 
	float _spread; //Träffsäkerhet

	float _bulletSpeed;
	float _bulletDamage;
	float _frameCounter;
	GameEngine::SoundEffect _effect;
};

