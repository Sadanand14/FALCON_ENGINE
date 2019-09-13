#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H

#include <glm/glm.hpp>

struct Vertex {
	//Position 
	glm::vec3 Position;
	//Normal
	glm::vec3 Normal;
	//Texture Coordinates
	glm::vec2 TexCoords;
	//Tangent
	glm::vec3 Tangent;
	//Bitangent
	glm::vec3 Bitangent;
};

#endif // !VERTEXLAYOUT_H
