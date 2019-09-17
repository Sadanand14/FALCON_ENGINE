#include "ComponentFactory.h"
ComponentLink ComponentFactory::m_links = ComponentFactory::InitLinks();


ComponentLink ComponentFactory::InitLinks()
{
	ComponentLink temp;
	temp[Rendering] = InvokerRenderer;
	return temp;
}


ComponentBase* ComponentFactory::InvokeComponent(ComponentType type) 
{
	return m_links[type]();
}

ComponentBase* ComponentFactory::InvokerRenderer()
{
	return new RenderComponent();
}



// add the code below as we get individual component classes

//ComponentBase* ComponentFactory::InvokeAnimation() 
//{
//	//return animation component ptr;
//}
//
//ComponentBase* ComponentFactory::InvokeAudio() 
//{
//	//return audio component ptr;
//}
//
//ComponentBase* ComponentFactory::InvokeInput() 
//{
//	//return input component ptr;
//}
//
//ComponentBase* ComponentFactory::InvokePhysics() 
//{
//	//return Physics component ptr;
//}
//
//ComponentBase* ComponentFactory::InvokeAI() 
//{
//	//return AI component ptr;
//}