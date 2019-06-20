#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <glad/glad.h>

#include <vector>
#include <cassert>

struct VertexElements
{
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeofType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			 return 4;
		case GL_UNSIGNED_INT:	 return 4;
		case GL_UNSIGNED_BYTE:			 return 1;
		}
		assert(false);
		return 0;
	}
};

class VertexLayout
{
private:
	std::vector<VertexElements> m_elements;
	unsigned int m_stride;

public:

	VertexLayout() :m_stride(0) {};
	~VertexLayout() {};

	inline const std::vector<VertexElements>& GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }


	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, false }); \
			m_stride += VertexElements::GetSizeofType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, false });
		m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_BYTE) * count;
	}

};

#endif // !1

