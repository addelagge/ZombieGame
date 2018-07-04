#pragma once

#include <GameEngine/Window.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/SpriteFont.h>
#include <GameEngine/AudioEngine.h>
#include <GameEngine/ParticleEngine2D.h>
#include <GameEngine/ParticleBatch2D.h>
#include "Level.h"
#include "Bullet.h"

class Zombie;
class Player;


enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();

	void initShaders();

	void initLevel();

	void gameLoop();

	void updateAgents(float deltaTime);

	void updateBullets(float deltaTime);

	void processInput();

	void drawGame();

	void checkVictory();

	void prepare();

	GameEngine::Window _window;
	GameEngine::GLSLprogram _textureProgram; ///< Shader programmet
	GameEngine::InputManager _inputManager; 
	GameEngine::Camera2D _camera; ///< Huvudkameran
	GameEngine::SpriteBatch _agentSpriteBatch;
	GameEngine::AudioEngine _audioEngine;

	int _screenWidth;
	int _screenHeight;
	int _killedZombies;
	int _killedHumans;
	int _currentLevel;
	
	GameState _gameState;
	std::vector<Level *> _levels;	
	std::vector<Human *> _humans;
	std::vector<Zombie *> _zombies;	
	std::vector<Bullet> _bullets;
	Player *_player;	
};
