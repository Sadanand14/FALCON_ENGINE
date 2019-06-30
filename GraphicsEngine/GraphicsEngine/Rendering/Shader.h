#ifndef SHADER_H
#define	SHADER_H

#include "framework.h"


#include <boost/container/string.hpp>
#include <string>
typedef boost::container::string boostString;


struct SimpleShader
{
	unsigned int type;
	std::string source;

	//SimpleShader():type(0),source("") {};
};

class Shader
{
private:
	unsigned int m_compiledID; // id for this shader
	SimpleShader m_Shader; // structure storing the code and type of shader
public:
	Shader(const std::string& path);
	~Shader();

	void SetUniform4f(const boostString&, float, float, float, float);

	inline const unsigned int GetID() const { return m_compiledID;} 

private:
	SimpleShader LoadShader(const std::string& name);
	void CompileShader(unsigned int, std::string);
	int GetUniformLocation(const boostString& name);
};

#endif // !1

