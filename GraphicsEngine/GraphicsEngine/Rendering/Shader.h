#ifndef SHADER_H
#define	SHADER_H

#include "framework.h"

#include <string>


struct SimpleShader
{
	unsigned int m_type;
	std::string m_source;
	SimpleShader() :m_type(0){ return; }
	SimpleShader(unsigned int type, std::string source) : m_type(type), m_source(source) { return; }
};

class Shader
{
private:
	unsigned int m_compiledID; // id for this shader
	SimpleShader m_Shader; // structure storing the code and type of shader
public:
	Shader(const std::string& path);
	~Shader();

	void SetUniform4f(const std::string&, float, float, float, float);
	void SetUniform1f(const std::string&, float);
	void SetUniform1i(const std::string&, int);

	inline const unsigned int GetID() const { return m_compiledID;} 

private:
	SimpleShader LoadShader(const std::string& name);
	void CompileShader(unsigned int, std::string);
	int GetUniformLocation(const std::string& name);
};

#endif // !1

