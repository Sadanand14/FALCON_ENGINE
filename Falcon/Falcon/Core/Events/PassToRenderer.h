#ifndef PASS_TO_RENDERER_H
#define PASS_TO_RENDERER_H

#include <Events/Event.h>
#include <PipeLine/Mesh.h>

struct PassToRenderer : public Event 
{
private:
	Mesh* m_skyMesh, *m_terrainMesh;
public: 
	PassToRenderer(Mesh* skyMesh, Mesh* terrainMesh): m_skyMesh(skyMesh), m_terrainMesh(terrainMesh), Event(EVENT_DATA_TO_RENDERER){}
	inline Mesh* GetSkyMesh() { return m_skyMesh;}
	inline Mesh* GetTerrainMesh() { return m_terrainMesh;}
};

#endif // !1
