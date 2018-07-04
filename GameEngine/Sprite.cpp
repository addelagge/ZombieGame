#include "Sprite.h"
#include <cstddef>
#include "ResourceManager.h"
#include "vertex.h"
#include <iostream>
namespace GameEngine{
	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		//Always remember to delete your buffers when
		//you are done!
		if (_vboID != 0){
			glDeleteBuffers(1, &_vboID);
		}
		std::cout << "deleting sprite\n";

	}

	//skapar ett vertexbuffer objekt och en vertex array. Sedan binds bufferobjektet till ARRAY_BUFFER och datat i vertex arrayen buffras till den.
	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		//Generate the buffer if it hasn't already been generated
		if (_vboID == 0){
			glGenBuffers(1, &_vboID);
		}


		//This array will hold our vertex data.
		//We need 6 vertices, and each vertex has 2
		//floats for X and Y
		Vertex vertexData[6];

		//first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);

		//Set all vertex colors
		for (int i = 0; i < 6; i++){
			vertexData[i].setColor(0, 0, 255, 255);
		}
		vertexData[3].setColor(255, 0, 0, 255);
		vertexData[2].setColor(255, 0, 0, 255);
		
		//Tell opengl to bind our vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//Upload the data to the GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//Unbind the buffer (optional)
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}

	//Draws the sprite to the screen
	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		

		//Draw the 6 vertices to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//glDisableVertexAttribArray sköts i GLSLProgram unuse()

		//Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}