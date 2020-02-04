#include "CanvasItem.h"

CanvasItem::CanvasItem(const char* name) : m_bounds(nk_rect(0, 0, 100, 100)), m_color(nk_rgb(255, 255, 255)),
										   m_flags(0), m_name(name)
{

}

void CanvasItem::Begin(nk_context* ctx)
{
	nk_window_set_bounds(ctx, m_name.c_str(), m_bounds);
	nk_begin(ctx, m_name.c_str(), m_bounds, m_flags);
}

void CanvasItem::End(nk_context* ctx)
{
	nk_end(ctx);
}

void CanvasItem::Draw(nk_context* ctx)
{
	Begin(ctx);
	Commands(ctx);
	End(ctx);
}

CanvasItem::~CanvasItem()
{

}