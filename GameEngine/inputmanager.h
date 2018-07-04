#pragma once
#include <unordered_map>
#include <glm\glm.hpp>

namespace GameEngine{
class InputManager
{
public:
	InputManager();
	~InputManager();
	
	void pressKey(unsigned int keyID);
	
	void releaseKey(unsigned int keyID);
	
	void update();

	bool isKeyDown(unsigned int keyID);
	
	bool isKeyPressed(unsigned int KeyId);
	
	void setMouseCoords(float x, float y);
	
	glm::vec2 const getMouseCoords(){ return _mouseCoords; }

private:
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	glm::vec2 _mouseCoords;
	bool wasKeyDown(unsigned int keyID);

};

}