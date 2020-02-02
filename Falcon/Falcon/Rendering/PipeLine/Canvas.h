#ifndef CANVAS_H
#define CANVAS_H

#include "Renderable.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

class Texture;

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
		nk_buffer m_cmds;
		nk_command_buffer* m_painter;
		struct nk_vec2 m_itemSpacing;
		struct nk_vec2 m_panelPadding;
		nk_style_item m_windowBackground;
		Texture m_fontTex;
		nk_font_atlas m_atlas;
		nk_font* m_font;
		const void* m_image;
		u32 m_maxVerts = 512 * 1024;
		u32 m_maxInds = 128 * 1024;

		//boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexArray;
		//boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexOffsets;

	public:
		boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexArray;
		boost::container::vector<u32, fmemory::STLAllocator<u32>> m_indexOffsets;

		Canvas();
		virtual ~Canvas();

		//Functions
		void Setup() override;
		void Bind() override;
		inline u32 GetMaxVertexSize() { return m_maxVerts; }
		inline u32 GetMaxIndexSize() { return m_maxInds; }
		inline nk_buffer* GetCommandBuffer() { return &m_cmds; }
		inline const nk_font* GetFont() { return m_font; }
		inline const nk_draw_null_texture GetNull() { return m_null; }
		inline nk_command_buffer* GetPainter() { return m_painter; }
		inline void SetPainter(nk_command_buffer* painter) { m_painter = painter; }
		inline struct nk_vec2 GetItemSpacing() { return m_itemSpacing; }
		inline void SetItemSpacing(struct nk_vec2 itemSpacing) { m_itemSpacing = itemSpacing; }
		inline struct nk_vec2 GetPanelPadding() { return m_panelPadding; }
		inline void SetPanelPadding(struct nk_vec2 panelPadding) { m_panelPadding = panelPadding; }
		inline nk_style_item GetWindowBackground() { return m_windowBackground; }
		inline void SetWindowBackground(nk_style_item windowBackground) { m_windowBackground = windowBackground; }
};

#endif