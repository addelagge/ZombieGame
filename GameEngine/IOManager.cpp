#include "IOManager.h"
#include <fstream>

namespace GameEngine{

	//L�ser inneh�llet av en fil, bildfil i detta fall, till en buffer
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer)
	{
		std::ifstream file;
		file.open(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//s�k till slutet av filen
		file.seekg(0, std::ios::end);
		//Get file size
		int fileSize = file.tellg();
		//Tillbaka till b�rjan av filen
		file.seekg(0, std::ios::beg);

		//fileSize -= file.tellg();

		buffer.resize(fileSize);
		//L�ser filen och lagrar data i buffer
		file.read((char *)&buffer[0], fileSize);
		file.close();
		return true;

	}
}