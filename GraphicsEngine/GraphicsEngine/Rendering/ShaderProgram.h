#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "framework.h"

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

	inline const unsigned int GetId() const { return m_programID; }
};

#endif // !1

