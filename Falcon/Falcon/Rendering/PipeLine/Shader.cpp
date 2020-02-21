#include "Shader.h"
#include "Log.h"


/**
* Get vertex/fragment code from the file path\
*/
bool Shader::LoadShaderCode(const GLchar* filePath, GLenum& type)
{
	std::string shaderSrc;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//Open the files
		shaderFile.open(filePath);
		//Read file's buffer content into the streams
		std::stringstream shaderSrcStream;
		shaderSrcStream << shaderFile.rdbuf();
		//Close the files
		shaderFile.close();

		switch (type)
		{
		case GL_VERTEX_SHADER:
			m_vertexShader.m_source = shaderSrcStream.str();
		case GL_FRAGMENT_SHADER:
			m_fragmentShader.m_source = shaderSrcStream.str();
		}
	}
	catch (std::exception e)
	{
		FL_ENGINE_ERROR("ERROR::Failed to read shader code.{0}",e.what());
		return false;
	}
	return true;
}





/**
* Compiles the shader code recieved from load shader function to store an id for the compiled source code.
*/
bool Shader::CompileShaderCode(SimpleShader& shader)
{
	
	shader.m_shaderId = glCreateShader(shader.m_type); //generate the shader buffer

	const char* src = shader.m_source.c_str();	// converts to const char*
	glShaderSource(shader.m_shaderId, 1, &src, NULL);//binds source to shader id
	glCompileShader(shader.m_shaderId);//compilder the shader with provided id
	
	//Print any compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader.m_shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader.m_shaderId, 512, NULL, infoLog);
		FL_ENGINE_ERROR("Error::Shader compilation failed:\n {0}", infoLog);
		return false;
	};

	return true;
}


/**
* Links the shaders with current program.
*/

bool Shader::LinkShaders()
{
	//Link Shaders
	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShader.m_shaderId);
	glAttachShader(m_programID, m_fragmentShader.m_shaderId);
	glLinkProgram(m_programID);
	glValidateProgram(m_programID);
	//Print any linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
		FL_ENGINE_ERROR("Error::Shader Linking Failed {0}", infoLog);
		return false;
	}
	return true;
}


/*
* Shader constructor takes in paths to the src files of shaders and takes care of building and linking them with the 
* current opengl context.
* Parameters must be in order of VertexShaderPath, Fragmentshaderpath
*/
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	:m_vertexShader(GL_VERTEX_SHADER),
	m_fragmentShader(GL_FRAGMENT_SHADER)
{
	//Read the shaders 
	GLenum vertexShader = GL_VERTEX_SHADER;
	if (!LoadShaderCode(vertexPath, vertexShader))
		FL_ENGINE_ERROR("Vertex Shader failed to load for path: {0}", vertexPath);

	GLenum fragmentShader = GL_FRAGMENT_SHADER;
	if (!LoadShaderCode(fragmentPath, fragmentShader))
		FL_ENGINE_ERROR("Fragment Shader failed to load for path: {0}", fragmentPath);

	//Compile the shaders
	if (!CompileShaderCode(m_vertexShader))
		FL_ENGINE_ERROR("VertexShader failed to compile for path: {0}", vertexPath);
	
	if(!CompileShaderCode(m_fragmentShader))
		FL_ENGINE_ERROR("Fragment Shader failed to compile for path: {0}", fragmentPath);

	//Link Shaders
	if (!LinkShaders())
		FL_ENGINE_ERROR("Linking failed for \n VS : {0} \n FS:{1}", vertexPath, fragmentPath);

	//Delete the Shaders after they're linked to the program
	glDeleteShader(m_vertexShader.m_shaderId);
	glDeleteShader(m_fragmentShader.m_shaderId);
}



void Shader::UseShader()
{
	glUseProgram(m_programID);
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
	int temp = glGetUniformLocation(m_programID, name.c_str());
	glUniformMatrix4fv(temp, 1, GL_FALSE, &mat[0][0]);
}

GLint Shader::GetUniform(const GLchar* name)
{
	return glGetUniformLocation(this->m_programID, name);
}
