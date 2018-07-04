#include "MainGame.h"
#include <GameEngine/GameEngine.h>
#include <GameEngine/GameEngineErrors.h>
#include <GameEngine/ResourceManager.h>
#include <SDL\SDL.h>
#include <glm\gtx\rotate_vector.hpp>
#include <iostream>
#include "Zombie.h"
#include "Player.h"
#include <random>
#include <ctime>
#include "Gun.h"
#include <algorithm>


const float ZOMBIE_SPEED = 1.3f;
const float HUMAN_SPEED = 1.0f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame() :
	_screenHeight(700),
	_screenWidth(1024),
	_gameState(GameState::PLAY),
	_player(nullptr),
	_killedHumans(0),
	_killedZombies(0)
	{
	}

MainGame::~MainGame() 
{
	for (int i = 0; i < _levels.size(); i++){
		delete(_levels[i]);
	}
	for (int i = 0; i < _humans.size(); i++){
		delete _humans[i];
	}

	for (int i = 0; i < _zombies.size(); i++){
		delete _zombies[i];
	}
}

void MainGame::run()
{	
	initSystems();
	initLevel();	
	gameLoop();
}


void MainGame::initSystems() 
{
	//initiera SDL och aktivera double buffering
	GameEngine::init();

	//initiera ljudet, måste ske efter SDL_INIT_EVERYTHING av någon anledning
	_audioEngine.init();

	//skapa och initiera ett OpenGl window. 2 Fullscreen.
	_window.create("Zombie game", _screenWidth, _screenHeight, 0);	

	initShaders();
	_agentSpriteBatch.init();
	_camera.init(_screenWidth, _screenHeight);	
}

//Skapa ett gl_program och koppla shaders till detta
void MainGame::initShaders() 
{	
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute(0, "vertexPosition");
	_textureProgram.addAttribute(1, "vertexColor");
	_textureProgram.addAttribute(2, "vertexUV");
	_textureProgram.linkShaders();
}

//Läser in level data, laddar musik/ljudeffekter, skapar människor och zombier
void MainGame::initLevel()
{
	GameEngine::Music mainTheme = _audioEngine.loadMusic("Sound/XYZ.ogg");
	mainTheme.play();
	_levels.push_back(new Level("Levels/newLevel1.txt"));
	_currentLevel = 0;	

	//Lägg till player
	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getPlayerStartPos(), &_inputManager, &_camera, &_bullets);
	_humans.push_back(_player);

	//randX & randY är en 'human's randomiserade startposition som används i init funktionen.
	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));	
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getgetWidth() -2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() -2);
	
	//Lägg till människorna
	for (int i = 1; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)*TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	//Lägg till zombierna
	const std::vector<glm::vec2> &zombieStartPos = _levels[_currentLevel]->getZombieStartPos();	
	for (int i = 0; i < zombieStartPos.size(); i++)
	{
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombieStartPos[i]);
	}
	
	//Lägg till spelarens vapen
	const float  BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 30, 1, 0.15f, BULLET_SPEED, 30, _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun", 60, 3, 0.3, BULLET_SPEED, 10, _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP5", 5, 1, 0.2f, BULLET_SPEED, 20, _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
	
}

//Evighetsloop där spelet uppdateras
void MainGame::gameLoop() 
{
	//constants
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;	
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;
	
	// Zooma ut kameran 4 ggr
	const float CAMERA_SCALE = 1.0f / 4.0f;
	_camera.setScale(CAMERA_SCALE);
	
	float previousTicks = SDL_GetTicks();

	while (_gameState != GameState::EXIT)
	{
		// Beräkna frame time i millisekunder
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		
		//Används för att få mer jämna rörelser vid variationer i frame time
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		_inputManager.update();
		processInput();

		int i = 0; // frame counter

		//Uppdaterar i max sex steg
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			//deltatime bör vara den mindre av totalDeltaTime och MAX_DELTA_TIME
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);

			totalDeltaTime -= deltaTime;			
			i++;
		}
		_camera.setPosition(_player->getPosition());
		_camera.update();		

		drawGame();
		checkVictory();				
	}
}


void MainGame::updateAgents(float deltaTime)
{
	//Uppdatera alla humans, inkl Player
	for (int i = 0; i < _humans.size(); i++){
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	// Uppdatera alla zombies
	for (int i = 0; i < _zombies.size(); i++){
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	//Human kollisioner
	for (int i = 0; i < _humans.size(); i++)
	{
		for (int j = i+1 ; j < _humans.size(); j++){
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}	

	//Zombie kollisioner
	for (int i = 0; i < _zombies.size(); i++)
	{		
		//Kolla kollisioner med andra zombies
		for (int j = i + 1; j < _zombies.size(); j++){
			_zombies[i]->collideWithAgent(_zombies[j]);
		}

		//Kolla kollisioner med humans. _human[0] är player så jag startar på 1	
		for (int j = 1; j < _humans.size(); )
		{
			if (_zombies[i]->collideWithAgent(_humans[j]) == true)
			{
				//Skapa ny zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//ta bort människan
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
				_killedHumans++;
			}
			else{
				j++;
			}
		}

		//Kolla kollision med player
		if (_zombies[i]->collideWithAgent(_player) == true){
			GameEngine::fatalError("YOU LOOSE");
		}
	}
}

//Kontrollerar om bullets kolliderar med människor, zombier eller världen
void MainGame::updateBullets(float deltaTime)
{
	bool hit = false;

	//Kollidera med världen
	for (int i = 0; i < _bullets.size(); )
	{

		//kollision?
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime) == true)
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		} 
		else{
			i++;
		}
	}

	//kollidera med människor och zombier
	for (int i = 0; i < _bullets.size(); i++)
	{
		hit = false;

		//Kontrollera kollision med zombies
		for (int j = 0; j < _zombies.size();)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				hit = true;
				
				//Ta bort zombien ifall den dör
				if (_zombies[j]->applyDamage(_bullets[i].getDamage()) == true)
				{
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_killedZombies++;
				}
				//bullet 'dör' oavsett vad som händer med zombien
				_bullets[i] = _bullets.back();
				_bullets.pop_back();

				//fullösning för att inte hoppa över en bullet i o m omflyttningen ovan
				hit = true;
				i--;
				break;
			}
			//om ingen zombie träffades kollas nästa
			else{
				j++;
			}
		}


		if (hit == false)
		{
			//Kontrollera kollision med humans, ej player (_human[0])
			for (int j = 1; j < _humans.size();)
			{
				if (_bullets[i].collideWithAgent(_humans[j]))
				{
					//Ta bort human ifall den dör
					if (_humans[j]->applyDamage(_bullets[i].getDamage()) == true)
					{
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_killedHumans++;
					}
					//bullet 'dör' oavsett vad som händer med human
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					//fullösning för att inte hoppa över en bulet i o m omflyttningen ovan
					i--;
					break;
				}
				//om ingen human träffades kollas nästa
				else{
					j++;
				}
			}
		}
	}	
}


void MainGame::processInput() 
{
	SDL_Event evnt;
	const float CAMERA_SPEED = 5.0f;
	const float SCALE = 0.1f;
	
	//Kollar vilka SDL events, om några, som skett
	while (SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type) 
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
		}
	}	
}

//Förbereder OpenGl för att börja rita
void MainGame::prepare()
{
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

void MainGame::drawGame() 
{
	prepare();

	//aktivera shadern
	_textureProgram.use();	

	//Set the camera matrix
	GLint PLocation = _textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(PLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	_levels[_currentLevel]->draw();
	_agentSpriteBatch.begin();

	const glm::vec2 agentDimensions = glm::vec2(AGENT_WIDTH);

	//rita alla humans
	for (int i = 0; i < _humans.size(); i++)
	{
		if (_camera.isBoxInView(_humans[i]->getPosition(), agentDimensions)){
			_humans[i]->draw(_agentSpriteBatch);
		}
	}

	//rita alla zombies
	for (int i = 0; i < _zombies.size(); i++)
	{
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDimensions)){
		_zombies[i]->draw(_agentSpriteBatch);
		}
	}

	//rita alla bullets
	for (int i = 0; i < _bullets.size(); i++){
		_bullets[i].draw(_agentSpriteBatch);
	}
	
	_agentSpriteBatch.end();

	//rita alla 'agents'
	_agentSpriteBatch.renderBatch();

	//deaktivera shadern
	_textureProgram.unuse();

	//Uppdatera fönstret
	_window.swapBuffer();
}



void MainGame::checkVictory()
{
	if (_zombies.empty()){
		std::printf("killed humans %d, killed zombies %d\n", _killedHumans, _killedZombies);
		GameEngine::fatalError("You win");
	}
}



