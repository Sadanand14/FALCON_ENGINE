#include "UIManager.h"

namespace UI 
{
	/**
	* Main constructor. Creates and sets up the cavas object.
	*/
	UI_Manager::UI_Manager() 
	{
		canvas = fmemory::fnew<Canvas>();
		canvas->Setup();
	}

	/**
	* Destructor
	*/
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

	/**
	* Load the requested layer through its title.
	*
	* @param[in] a string which basically indicates the title of the page for mapping.
	*/
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

	/**
	* Primary function for adding a button to a UI layer.
	*
	* @param[in] a string indicating the title of the layer.
	* @param[in] color as vec4 indicating default button color.
	* @param[in] color as vec4 indicating hover button color.
	* @param[in] color as vec4 indicating active button color.
	* @param[in] color as vec4 indicating default text color.
	* @param[in] color as vec4 indicating hover text color.
	* @param[in] color as vec4 indicating active text color.
	* @param[in] coordinates for diagonally opposite points of the button(x1,y1,x2,y2)
	* @param[in] string to be written on the button.
	* @param[in] a callback function indicating the action on click.
	*
	* @param[out] a pointer to the button object created
	*/
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

	/**
	* Primary function for adding a slider onto a layer in UI.
	*
	* @param[in] a string indicating the title of the layer.
	* @param[in] coordinates for diagonally opposite points of the slider box(x1,y1,x2,y2)
	* @param[in] minimum slider value in float.
	* @param[in] maximum slider value in float.
	* @param[in] step-size for the slider in float.
	*
	* @param[out] pointer to the created slider object.
	*/
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

	/**
	*Primary function for adding a background image to a UI layer.
	*
	* @param[in] a string indicating the title of the layer.
	* @param[in] the name of the image file with extention as string.
	* @param[in] coordinates for diagonally opposite points of the image(x1,y1,x2,y2)
	*
	* @param[out] a pointer to the background object created.
	*/
	
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

	/**
	* Primary function for changing the image on a pre-existing background object.
	*
	* @param[in] pointer to the background object.
	* @param[in] the name of the image file with extention as string.
	*/
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