#pragma once
#include <map>
#include "GLTexture.h"

namespace GameEngine{
	
	//Cache'ar alla textures så att de effektivt kan användas flera gånger
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		//Gltexture är en struct bestående av lite olika parametrar som en texture har, typ 'id' 
		GLTexture getTexture(std::string texturePath);

	private:
		//Maps liknar dictionary i python och består av Key och Value. Här är Key en string och value en GLTexture.
		std::map<std::string, GLTexture> _textureMap;

	};

}