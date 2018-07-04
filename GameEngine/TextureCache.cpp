#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace GameEngine{

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	//Returnerar en GLTexture. Om den inte laddats in tidigare s� laddas den och lagras i _textureMap.
	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//auto st�r i detta fall f�r std::map<std::string, GLTexture>::iterator
		//Ser om texturepath (Key'n) finns i _texturemap. Om den g�r det returneras en pekare till den nod i map'en som inneh�ller den texture man letade.
		//Om den Key man s�kte efter inte finns returneras end(); 
		auto mit = _textureMap.find(texturePath);

		//om texturepath inte hittas s� laddes den in
		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//lagra i _textureMap
			_textureMap.insert(make_pair(texturePath, newTexture));
			return newTexture;
		}

		return mit->second;

	}
}