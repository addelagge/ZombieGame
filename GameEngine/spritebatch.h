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

//typ en sprite bestående av 6 vertices (två av de fyra kommer att användas två gånger)
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
	
		//parametrarna i draw() är 'by reference' bara för att det går snabbare för processorn att slippa göra kopior av alla värden. Texture är bara en variabel 
		//så det kan man skicka en kopia av. Const är för att man inte ska råka ändra ändra värdet man skickar in i funktionen.	
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
		std::vector<Glyph *> _glyphPointers; ///för snabbare sortering
		std::vector<Glyph> _glyphs; 
		GlyphSortType _sortType;
		std::vector<RenderBatch> _renderBatches;
	};

}