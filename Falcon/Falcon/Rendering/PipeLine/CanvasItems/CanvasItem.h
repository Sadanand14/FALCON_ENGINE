#ifndef CANVASITEM_H
#define CANVASITEM_H

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
		glm::uvec2 m_position;
		nk_color m_color;

	public:
		CanvasItem();
		virtual ~CanvasItem();
		virtual void Draw(nk_context* ctx) = 0;
		inline void SetPosition(glm::uvec2 position) { m_position = position; }
		inline void SetColor(nk_color color) { m_color = color; }
};

#endif