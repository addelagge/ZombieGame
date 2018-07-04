#include "Camera2D.h"
#include <iostream>

namespace GameEngine
{

	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_orthoMatrix(1.0f),
		_cameraMatrix(1.0f), 
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(500), 
		_screenHeight(500) 
	{
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenHeight = screenHeight;
		_screenWidth = screenWidth;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight); //<skapar ortho matrix från left, right , bottom, top
	}

	void Camera2D::update()
	{
		if (_needsMatrixUpdate)
		{
			//Kamera translation. Negativ position för att flytta allt uppåt om kamera rörs nedåt. För att fokus ska hamna  i mitten används +_screenwidth/height / 2 
			glm::vec3 translate(-_position.x + _screenWidth/2, -_position.y + _screenHeight/2, 0.0f); 
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//skala
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			_needsMatrixUpdate = false;
		}
	}

	//Kollar om en sak som ska ritas befinner sig i kamerans "synfält"
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
	{
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;

		//Minimumavstånd innan kollision sker
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		//Mittpunkter av det som ska ritas samt kameran
		glm::vec2 centerPos = position + dimensions / 2.0f;
		glm::vec2 centerCameraPos = _position;

		//distvec = avståndet mellan mitten på Agent och mitten på den tile man kollar
		glm::vec2 distVec = centerPos - centerCameraPos;

		//Kollisionsdjup
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//Kollision?
		if (xDepth > 0 && yDepth > 0){
			return true;
		}

		return false;
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords)
	{
		//Invertera Y
		screenCoords.y = _screenHeight - screenCoords.y;

		//Gör så att mitten av fönstret är x=0 y=0
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

		//skala koordinaterna
		screenCoords /= _scale;
		//translate with the camera position
		screenCoords += _position;
		return screenCoords;

	}
}

