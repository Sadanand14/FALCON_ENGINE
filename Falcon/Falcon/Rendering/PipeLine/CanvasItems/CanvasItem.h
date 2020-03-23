#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <string>

#include <glm/glm.hpp>

#include "System/Types.h"

#include "Canvas.h"
#include "WindowData.h"
#include "Font.h"

enum class ItemStatus {Active , Inactive, Unused};

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
	ItemStatus m_status;
	std::string m_title;

public:
	CanvasItem();
	virtual ~CanvasItem();
	void AddChild(CanvasItem* child);

	void SetBounds(glm::vec4 bounds);
	void UpdateBounds();

	virtual void Draw(nk_context* ctx);
	virtual void Commands(nk_context* ctx) = 0;

	inline void SetColor(glm::vec4 color) { m_color = nk_rgba(color.x,color.y,color.z,color.w); }
	inline void SetFlags(int32_t flags) { m_flags = flags; }
	inline void SetActive(bool active) { m_active = active; }
	inline void SetStatus(ItemStatus status) { m_status = status; }
};
#endif