#include "Panel.h"

#include "Font.h"
#include "AssetManager.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

/**
 * Constructor for a panel
 */
Panel::Panel() : CanvasItem()
{

}

/**
 * Panel destructor
 */
Panel::~Panel()
{

}

/** The draw commands for the panel
 *
 * @param ctx - The nuklear context to draw to
 */
void Panel::Commands(nk_context* ctx)
{
	struct nk_rect bounds = nk_layout_space_bounds(ctx);
	bounds = nk_rect(bounds.x * m_bounds.x, bounds.y * m_bounds.y, bounds.w * m_bounds.w, bounds.h * m_bounds.h);

	nk_fill_rect(nk_window_get_canvas(ctx), bounds, 5, nk_rgb(50, 50, 50));
}
