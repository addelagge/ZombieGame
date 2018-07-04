#include "ImageLoader.h"
#include "PicoPNG.h"
#include "IOManager.h"
#include "GameEngineErrors.h"

namespace GameEngine{

	//används i texturecash för att hämta en ny texture
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		//Skapar en GLTexture och initialiserar all fält till 0
		GLTexture texture = {};

		//Input till decodePNG
		std::vector<unsigned char> in;
		
		//Output från decodePNG
		std::vector<unsigned char> out;
		unsigned long height, width;

		//Läser in innehållet i bildfilen till buffern 'in'
		if (IOManager::readFileToBuffer(filePath, in) == false){
			fatalError("failed to load PNG file to buffer");
		}

		//Avkoda .png filen
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

		if (errorCode != 0){
			fatalError("Decode PNG failed with error: " + std::to_string(errorCode));
		}

		//Generera openGL texture objektet
		glGenTextures(1, &(texture.id));
		
		//Bind texture objektet
		glBindTexture(GL_TEXTURE_2D, texture.id);
		
		//Ladda upp pixlarna i 'out' vektorn till texturen
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		
		//Boilerplate texture parametrar
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		//mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}
}