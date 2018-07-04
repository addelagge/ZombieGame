#pragma once
#include "TextureCache.h"
#include <string>

namespace GameEngine{

	class ResourceManager
	{
	public:
		//wrapper function som bara anropar samma funktion (getTexture) i textureCache
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;

	};
}