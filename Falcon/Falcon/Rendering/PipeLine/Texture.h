#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

/**
*Texture Data Structure
*/
struct Texture {
	unsigned int textureID = 0;
	std::string type;
	std::string path;

};
#endif // TEXTURE_H
