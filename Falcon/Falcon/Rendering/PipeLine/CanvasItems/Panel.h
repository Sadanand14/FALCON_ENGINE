#ifndef PANEL_H
#define PANEL_H

#include <string>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include "CanvasItem.h"
#include "System/Types.h"

/**
 * A basic Panel
 */
class Panel : public CanvasItem
{
	protected:

	public:
		Panel();
		virtual ~Panel();
		void Commands(nk_context* ctx) override;
};

#endif