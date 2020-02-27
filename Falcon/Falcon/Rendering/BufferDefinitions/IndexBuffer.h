#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "framework.h"
#include <Types.h>
#include <vector>
#include <Memory/fmemory.h>

/**
* Index Buffer CLass to initialize an index buffer and store data on using OpenGL commands.
*/
class IndexBuffer
{
private:
	unsigned int m_renderBufferId;
	size_t m_count;
	void SetupIndexBuffer(const unsigned int* indices, u32 drawType);

public:

	IndexBuffer(const unsigned int*, size_t, u32 drawType);
	IndexBuffer(const unsigned int*, size_t);
	IndexBuffer(const std::vector<unsigned int,fmemory::StackSTLAllocator<unsigned int>>& indices, size_t count, u32 drawType);
	void BufferData(const void* data, size_t size, u32 drawType);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif // !1
