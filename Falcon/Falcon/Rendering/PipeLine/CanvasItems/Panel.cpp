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
 *
 * @param name - The name for the nuklear window
 */
Panel::Panel(const char* name) : CanvasItem(name)
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
	nk_layout_row_dynamic(ctx, m_bounds.h, 1);
	nk_widget(&m_bounds, ctx);
}