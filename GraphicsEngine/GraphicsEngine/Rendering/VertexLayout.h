#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include "framework.h"

#include <boost/container/vector.hpp>
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

typedef boost::container::vector<VertexElements> boost_elements_vector;

//class that stores all the attributes attributed to a single vertex buffer
class VertexLayout
{
private:
	boost_elements_vector m_elements;//stores a list of attributes
	unsigned int m_stride;					//stores the stride of all those attributes

public:

	VertexLayout() :m_stride(0), m_elements({}) {};
	~VertexLayout() {};

	inline const boost_elements_vector GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }

	// templated function to add attributes to
	template <typename T>
	void Push(unsigned int count)
	{
		//static_assert(false, "Assertion failed");
	}

};
template<>
inline void VertexLayout::Push<float>(unsigned int count)
{
	m_elements.push_back({ GL_FLOAT, count, false }); // adds the attributes with its count to the function
	m_stride += VertexElements::GetSizeofType(GL_FLOAT) * count;// sets the stride to add the above addition
}

template<>
inline void VertexLayout::Push<unsigned int>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_INT, count, false }); // adds the attributes with its count to the function
	m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_INT) * count;// sets the stride to add the above addition
}

template<>
inline void VertexLayout::Push<unsigned char>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_BYTE, count, false });// adds the attributes with its count to the function
	m_stride += VertexElements::GetSizeofType(GL_UNSIGNED_BYTE) * count;// sets the stride to add the above addition
}



#endif // !1

