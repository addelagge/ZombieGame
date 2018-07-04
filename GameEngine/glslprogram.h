#pragma once
#include <string>
#include <GL\glew.h>

namespace GameEngine{
	//This class handles the compilation, linking, and usage of a GLSL shader program.
	//Reference: http://www.opengl.org/wiki/Shader_Compilation
	class GLSLprogram
	{
	public:
		GLSLprogram();
		~GLSLprogram();
		//vertexShaderFilePath �r "by reference" f�r att man inte ska beh�va g�ra en kopia av filen varje g�ng man ska kompilera shaders
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void addAttribute(int index, const std::string& attribName);
		void linkShaders();
		void use();
		void unuse();
		GLint getUniformLocation(const std::string& uniformName);
		GLuint GLSLprogram::getAttibLocation(const std::string& uniformName);


	private:
		void compileShader(const std::string& filePath, GLuint id);
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};

}