#include "GLSLprogram.h"
#include "GameEngineErrors.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace GameEngine{
	//The : _numAttributes(0) ect. is an initialization list. It is a better way to initialize variables, since it avoids an extra copy. 
	GLSLprogram::GLSLprogram() :
		_programID(0),
		_vertexShaderID(0),
		_fragmentShaderID(0)
	{

	}


	GLSLprogram::~GLSLprogram()
	{
	}

	//skapar ett gl_programobjekt, ett par shader objekt och anropar compileShader
	void GLSLprogram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		_programID = glCreateProgram();

		//Skapar ett vertex shader object och lagrar dess Id
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0){
			fatalError("Vertex shader create error");
		}

		//Skapar ett fragment shader object och lagrar dess Id
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0){
			fatalError("Fragment shader create error");
		}

		//Kompilera vertex och fragment shaders
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}

	//kopplar ihop shaderobjekten som skapades ovan med vertex/fragmentshader .txt-filerna i shadersmappen
	void GLSLprogram::compileShader(const std::string& filePath, GLuint id)
	{
		std::ifstream shaderFile;
		shaderFile.open(filePath);
		if (shaderFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";		
		std::string line;
		
		//Läs alla rader i filen och lagra dom i fileContents
		while (getline(shaderFile, line)){
			fileContents += line + "\n";
		}

		shaderFile.close();

		//Pekare till fileContents, behövs för glShaderSource
		const char* contentsPtr = fileContents.c_str();
		//kopplar ihop shadern med innehållet i fileContents
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);
		GLint success = 0;
		
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		//Kontrollerar status på kompileringen av shadern.
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id); 
			printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
		}
	}

	//Attachar alla shaders till gl_programmet, och länkar till slut programmet
	void GLSLprogram::linkShaders()
	{
		//Attatchar alla shaders till gl_programmet
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Länkar programmet
		glLinkProgram(_programID);

		//Kontrollerar status på länkningen
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
			glDeleteProgram(_programID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to link");
		}

		//Shaders ska alltid detach'as efter en lyckad länkning
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

	}

	//associerar vertex attribute nr (_numerattributes) med namnet (attribname). Där jag anropar funktionen kopplas '0' ihop med 'position' (i vertex shader)
	void GLSLprogram::addAttribute(int index, const std::string& attribName)
	{
		glBindAttribLocation(_programID, index, attribName.c_str());
	}

	//aktivera shadern, och alla dess attribut
	void GLSLprogram::use()
	{
		glUseProgram(_programID);
	}

	//deaktivera shadern
	void GLSLprogram::unuse()
	{
		glUseProgram(0);
	}

	GLint GLSLprogram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX){
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	GLuint GLSLprogram::getAttibLocation(const std::string& uniformName)
	{
		GLuint location = glGetAttribLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX){
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}
}


