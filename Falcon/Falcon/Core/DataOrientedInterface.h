#ifndef DATA_ORIENTED_INTERFACE_H
#define DATA_ORIENTED_INTERFACE_H

#include <Types.h>
#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>
#include  <vector>

class EntityArr 
{
private:
	unsigned int m_lastIndex;
	std::vector<Transform> m_tranformArr;
	std::vector<RenderComponent> m_renderArr;

public:

	EntityArr(unsigned int value): m_lastIndex(0) { m_tranformArr.reserve(value); m_renderArr.reserve(value);}

	void AddEntity(Transform* transform, RenderComponent* renderCom) 
	{
		if (transform == nullptr) 
		{
			m_tranformArr.push_back(Transform());
		}
		else 
		{
			m_tranformArr.push_back(*transform);
		}

		if (transform == nullptr) 
		{
			m_renderArr.push_back(RenderComponent());
		}
		else 
		{
			m_renderArr.push_back(*renderCom);
		}
	}

};

#endif 
