#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//Get vertex/fragment code from the file path
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//Read file's buffer content into the streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Close the files
		vShaderFile.close();
		fShaderFile.close();
		//Convert stream to string
			vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	//Build and Compile Shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//Vertex Shader 
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//Print any compilation errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Error::Shader::Vertex::Compilation_Failed\n" << infoLog << std::endl;
	};

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//Check for shader compile errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Error::Shader::Fragment1::Compilation_Failed\n" << infoLog << std::endl;
	}

	//Link Shaders
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertex);
	glAttachShader(m_programID, fragment);
	glLinkProgram(m_programID);
	glValidateProgram(m_programID);
	//Print any linking errors
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the Shaders after they're linked to the program
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::UseShader()
{
	glUseProgram(m_programID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->m_programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->m_programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->m_programID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->m_programID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->m_programID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->m_programID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->m_programID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->m_programID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->m_programID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLint Shader::GetUniform(const GLchar* name)
{
	return glGetUniformLocation(this->m_programID, name);
}
