#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <string>

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

class CanvasItem
{
	protected:
		struct nk_rect m_bounds;
		nk_color m_color;
		i32 m_flags;
		std::string m_name;

	public:
		CanvasItem(const char* name);
		virtual ~CanvasItem();
		void Begin(nk_context* ctx);
		void End(nk_context* ctx);
		virtual void Draw(nk_context* ctx);
		virtual void Commands(nk_context* ctx) = 0;
		inline void SetBounds(struct nk_rect bounds) { m_bounds = bounds; }
		inline void SetColor(nk_color color) { m_color = color; }
		inline void SetFlags(int32_t flags) { m_flags = flags; }
};

#endif