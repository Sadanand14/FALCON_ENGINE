#include "UIManager.h"

namespace UI 
{
	UI_Manager::UI_Manager() 
	{
		canvas = fmemory::fnew<Canvas>();
		canvas->Setup();
	}

	UI_Manager::~UI_Manager() 
	{
		for (auto iterator = m_layers.begin(); iterator != m_layers.end(); ++iterator) 
		{
			for (auto item : *iterator->second) 
			{
				fmemory::fdelete(item);
			}
			fmemory::fdelete(iterator->second);
		}

		for (auto iterator = m_backgrounds.begin(); iterator != m_backgrounds.end(); ++iterator) 
		{
			fmemory::fdelete(iterator->second);
		}

		if (canvas != nullptr) fmemory::fdelete(canvas);
	}

	void UI_Manager::LoadUI(std::string page) 
	{
		canvas->ClearCanvas();
		boost::container::vector<CanvasItem*>* itemArr = m_layers[page];

		for (unsigned int i = 0; i < itemArr->size(); ++i) 
		{
			itemArr->at(i)->SetActive(true);
			canvas->AddCanvasItem(itemArr->at(i));
		}
	}

	Button* UI_Manager::AddButton(std::string layer, glm::vec4 NBC, glm::vec4 HBC, glm::vec4 ABC, glm::vec4 NTC, glm::vec4 HTC,
		glm::vec4 ATC, glm::vec4 bounds, std::string text, boost::function<void()> callback)
	{
		Button* button = fmemory::fnew<Button>();
		button->SetNormalButtonColor(NBC);
		button->SetHoverButtonColor (HBC);
		button->SetActiveButtonColor(ABC);

		button->SetNormalTextColor(NTC);
		button->SetHoverTextColor(HTC);
		button->SetActiveTextColor(ATC);

		button->SetBounds(bounds);
		button->SetText(text);
		button->SetCallback(callback);

		if (m_layers.find(layer) == m_layers.end()) 
		{
			m_layers[layer] = fmemory::fnew<boost::container::vector<CanvasItem*>>();
		}

		m_layers[layer]->push_back(button);

		return button;
	}

	Slider* UI_Manager::AddSlider(std::string layer, glm::vec4 bounds, float min, float max, float step)
	{
		Slider* slider = fmemory::fnew<Slider>();
		slider->SetBounds(bounds);
		slider->SetMinValue(min);
		slider->SetMaxValue(max);
		slider->SetStep(step);

		if (m_layers.find(layer) == m_layers.end())
		{
			m_layers[layer] = fmemory::fnew<boost::container::vector<CanvasItem*>>();
		}

		m_layers[layer]->push_back(slider);

		return slider;
	}

	Image* UI_Manager::AddImage(std::string layer, std::string title, glm::vec4 bounds) 
	{
		Texture* tex;
		if (m_backgrounds.find(title)==m_backgrounds.end()) 
		{
			std::stringstream ss;
			ss << "../Assets/Textures/UI/" << title;
			tex = fmemory::fnew<Texture>();
			tex->textureID	= AssetManager::LoadTexture(ss.str());
			m_backgrounds[title] = tex;
		}
		else 
		{
			tex = m_backgrounds[title];
		}
		
		Image* background = fmemory::fnew<Image>();
		background->SetBounds(bounds);
		background->SetImage(*tex);
	
			
		if (m_layers.find(layer) == m_layers.end())
		{
			m_layers[layer] = fmemory::fnew<boost::container::vector<CanvasItem*>>();
		}

		m_layers[layer]->push_back(background);

		return background;
	}

	void UI_Manager::ChangeImage(Image* bg, std::string title) 
	{
		Texture* tex;
		if (m_backgrounds.find(title) == m_backgrounds.end())
		{
			std::stringstream ss;
			ss << "../Assets/Textures/UI/" << title;
			tex = fmemory::fnew<Texture>();
			tex->textureID = AssetManager::LoadTexture(ss.str());
			m_backgrounds[title] = tex;
		}
		else 
		{
			tex = m_backgrounds[title];
		}

		bg->SetImage(*tex);
	}
}