#ifndef CANVAS_H
#define CANVAS_H

#include "RenderPasses/Renderable.h"

struct Texture;
class CanvasItem;

struct UIVertex
{
	glm::vec2 position;
	glm::vec2 uv;
	nk_byte color[4];
};

/**
 * Canvas for rendering a 2D UI
 */
class Canvas : public Renderable
{
private:
	VertexBuffer* m_VBO;
	IndexBuffer* m_IBO;
	nk_draw_null_texture m_null;
	nk_buffer m_cmds;
	u32 m_maxVerts = 512 * 1024;
	u32 m_maxInds = 128 * 1024;

	boost::container::vector<CanvasItem*, fmemory::STLAllocator<CanvasItem*>> m_canvasItems;

public:
	boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexArray;
	boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexOffsets;

	Canvas();
	virtual ~Canvas();

	//Functions
	void Setup() override;
	void Bind() override;
	void CallDrawCommands(nk_context* ctx);
	inline u32 GetMaxVertexSize() { return m_maxVerts; }
	inline u32 GetMaxIndexSize() { return m_maxInds; }
	inline nk_buffer* GetCommandBuffer() { return &m_cmds; }
	inline const nk_draw_null_texture GetNull() { return m_null; }
	inline void AddCanvasItem(CanvasItem* canvasItem) { m_canvasItems.push_back(canvasItem); }
	inline void ClearCanvas() { m_canvasItems.clear(); }
};

#endif