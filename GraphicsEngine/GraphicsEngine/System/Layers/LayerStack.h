#ifndef LAYERSTACK_H
#define LAYERSTACK_H
#include "framework.h"
#include "Layer.h"

#include <vector>

namespace layers {

	/*
	* Layerstack maintains all the layers and overlays. Layers are pushed in the front of array,
	* whereas overlays are towards the end. This is to define the rendering order.
	*/

	class LayerStack
	{

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	};

}


#endif //LAYERSTACK_H

