#include "
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
	
	//points centered at origin
	glm::vec2 halfDims(destRectangle.z / 2.0f, destRectangle.w / 2.0f);
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);

	//rotate the points
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

void SpriteBatch::init()
{
	
	createVertexArray();

}


void SpriteBatch::begin(GlyphSortType sortType /*= GlyphSortType::TEXTURE*/)
{
	_sortType = sortType;
	//För att renderbatches ska vara size 0 när renderBatch anropas. Clear frigör inte minnet utan ändrar bara size() till 0.
	_renderBatches.clear();

	_glyphs.clear();

}

//sortGlyphs and createRenderbatches
void SpriteBatch::end()
{
	//Set up all pointers for fast sorting
	_glyphPointers.resize(_glyphs.size());
	for (int i = 0; i < _glyphs.size(); i++){
		_glyphPointers[i] = &_glyphs[i];
	}
	sortGlyphs();
	createRenderBatches();

}

//Create glyph, store info about size, position, texture in it and add the glyph to the _glyphs vector 
void SpriteBatch::draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color)
{
	//6 vertices will later be created from every glyph
	//Glyph newGlyph;	
	//add new glyph to the _glyphs vector
	_glyphs.emplace_back(destRectangle, uvRectangle, Texture, Depth, Color);

}

void SpriteBatch::draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, float angle)
{
	_glyphs.emplace_back(destRectangle, uvRectangle, Texture, Depth, Color, angle);

}

void SpriteBatch::draw(const glm::vec4 &destRectangle, const glm::vec4 &uvRectangle, GLuint Texture, float Depth, const Color &Color, const glm::vec2& dir)
{
	const glm::vec2 right(1.0f, 0.0f);
	float angle = acos(glm::dot(right, dir)); ///<vinkeln beräknas genom skalärprodukten av right & dir och så tarman acos av det.
	if (dir.y < 0.0f) angle = -angle; ///<annars kan man inte titta nedåt
	_glyphs.emplace_back(destRectangle, uvRectangle, Texture, Depth, Color, angle);

}

//bind the vao and draw arrays
void SpriteBatch::renderBatch()
{
	// Bind our VAO. This sets up the opengl state we need, including the 
	// vertex attribute pointers and it binds the VBO
	glBindVertexArray(_vao);
	for (int i = 0; i < _renderBatches.size(); i++){
		//glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}
	glBindVertexArray(0);
}

//creates a vector of vertices with the data in the _glyphs vector. If two glyphs
//have the same texture they become part of the same renderbatch. Otherwise a new batch is created. The _vbo is bound to
//GL_ARRAY_BUFFER and supplied with the data in the vector of vertices.
void SpriteBatch::createRenderBatches()
{
	// This will store all the vertices that we need to upload
	std::vector<Vertex> vertices;
	// Resize the buffer to the exact size we need so we can treat
	// it like an array. 6 is number of Vertices
	vertices.resize(_glyphPointers.size() * 6);

	if (_glyphPointers.empty()){
		return;
	}

	int offset = 0; //offset into the _vbo
	int cv = 0; // current vertex

	//first batch
	//nedanstående rad är samma som RenderBatch myRenderBatch(0, 6, _glyphPointers[0]->texture) och _renderbatches.push_back(myRenderbatch)
	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;	
	offset += 6;

	//Add all the rest of the glyphs
	for (int cg = 1; cg < _glyphPointers.size(); cg++){

		// Check if this glyph can be part of the current batch
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture){
			// Make a new batch
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
		}
		else{
			// If its part of the current batch, just increase numVertices
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

	// Bind our VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//orphan the buffer (for speed)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload the data. vertices.data() == &vertices[0]
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}


void SpriteBatch::createVertexArray()
{
	// Generate the VAO if it isn't already generated
	if (_vao == 0){
		glGenVertexArrays(1, &_vao);
	}
	//allt man gör som ändrar opgenl's state efter att man bundit en vao sparas i den. Dvs varje gång man binder ett vao så görs automatiskt alla nedanstående saker.
	glBindVertexArray(_vao);

	//Generate the VBO if it isn't already generated
	if (_vbo == 0){
		glGenBuffers(1, &_vbo);
	}

	//bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);	

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//position attrib pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

	//color attrib pointer. Sätter GL_TRUE eftersom jag satt färgvärden 0-255 och de normaliseras då till 1 (ifall de är 255).
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
	//UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

	glBindVertexArray(0);

}

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

