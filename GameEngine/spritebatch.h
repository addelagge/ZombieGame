#pragma once
#include <GL\glew.h>
#include "vertex.h"
#include <glm\glm.hpp>
#include <vector>

namespace GameEngine{

enum class GlyphSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE

};

//typ en sprite best�ende av 6 vertices (tv� av de fyra kommer att anv�ndas tv� g�nger)
class Glyph
{
	public:
		Glyph(){};
		Glyph(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color);
		Glyph(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, float angle); ///<overload

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	private:
		glm::vec2 rotatePoint(glm::vec2 pos, float angle);

};


class RenderBatch
{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture){
		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
};


class SpriteBatch
{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		void bindVao();
		void unbindVao();
	
		//parametrarna i draw() �r 'by reference' bara f�r att det g�r snabbare f�r processorn att slippa g�ra kopior av alla v�rden. Texture �r bara en variabel 
		//s� det kan man skicka en kopia av. Const �r f�r att man inte ska r�ka �ndra �ndra v�rdet man skickar in i funktionen.	
		void draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color);	

		void draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, const glm::vec2& dir);///<overload

		void renderBatch();

	private: 
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();
		static bool compareFrontToBack(Glyph *a, Glyph *b);
		static bool compareBackToFront(Glyph *a, Glyph *b);
		static bool compareTexture(Glyph *a, Glyph *b);

		GLuint _vbo;
		GLuint _vao;
		std::vector<Glyph *> _glyphPointers; ///f�r snabbare sortering
		std::vector<Glyph> _glyphs; 
		GlyphSortType _sortType;
		std::vector<RenderBatch> _renderBatches;
	};

}