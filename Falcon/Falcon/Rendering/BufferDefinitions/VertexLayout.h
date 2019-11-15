#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H

#include <glm/glm.hpp>

struct Bone {
	glm::uvec4 ids;
	glm::vec4 weights;
};

/**
* Vertex Data Structure
*/

struct Vertex {
	//Position
	glm::vec3 Position;
	//Texture Coordinates
	glm::vec2 TexCoords;
	//Normal
	glm::vec3 Normal;
	//Tangent
	glm::vec3 Tangent;
	//Bitangent
	glm::vec3 Bitangent;
	//Bone
	Bone bone;
};

#endif // !VERTEXLAYOUT_H
