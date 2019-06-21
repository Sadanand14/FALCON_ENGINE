#ifndef SHADER_H
#define	SHADER_H

#include "framework.h"

#include <string>

struct SimpleShader
{
	unsigned int type;
	std::string source;
};

class Shader
{
private:
	unsigned int m_compiledID; // id for this shader
	SimpleShader m_Shader; // structure storing the code and type of shader
public:
	Shader(const std::string& path);
	~Shader();

	inline const unsigned int GetID() const { return m_compiledID;} 

private:
	SimpleShader LoadShader(const std::string& name);
	void CompileShader(unsigned int, std::string);
};

#endif // !1

