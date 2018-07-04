#pragma once

#include <GL/glew.h>
#include "GLTexture.h"
#include <string>

namespace GameEngine{
	// A 2D quad that can be rendered to the screen
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

	private:
		float _x;
		float _y;
		float _height;
		float _width;
		GLuint _vboID;
		GLTexture _texture;

	};

}