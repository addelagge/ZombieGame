#pragma once
#include <GL\glew.h>

namespace GameEngine{

	struct Position
	{
		float x;
		float y;

	};

	struct Color
	{
		Color() : r(0), g(0), b(0), a(255) 
		{ 
		
		}

		Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A){
			r = R;
			g = G;
			b = B;
			a = A;
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

	};

	//UV texture coordinates
	struct UV
	{
		float u;
		float v;
	};

	//Det är bra att en Vertex är en multipel av 4 bytes. I detta fall 2floats (2x4 bytes) + 4ubytes (1x4 bytes) = 12 bytes;
	struct Vertex
	{
		Position position;
		
		//4 bytes för r g b a färg.
		Color color;
		
		//UV texture koordinater
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a){
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setPosition(float x, float y){
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v){
			uv.u = u;
			uv.v = v;
		}

	};

}