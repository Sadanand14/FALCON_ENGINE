#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <string>

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