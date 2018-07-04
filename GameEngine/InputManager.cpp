#include "InputManager.h"
namespace GameEngine{

	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}


	void InputManager::pressKey(unsigned int keyID)
	{
		//kollar om KeyId finns i _keyMap. Om det inte gör det skapas det och sätts till true.
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void InputManager::update()
	{
		//kopiera _keymap to previousKeyMap
		for (auto it = _keyMap.begin(); it != _keyMap.end(); it++){
			_previousKeyMap[it->first] = it->second;
		}

	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}


	//returnerar true om knappen med keyId är nedtryckt
	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto mit = _keyMap.find(keyID);
		if (mit != _keyMap.end()){
			return mit->second;
		}
		else{
			return false;
		}
	}

	//Kontrollerar om om knappen är nedtryckt en längre stund
	bool InputManager::isKeyPressed(unsigned int KeyId)
	{
		if (isKeyDown(KeyId) == true  && wasKeyDown(KeyId) == false){
			return true;
		}
		return false;
	}

	//returnerar true om knappen med keyId var nedtryckt förra uppdateringsvarvet
	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto mit = _previousKeyMap.find(keyID);
		if (mit != _previousKeyMap.end()){
			return mit->second;
		}
		return false;
	}

}

