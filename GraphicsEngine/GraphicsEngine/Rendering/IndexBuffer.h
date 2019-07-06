#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "framework.h"
class IndexBuffer
{
private:
	unsigned int m_renderBufferId;
	unsigned int m_count;
public:

	IndexBuffer(const unsigned int*, unsigned int);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1
