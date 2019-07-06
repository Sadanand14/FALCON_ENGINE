#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "framework.h"
#include <string>

//class that binds compiled shader id's to a program to be used while drawing.
class ShaderProgram
{
private:
	unsigned int m_programID = 0;

public:

	ShaderProgram(const unsigned int&, const unsigned int&);
	~ShaderProgram();

	void Use() const;
	void Disuse() const;

	void SetUniform1i(const std::string&, int);
	void SetUniform1f(const std::string&, float);
	void SetUniform4f(const std::string&, float, float, float, float);

	inline const unsigned int GetId() const { return m_programID; }

private:

	int GetUniformLocation(const std::string& name);
};

#endif // !1

