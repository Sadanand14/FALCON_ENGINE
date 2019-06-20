#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const unsigned int& shaderID1, const unsigned int& shaderID2)
{
	m_programID = glCreateProgram();

	glAttachShader(m_programID, shaderID1);
	glAttachShader(m_programID, shaderID2);

	glLinkProgram(m_programID);
	glValidateProgram(m_programID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programID);
}

void ShaderProgram::Use() const
{
	glUseProgram(m_programID);
}

void ShaderProgram::Disuse() const
{
	glUseProgram(0);
}