#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <glad/glad.h>

#include <vector>
#include <cassert>


//a structure that stores the properties of individual attributes of a vertexbuffer
struct VertexElements
{
	unsigned int type;
	unsigned int count;
	bool normalized;

	//helper method to deal with different data types
	static unsigned int GetSizeofType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			 return 4;
		case GL_UNSIGNED_INT:	 return 4;
		case GL_UNSIGNED_BYTE:	 return 1;
		}
		assert(false);
		return 0;
	}
};

//class that stores all the attributes attributed to a single vertex buffer
class VertexLayout
{
private:
	std::vector<VertexElements> m_elements;//stores a list of attributes
	unsigned int m_stride;					//stores the stride of all those attributes

public:

	VertexLayout() :m_stride(0) {};
	~VertexLayout() {};

	inline const std::vector<VertexElements>& GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }

	// templated function to add attributes to
	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, false }); // adds the attributes with its count to the function
		m_stride += VertexElements::GetSizeofType(GL_FLOAT) * count;// sets the stride to add the above addition
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, false }); // adds the attributes with its count to the function
		m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_INT) * count;// sets the stride to add the above addition
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, false });// adds the attributes with its count to the function
		m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_BYTE) * count;// sets the stride to add the above addition
	}

};

#endif // !1

