#include "Agent.h"
#include "Level.h"
#include <iostream>
#include <algorithm>


Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::draw(GameEngine::SpriteBatch &spriteBatch)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	spriteBatch.draw(destRect, uvRect, _textureId, 0.0f, _color, _direction);
}

//kollar de fyra "h�rnen" n�rmast den 'agent' som anropade funktionen.
bool Agent::collideWithLevel(const std::vector<std::string> &levelData)
{	
	std::vector<glm::vec2> collideTilePositions;	
	
	//F�rsta h�rnet
	checkTilePosition(_position.x, _position.y, levelData, collideTilePositions);	
	//Andra h�rnet
	checkTilePosition(_position.x, _position.y + AGENT_WIDTH, levelData, collideTilePositions);
	//Tredje h�rnet
	checkTilePosition(_position.x + AGENT_WIDTH, _position.y, levelData, collideTilePositions);
	//Fj�rde h�rnet
	checkTilePosition(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH, levelData, collideTilePositions);
	
	if (collideTilePositions.size() == 0){
		return false;
	}

	for (int i = 0; i < collideTilePositions.size(); i++){
		collideWithTile(collideTilePositions[i]);
	}

	return true;
}


bool Agent::collideWithAgent(Agent *agent)
{
	//fungerar s� l�nge alla agents har samma storlek
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	//mitten av de tv� 'agents' som kollsions-kollas
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	//avst�ndet mellan (mitten av) b�da 'agents'
	glm::vec2 distVec = centerPosA - centerPosB;	

	//length r�knar ut hypotenusan av distVec
	float distance = glm::length(distVec);	

	//vid en kollision �r distance mindre �n MIN_DISTANCE
	float collisionDepth = MIN_DISTANCE - distance;
	
	//Kollision
	if (collisionDepth > 0)
	{		
		//normalize ber�knar hypotenusan av distVec och omvandlar den till '0'. Man f�r allts� en vektor i samma rikting fast med l�ngden '1'.
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;		
		
		//de som krockar flyttas b�da bak�t med h�lften av kollisionsdjupet
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;

		return true;
	}
	return false;

}

void Agent::checkTilePosition(float x, float y, 
							  const std::vector<std::string> &levelData, 
							  std::vector<glm::vec2> &collideTilePositions)
{
	
	//konverterar coordinaterna f�r 'tile' f�r att kunna kolla i leveldata vektorn
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), 
									floor(y / (float)TILE_WIDTH));

	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		std::printf("outside level\n");
		std:printf("%f %f\n", cornerPos.x, cornerPos.y);
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		//koordinaterna f�r 'tile' konverteras tillbaka till world coordinates och adderas med TILE_WIDth f�r att f� mitten p� tile'n
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f)); 
	}
}


//AABB collision
void Agent::collideWithTile(glm::vec2 tilePos)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	//distvec = avst�ndet mellan mitten p� Agent och mitten p� den tile man kollar
	glm::vec2 distVec = centerPlayerPos - tilePos;

	//kollisionsdjup
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0)
	{

		//samma sak som  if (xdepth < ydepth) ?
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			
			//om Agent �r v�ster om hindret
			if (distVec.x < 0) {
				_position.x -= xDepth;
			}
			//om Agent �r �ster om hindret
			else{
				_position.x += xDepth;
			}
		}
		else
		{ 
			//om Agent �r s�der om hindret
			if (distVec.y < 0){
				_position.y -= yDepth;
			}
			//om Agent �r norr om hindret
			else{
				_position.y += yDepth;
			}
		}		
	}
}

bool Agent::applyDamage(float damage)
{
	_health -= damage;
	if (_health <= 0){
		return true;
	}
	return false;

}