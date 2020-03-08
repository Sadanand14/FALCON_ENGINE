#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <string>
#include <boost/container/vector.hpp>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include <glm/glm.hpp>

#include "System/Types.h"

/**
 * Base class for anything drawn on a canvas
 */
class CanvasItem
{
	protected:
		struct nk_rect m_bounds;
		nk_color m_color;
		i32 m_flags;
		boost::container::vector<CanvasItem*> m_children;
		CanvasItem* parent = nullptr;
		bool m_active = true;

	public:
		CanvasItem();
		virtual ~CanvasItem();
		void AddChild(CanvasItem* child);
		virtual void Draw(nk_context* ctx);
		virtual void Commands(nk_context* ctx) = 0;
		inline void SetColor(nk_color color) { m_color = color; }
		inline void SetFlags(int32_t flags) { m_flags = flags; }
		inline void SetActive(bool active) { m_active = active; }
		void SetBounds(struct nk_rect bounds);
		void UpdateBounds();
};

#endif