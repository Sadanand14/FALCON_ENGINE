#include "RenderComponent.h"


RenderComponent::RenderComponent()
{
	model = new Model();
}

RenderComponent::~RenderComponent() 
{
	delete model;
}

void RenderComponent::V_Update()
{
	//glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
}