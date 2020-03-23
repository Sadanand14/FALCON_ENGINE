#ifndef PARTICLERENDERPASS_H
#define PARTICLERENDERPASS_H

#include "RenderPass.h"
#include "Particle.h"

class ParticleRenderPass : public RenderPass
{
private:

public:
	ParticleRenderPass(uint32_t priority);
	void Render() override;
};

#endif