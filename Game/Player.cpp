
#include "Player.h"
#include <SDL\SDL.h>
#include "Gun.h"
#include <iostream>

Player::Player() : _currentGunIndex(-1)
{
}


Player::~Player()
{
}


void Player::init(float speed, glm::vec2 position, GameEngine::InputManager *inputmanager, GameEngine::Camera2D *camera, std::vector<Bullet> *bullets)
{
	_textureId = GameEngine::ResourceManager::getTexture("Textures/player.png").id;
	_bullets = bullets;
	_camera = camera;
	_inputManager = inputmanager;
	_health = 150;
	_speed = speed;
	_position = position;
	_color = GameEngine::Color(255, 255, 255, 255);
}

//Kollar input från keyboard och mus samt kontrollerar om det skett en några kollisioner med världen
void Player::update(const std::vector<std::string> &levelData, std::vector<Human *> &humans, std::vector<Zombie *> &zombies, float deltaTime)
{
	//Rörelse i sidled
	if (_inputManager->isKeyDown(SDLK_a)){
		_position.x -= _speed*deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_d)){
		_position.x += _speed*deltaTime;
	}

	//Rörelse framåt/bakåt
	if (_inputManager->isKeyDown(SDLK_w)){
		_position.y += _speed*deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s)){
		_position.y -= _speed*deltaTime;
	}	

	//Byte av vapen
	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
		std::printf("Equipped %s\n", _guns[_currentGunIndex]->getName().c_str());
	}
	else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
		std::printf("Equipped %s\n", _guns[_currentGunIndex]->getName().c_str());
	}
	else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
		std::printf("Equipped %s\n", _guns[_currentGunIndex]->getName().c_str());
	}
	
	//Så att player tittar åt muspekarens håll
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1){
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
										centerPosition, _direction, *_bullets, deltaTime);
	}

	collideWithLevel(levelData);	
}


//Lägger till ett nytt vapen till spelarens arsenal
void Player::addGun(Gun *gun)
{
	_guns.push_back(gun);

	if (_currentGunIndex == -1){
		_currentGunIndex = 0;
	}

	std::printf("Added %s\n", gun->getName().c_str());
}