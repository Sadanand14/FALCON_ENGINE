#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

enum class TextureType { CUBEMAP, TEXTURE2D };

/**
*Texture Data Structure
*/
struct Texture {
	unsigned int textureID = 0;
	//std::string type;
	//std::string path;
	TextureType type;
};
#endif // TEXTURE_H
