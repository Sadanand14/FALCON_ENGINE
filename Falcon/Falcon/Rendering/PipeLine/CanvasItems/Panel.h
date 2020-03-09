#ifndef PANEL_H
#define PANEL_H

#include <string>

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