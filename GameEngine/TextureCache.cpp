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

	//Returnerar en GLTexture. Om den inte laddats in tidigare så laddas den och lagras i _textureMap.
	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//auto står i detta fall för std::map<std::string, GLTexture>::iterator
		//Ser om texturepath (Key'n) finns i _texturemap. Om den gör det returneras en pekare till den nod i map'en som innehåller den texture man letade.
		//Om den Key man sökte efter inte finns returneras end(); 
		auto mit = _textureMap.find(texturePath);

		//om texturepath inte hittas så laddes den in
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