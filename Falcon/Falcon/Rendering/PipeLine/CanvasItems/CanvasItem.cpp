#include "CanvasItem.h"

/**
 * Base CanvasItem constructor
 */
CanvasItem::CanvasItem() : m_bounds(nk_rect(0, 0, 100, 100)), m_color(nk_rgb(255, 255, 255)),
										   m_flags(NK_WINDOW_NO_SCROLLBAR)
{

}

void CanvasItem::AddChild(CanvasItem* child)
{
	m_children.push_back(child);
	child->parent = this;
	child->UpdateBounds();
}

/**
 * Calls the nuklear begin command
 * @param ctx - The nuklear context to draw to
 */
void CanvasItem::Begin(nk_context* ctx)
{
	//ctx->style.window.background = m_color;
	//ctx->style.window.fixed_background = nk_style_item_color(m_color);
	nk_layout_space_push(ctx, m_bounds);
}

/**
 * Calls the nuklear end command
 * @param ctx - The nuklear context to draw to
 */
void CanvasItem::End(nk_context* ctx)
{

}

/**
 * Begins a nuklear context, calls the draw commands, then ends drawing
 */
void CanvasItem::Draw(nk_context* ctx)
{
	Begin(ctx);
	Commands(ctx);

	for(auto child : m_children)
	{
		child->Draw(ctx);
	}
	End(ctx);
}

/**
 * Sets the bounds of the canvas
 */
void CanvasItem::SetBounds(struct nk_rect bounds)
{
	m_bounds = bounds;
	UpdateBounds();
}

/**
 * Updates the bounds of a mesh
 */
void CanvasItem::UpdateBounds()
{
	if(parent != nullptr)
	{
		m_bounds = nk_rect(parent->m_bounds.x + parent->m_bounds.x * m_bounds.x,
						   parent->m_bounds.y + parent->m_bounds.y * m_bounds.y,
						   parent->m_bounds.w * m_bounds.w,
						   parent->m_bounds.h * m_bounds.h);
	}

	for(auto child : m_children)
	{
		child->UpdateBounds();
	}
}

/**
 * Canvas item destructor
 */
CanvasItem::~CanvasItem()
{

}