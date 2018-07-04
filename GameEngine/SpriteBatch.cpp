#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>

namespace GameEngine{

Glyph::Glyph(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color)
{
	texture = Texture;
	depth = Depth;

	//OBS i vec4 destRectangle (och uvRectangle) är z = width och w = height. En vec4 består av x,y,z,w.
	//Man gör kan sedan göra 6 vertices av de fyra hörnen topleft, bottomleft, bottomright, och topright
	topLeft.color = Color;
	topLeft.setPosition(destRectangle.x, destRectangle.y + destRectangle.w);
	topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

	bottomLeft.color = Color;
	bottomLeft.setPosition(destRectangle.x, destRectangle.y);
	bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

	bottomRight.color = Color;
	bottomRight.setPosition(destRectangle.x + destRectangle.z, destRectangle.y);
	bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);

	topRight.color = Color;
	topRight.setPosition(destRectangle.x + destRectangle.z, destRectangle.y + destRectangle.w);
	topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);
}

Glyph::Glyph(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, float angle)
{
	texture = Texture;
	depth = Depth;	

	//Roterar glyphen så att dess texture hamnar rätt utifrån riktningen dess 'agent' har
	glm::vec2 halfDims(destRectangle.z / 2.0f, destRectangle.w / 2.0f);
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);

	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;

	//OBS i vec4 destRectangle (och uvRectangle) är z = width och w = height. En vec4 består av x,y,z,w.
	//Man gör kan sedan göra 6 vertices av de fyra hörnen topleft, bottomleft, bottomright, och topright
	topLeft.color = Color;
	topLeft.setPosition(destRectangle.x + tl.x, destRectangle.y + tl.y);
	topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

	bottomLeft.color = Color;
	bottomLeft.setPosition(destRectangle.x + bl.x, destRectangle.y + bl.y);
	bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

	bottomRight.color = Color;
	bottomRight.setPosition(destRectangle.x + br.x, destRectangle.y + br.y);
	bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);

	topRight.color = Color;
	topRight.setPosition(destRectangle.x + tr.x , destRectangle.y + tr.y);
	topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);
}


glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle)
{
	glm::vec2 newVec;
	newVec.x = pos.x * cos(angle) - pos.y * sin(angle);
	newVec.y = pos.x * sin(angle) + pos.y *cos(angle);
	return newVec;
}

SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
{
}


SpriteBatch::~SpriteBatch()
{
}

//Skapar ett vertex array objekt, binder en vertex buffer till den
void SpriteBatch::init()
{	
	createVertexArray();
}


//Rensar _glyphs och _renderBatches vektorerna
void SpriteBatch::begin(GlyphSortType sortType /*= GlyphSortType::TEXTURE*/)
{
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();

}

//Fyller vektorn _glyphPointers med pekare på varsin Glyph i _glyhps vektorn. Görs för snabbare sortering
void SpriteBatch::end()
{
	_glyphPointers.resize(_glyphs.size());
	for (int i = 0; i < _glyphs.size(); i++){
		_glyphPointers[i] = &_glyphs[i];
	}
	sortGlyphs();
	createRenderBatches();

}

//Skapa en Glyph, lagra info om dess storlek, position samt texture och lägg till den i _glyphs vektorn
void SpriteBatch::draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, Texture, Depth, Color);
}

void SpriteBatch::draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, const glm::vec2& dir)
{
	const glm::vec2 right(1.0f, 0.0f);
	float angle = acos(glm::dot(right, dir)); ///<vinkeln beräknas genom skalärprodukten av right & dir och så tarman acos av det.
	if (dir.y < 0.0f) angle = -angle; ///<annars kan man inte titta nedåt
	_glyphs.emplace_back(destRectangle, uvRectangle, Texture, Depth, Color, angle);

}

//Bind Vao och anropa drawarrays
void SpriteBatch::renderBatch()
{
	bindVao();	
	for (int i = 0; i < _renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	unbindVao();
}

//Aktiverar vertex array objektet och de vertexbuffrar som bundits till den
void SpriteBatch::bindVao()
{
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void SpriteBatch::unbindVao()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

//Skapar en vertex vektor av data i _glyphs vektorn. Om två glypher har samma texture så blir de en del av samma renderBatch, annars skapas en ny renderbatch.
//_vbo är bunden till GL_ARRAY_BUFFER  och matas med datan i vektorn vertices
void SpriteBatch::createRenderBatches()
{
	//Lagrar alla vertices som ska laddas
	std::vector<Vertex> vertices;

	//Ändra storleken på vektorn så att den blir exakt lika stor som _glyphPointers * 6, antalet vertices per glyph
	vertices.resize(_glyphPointers.size() * 6);

	if (_glyphPointers.empty()){
		return;
	}

	int offset = 0; //offset i _vbo
	int cv = 0; // current vertex

	//Första batchen
	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;	
	offset += 6;

	//Lägg till resten av glypherna
	for (int cg = 1; cg < _glyphPointers.size(); cg++)
	{

		//Kolla om denna glyph har samma texture som den innan
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture){
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
		}

		//Om så är fallet
		else{
			//Ändra totala antalet vertices i den aktuella renderbatchen
			_renderBatches.back().numVertices += 6;
		}
		
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset += 6;
	}

	// Bind VBO'n
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//Ladda upp all data. vertices.data() == &vertices[0]
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// Unbinda VBO'n
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

//Skapar ett vertex array objekt, binder vertex buffrar till den och förbereder dessa för att senare fyllas med data
void SpriteBatch::createVertexArray()
{
	// Skapa ett vertex array object
	if (_vao == 0){
		glGenVertexArrays(1, &_vao);
	}

	//Skapa ett vertex buffer objekt
	if (_vbo == 0) {
		glGenBuffers(1, &_vbo);
	}

	//allt man gör som ändrar opgenl's state efter att man bundit en vao sparas i den. Dvs varje gång man binder ett vao så görs automatiskt alla nedanstående saker.
	glBindVertexArray(_vao);

	//bind vertex buffern
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);	

	//position attrib pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

	//color attrib pointer. Sätter GL_TRUE eftersom jag satt färgvärden 0-255 och de normaliseras då till 1 (ifall de är 255).
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
	
	//UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

	//unbind _vao
	glBindVertexArray(0);

}


//Sorterar _GlyphPointers utifrån _sortType
void SpriteBatch::sortGlyphs()
{
	switch (_sortType){
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;

	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;

	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	}
}

//Dessa tre funktioner skickas som funktionsparametrar i stable_sort och används för att sortera utifrån vad som angetts i de olika funktionerna.
bool SpriteBatch::compareFrontToBack(Glyph *a, Glyph *b)
{
	return (a->depth < b->depth);

}
bool SpriteBatch::compareBackToFront(Glyph *a, Glyph *b)
{
	return (a->depth > b->depth);

}
bool SpriteBatch::compareTexture(Glyph *a, Glyph *b)
{
	return (a->texture < b->texture);

}

}//end of namespace scope

