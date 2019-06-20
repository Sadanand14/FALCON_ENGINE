#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

Shader::Shader(const std::string& path)
{
	m_Shader = LoadShader(path);
	CompileShader(m_Shader.type, m_Shader.source);
}

Shader::~Shader()
{
	glDeleteShader(m_compiledID);
}

SimpleShader Shader::LoadShader(const std::string& path)
{
	std::ifstream input(path);
	std::string line;

	if (getline(input, line))
	{
		std::cout << "FILE FOUND" << std::endl;
	}
	std::stringstream ss;
	unsigned int shaderType = NULL;


	if (line.find("#SHADER") != std::string::npos)
	{
		if (line.find("VERTEX") != std::string::npos)
		{
			shaderType = GL_VERTEX_SHADER;
		}
		else if (line.find("FRAGMENT") != std::string::npos)
		{
			shaderType = GL_FRAGMENT_SHADER;
		}
		else
		{
			assert(false);
		}
		while (getline(input, line))
		{
			ss << line << "\n";
		}

		std::cout << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;

	}
	else
	{
		assert(false);
	}

	std::cout << ss.str() << std::endl;

	return { shaderType, ss.str().c_str() };
}



void Shader::CompileShader(unsigned int type, std::string source)
{
	m_compiledID = glCreateShader(type);

	const char* src = source.c_str();

	glShaderSource(m_compiledID, 1, &src, NULL);
	glCompileShader(m_compiledID);

	int result;
	glGetShaderiv(m_compiledID, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(m_compiledID, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(m_compiledID, length, &length, message);

		std::cout << "Could'nt Compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "Shader! \n";
		std::cout << message << std::endl;

		assert(false);
	}
}
