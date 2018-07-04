#pragma once
#include <map>
#include "GLTexture.h"

namespace GameEngine{
	
	//Cache'ar alla textures s� att de effektivt kan anv�ndas flera g�nger
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		//Gltexture �r en struct best�ende av lite olika parametrar som en texture har, typ 'id' 
		GLTexture getTexture(std::string texturePath);

	private:
		//Maps liknar dictionary i python och best�r av Key och Value. H�r �r Key en string och value en GLTexture.
		std::map<std::string, GLTexture> _textureMap;

	};

}