#ifndef MESHRENDERPASS_H
#define MESHRENDERPASS_H

#include "RenderPass.h"
#include "Mesh.h"

class MeshRenderPass : public RenderPass
{
private:

public:
	MeshRenderPass(uint32_t priority);
	void Render() override;
};

#endif