#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "CanvasItem.h"
#include "System/Types.h"
#include "../Texture.h"

/**
 * A basic label
 */
class Image : public CanvasItem
{
	protected:
		struct nk_image m_img;

	public:
		Image();
		virtual ~Image();
		void Commands(nk_context* ctx) override;
		inline void SetImage(Texture tex) { m_img = nk_image_id(tex.textureID); }
};

#endif