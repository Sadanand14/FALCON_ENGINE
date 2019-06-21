#include "ShaderProgram.h"

// takes in two shader id's and creates a program to bind those to it. Make sure to pass in one fragment 
//shader id and one vertex shader ID.
ShaderProgram::ShaderProgram(const unsigned int& shaderID1, const unsigned int& shaderID2)
{
	m_programID = glCreateProgram();// creates a program to use the shaders

	glAttachShader(m_programID, shaderID1);// binds the first shader to that program.
	glAttachShader(m_programID, shaderID2);// binds the second shader to that program.

	glLinkProgram(m_programID);		// links the program, check docs.gl for more info
	glValidateProgram(m_programID); // validates if its usables, check docs.gl for more info
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programID);	// deletes the program
}

void ShaderProgram::Use() const
{
	glUseProgram(m_programID);		// use the program for the current context
}

void ShaderProgram::Disuse() const
{
	glUseProgram(0);				// stop using this program for the current context
}