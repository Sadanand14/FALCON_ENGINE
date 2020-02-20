#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <string>
#include <vector>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include <glm/glm.hpp>

#include "System/Types.h"
#include "Style.h"

/**
 * Base class for anything drawn on a canvas
 */
class CanvasItem
{
	protected:
		struct nk_rect m_bounds;
		nk_color m_color;
		i32 m_flags;
		std::vector<CanvasItem*> m_children;
		CanvasItem* parent = nullptr;

	public:
		CanvasItem();
		virtual ~CanvasItem();
		void AddChild(CanvasItem* child);
		void Begin(nk_context* ctx);
		void End(nk_context* ctx);
		virtual void Draw(nk_context* ctx);
		virtual void Commands(nk_context* ctx) = 0;
		inline void SetColor(nk_color color) { m_color = color; }
		inline void SetFlags(int32_t flags) { m_flags = flags; }
		void SetBounds(struct nk_rect bounds);
		void UpdateBounds();
};

#endif