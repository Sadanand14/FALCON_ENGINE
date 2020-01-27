#ifndef Asset_Manager_H
#define Asset_Manager_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <PipeLine/Material.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

//Include the pretty writer in debug mode (easier to read)
#if defined BUILD_DEBUG_MODE
#include <rapidjson/prettywriter.h>
#define Writer PrettyWriter
#else
#include <rapidjson/writer.h>
#endif

#include <Memory/fmemory.h>

class Mesh;
struct Vertex;
/**
* A class for defining procedures of extraction of asset data from external files and storing them in the engine.
* Most Methods are primarily static and this class stores no data.
*/
class AssetManager
{
private:
	static boost::unordered_map<std::string, Mesh*> m_meshes;
	static boost::unordered_map<std::string, Material*> m_materials;

	// Process nodes
	static void ProcessNode(aiNode* node, const aiScene* scene, boost::container::vector<Vertex> &verts, boost::container::vector<uint32_t> &inds, boost::container::vector<uint32_t> &indOffsets);
	static void ProcessMesh(aiMesh* mesh, boost::container::vector<Vertex> &verts, boost::container::vector<uint32_t> &inds, boost::container::vector<uint32_t> &indOffsets);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.

public:
	static Material* GetMaterial(const std::string& path);
	static Mesh* GetMesh(const std::string& path);
	static Mesh* LoadTerrain(const std::string& path);
	static Mesh* LoadModel(std::string const& path);
	static u32 LoadTexture(std::string const& path);
	static Material* LoadMaterial(std::string const& path);
	static void Clean();
};


#endif