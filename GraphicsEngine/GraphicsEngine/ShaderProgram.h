#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>

class ShaderProgram
{
private:
	unsigned int m_programID;

public:

	ShaderProgram(const unsigned int&, const unsigned int&);
	~ShaderProgram();

	void Use() const;
	void Disuse() const;

	inline const unsigned int GetId() const { return m_programID; }
};

#endif // !1

