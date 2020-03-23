#ifndef U_I_MANAGER_H
#define U_I_MANAGER_H

#include <Types.h>
#include "Canvas.h"
#include "CanvasItems/Label.h"
#include "CanvasItems/Button.h"
#include "CanvasItems/Image.h"
#include "CanvasItems/Panel.h"
#include "CanvasItems/Slider.h"
#include <AssetManager.h>

namespace UI
{
	/**
	* Main UI management system. Controls one canvas object for drawing UI on it.	
	*/
	class UI_Manager
	{
	private:
		boost::unordered_map<std::string, boost::container::vector<CanvasItem*>*> m_layers;
		boost::unordered_map<std::string, Texture*> m_backgrounds;
		Canvas* canvas = nullptr;


	public:	
		UI_Manager();
		~UI_Manager();

		inline Canvas* GetCanvas() { return canvas; }

		Button* AddButton(std::string layer, glm::vec4 NBC, glm::vec4 HBC, glm::vec4 ABC, glm::vec4 NTC, glm::vec4 HTC,
			glm::vec4 ATC, glm::vec4 bounds, std::string text, boost::function<void()> callback);

		Slider* AddSlider(std::string layer, glm::vec4 , float, float, float);

		Image* AddImage(std::string layer, std::string title, glm::vec4 bounds);

		void ChangeImage(Image* bg, std::string title);

		void LoadUI(std::string page);
	};
}

#endif // !1

