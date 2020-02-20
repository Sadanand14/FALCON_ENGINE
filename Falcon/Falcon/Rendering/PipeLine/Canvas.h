#ifndef CANVAS_H
#define CANVAS_H

#include <RenderPasses/Renderable.h>
#include "Style.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

class Texture;
class CanvasItem;

struct UIVertex
{
	glm::vec2 position;
	glm::vec2 uv;
	u08 color;
};

class Canvas : public Renderable
{
	private:
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		nk_draw_null_texture m_null;
		Style style;
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
		inline struct nk_vec2 GetItemSpacing() { return style.m_itemSpacing; }
		inline void SetItemSpacing(struct nk_vec2 itemSpacing) { style.m_itemSpacing = itemSpacing; }
		inline struct nk_vec2 GetPanelPadding() { return style.m_panelPadding; }
		inline void SetPanelPadding(struct nk_vec2 panelPadding) { style.m_panelPadding = panelPadding; }
		inline nk_style_item GetWindowBackground() { return style.m_windowBackground; }
		inline void SetWindowBackground(nk_style_item windowBackground) { style.m_windowBackground = windowBackground; }
		inline void AddCanvasItem(CanvasItem* canvasItem) { m_canvasItems.push_back(canvasItem); }
};

#endif