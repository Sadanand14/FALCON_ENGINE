#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "RenderComponent.h"

#pragma warning ( push )
#pragma warning (disable: 26451 26439 6285)
#include <boost/container/map.hpp>
#pragma warning ( pop )

class ComponentFactory;


typedef boost::container::map<ComponentType, ComponentBase*(*)()> ComponentLink;

class ComponentFactory
{
public :
	static ComponentLink m_links;

public:
	static ComponentLink InitLinks();

	static ComponentBase* InvokeComponent(ComponentType);

	static ComponentBase* InvokerRenderer();

	/*static ComponentBase* InvokeAudio();
	static ComponentBase* InvokeAnimation();
	static ComponentBase* InvokeInput();
	static ComponentBase* InvokePhysics();
	static ComponentBase* InvokeAI();*/
};

#endif // !1