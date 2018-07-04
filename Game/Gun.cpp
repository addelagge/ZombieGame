#include "Gun.h"

#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage, GameEngine::SoundEffect effect) :
	_name(name), 
	_fireRate(fireRate), 
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletSpeed(bulletSpeed),
	_bulletDamage(bulletDamage),
	_effect(effect)
{
	_frameCounter = 0;

}


Gun::~Gun()
{
}


//Avfyrar vapnet ifall man inte överskridit frekvens med vilken vapnet kan avfyras
void Gun::update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<Bullet> &bullets, float deltaTime)
{
	_frameCounter += 1.0f*deltaTime;

	if (_frameCounter > _fireRate && isMouseDown){
		_frameCounter = 0;
		fire(direction, position, bullets);
	}
}


void Gun::fire(const glm::vec2 &direction, const glm::vec2 &position, std::vector<Bullet> &bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);
	
	for (int i = 0; i < _bulletsPerShot; i++){
		_effect.play();
		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)), _bulletSpeed, _bulletDamage);
	}
}