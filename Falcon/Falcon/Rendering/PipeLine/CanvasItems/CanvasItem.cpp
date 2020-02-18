#include "CanvasItem.h"

/**
 * Base CanvasItem constructor
 */
CanvasItem::CanvasItem(const char* name) : m_bounds(nk_rect(0, 0, 100, 100)), m_color(nk_rgb(255, 255, 255)),
										   m_flags(NK_WINDOW_NO_SCROLLBAR), m_name(name)
{

}

void CanvasItem::AddChild(CanvasItem* child)
{
	m_children.push_back(child);
}

/**
 * Calls the nuklear begin command
 * @param ctx - The nuklear context to draw to
 */
void CanvasItem::Begin(nk_context* ctx)
{
	ctx->style.window.background = m_color;
	ctx->style.window.fixed_background = nk_style_item_color(m_color);

	nk_window_set_bounds(ctx, m_name.c_str(), m_bounds);
	nk_begin(ctx, m_name.c_str(), m_bounds, m_flags);
}

/**
 * Calls the nuklear end command
 * @param ctx - The nuklear context to draw to
 */
void CanvasItem::End(nk_context* ctx)
{
	nk_end(ctx);
}

/**
 * Begins a nuklear context, calls the draw commands, then ends drawing
 */
void CanvasItem::Draw(nk_context* ctx)
{
	Begin(ctx);
	Commands(ctx);

	for(auto child : m_children)
		child->Commands(ctx);

	End(ctx);
}

/**
 * Canvas item destructor
 */
CanvasItem::~CanvasItem()
{

}