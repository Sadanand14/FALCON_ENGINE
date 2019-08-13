#ifndef LAYER_H
#define LAYER_H

#include <string>

namespace layers {

	/*
	* Abstract layer class to defines the basic functionalities all layers should have.
	* Layers could be of 2 types, layers and overlays. 
	*/
	class Layer
	{
	protected:
		std::string m_LayerName;
	
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent() {}

		inline const std::string& GetName() const { return m_LayerName; }
	
	};
}

#endif // !LAYER_H
