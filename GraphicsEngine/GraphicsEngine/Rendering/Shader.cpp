#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "Log.h";

Shader::Shader(const std::string& path)
{
	m_Shader = LoadShader(path); // calls a function that processes the file path provided and returns a structure
	CompileShader(m_Shader.type, m_Shader.source); //passes the structure into the function to generate an ID.
}

Shader::~Shader()
{
	glDeleteShader(m_compiledID);// deletes the shader with that ID
}

//this function reads from a file a converts the shader code into a string variable
SimpleShader Shader::LoadShader(const std::string& path)
{
	std::ifstream input(path); 
	std::string line;

	if (getline(input, line))// reads first line
	{
		FL_ENGINE_INFO("INFO: File found");
	}
	std::stringstream ss;
	unsigned int shaderType = NULL;


	if (line.find("#SHADER") != std::string::npos) // if the first line has shader declaration
	{
		if (line.find("VERTEX") != std::string::npos)// if its declared as a vertex shader
		{
			shaderType = GL_VERTEX_SHADER;   // set type to vertex
		}
		else if (line.find("FRAGMENT") != std::string::npos)// if its declared as a fragment shader
		{
			shaderType = GL_FRAGMENT_SHADER; // set type to fragment
		}
		else
		{
			assert(false);					// stops execution if it is declared as neither vertex or fragment
		}
		while (getline(input, line))
		{
			ss << line << "\n";				// reads the rest of the lines in the shader
		}

		//if (shaderType == GL_VERTEX_SHADER)
			FL_ENGINE_INFO("INFO: Loading {0} shader.", (shaderType == GL_VERTEX_SHADER)?"vertex":"fragment");
		//else
			//FL_ENGINE_INFO("INFO: Loading Vertex shader.");

	}
	else
	{
		assert(false);					 //  stops execution if it isnt declared as a shader
	}

	//std::cout << ss.str() << std::endl;

	return { shaderType, ss.str()};		//returns a structure containing the type and the source code for the shader
}


//compiles the shader code recieved from load shader function to store an id for the compiled source code.
void Shader::CompileShader(unsigned int type, std::string source)
{
	m_compiledID = glCreateShader(type); // generates a shader buffer 

	const char* src = source.c_str();	// converts to const char*

	glShaderSource(m_compiledID, 1, &src, NULL); // binds the source code to that shader id.
	glCompileShader(m_compiledID);		 // compiles the shader with that ID.

	int result;
	glGetShaderiv(m_compiledID, GL_COMPILE_STATUS, &result); //  stores the result of compilation in the result variable as int
	if (!result)// if its 0 then compilation failed
	{
		int length;
		glGetShaderiv(m_compiledID, GL_INFO_LOG_LENGTH, &length);// stores the lenght of the info log in the length variable
		char* message = new char[length]; // generates a char array of that length
		glGetShaderInfoLog(m_compiledID, length, &length, message); // paasses in the error log into the created char array


		
		FL_ENGINE_ERROR("ERROR: Failed to compile {0} , {1}", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message);
		
		/*std::
		
		<< "Could'nt Compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "Shader! \n";
		std::cout << message << std::endl;*/

		assert(false);				// stops execution if the shader code didnt compile.
	}
}
