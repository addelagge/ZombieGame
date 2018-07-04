#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace GameEngine{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();
		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		//setters
		void setPosition(const glm::vec2 &newPosition){ _position = newPosition; _needsMatrixUpdate = true; }
		void setScale(float newScale){ _scale = newScale; _needsMatrixUpdate = true; }

		//getters
		glm::vec2 getPosition(){ return _position; }
		float getScale(){ return _scale; }
		glm::mat4 getCameraMatrix(){ return _cameraMatrix; }

	private:
		int _screenHeight, _screenWidth;
		bool _needsMatrixUpdate;
		float _scale; ///<för att zooma
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;

	};

}