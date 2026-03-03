#include "Menu.hpp"
#include "Tools/Miscellaneous/Text.hpp"
#include "Tools/SceneManager.hpp"

#define BUTTON_POS sf::Vector2f(SCREEN_W * 0.2f, SCREEN_H * (0.35f + 0.1f * i ))

Menu::Menu(GameData* _scene) : Scene(_scene)
{
}

void Menu::Load(void)
{
	#pragma region Buttons
	const std::string path = "Assets/Sprites/Menu/UI/";
	for (int i = 0; i < int(MenuButtonsType::COUNT); i++)
	{
		std::string buttonName = "Default";
		switch (MenuButtonsType(i))
		{
			case MenuButtonsType::CONTINUE:
				buttonName = "Continue";
				break;
			case MenuButtonsType::LOAD_SAVE:
				buttonName = "LoadSave";
				break;
			case MenuButtonsType::SETTINGS:
				buttonName = "Settings";
				break;
			case MenuButtonsType::LEAVE:
				buttonName = "Leave";
				break;
		default:
			break;
		}

		const sf::Font* font = GetFont(FontType::BOLD);
		button[i].GenerateTemplateTexture(path + buttonName + "Button", buttonName, font, {(int)300.f, int(300.f*0.33f)});
		const sf::Texture* playButtonTexture = data->assets->GetTexture(path + buttonName + "Button.png");
		button[i].Create(playButtonTexture, BUTTON_POS);

		switch (MenuButtonsType(i))
		{
		case MenuButtonsType::CONTINUE:
			button[i].DelegateClickAction(this, &Menu::OnContinueButtonClicked);
			break;
		case MenuButtonsType::LOAD_SAVE:
			button[i].DelegateClickAction(this, &Menu::OnLoadSaveButtonClicked);
			break;
		case MenuButtonsType::SETTINGS:
			button[i].DelegateClickAction(this, &Menu::OnSettingsButtonClicked);
			break;
		case MenuButtonsType::LEAVE:
			button[i].DelegateClickAction(this, &Menu::OnLeaveButtonClicked);
			break;
		default:
			break;
		}
	}
	#pragma endregion

	Scene::Load();
}

void Menu::Update(float _dt)
{
	Scene::Update(_dt);
	sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(*this->data->render);

	parralaxOffset = -(mousePosition - SCREEN_SIZE / 2.f) * 0.03f;
	parralaxOffset.y = 0;

	//Buttons
	for (int i = 0; i < int(MenuButtonsType::COUNT); i++)
	{
		this->button[i].positionOffset = parralaxOffset;
		this->button[i].Update();
	}
}

void Menu::Draw(sf::RenderTarget* _render)
{
	Scene::Draw(_render);

	// Buttons
	for (int i = 0; i < int(MenuButtonsType::COUNT); i++)
	{
		this->button[i].Draw(*_render);
	}
}

void Menu::Destroy(void)
{
	Scene::Destroy();
}

void Menu::OnContinueButtonClicked(void)
{
	std::cout << "Continue button clicked!" << std::endl;
	this->data->manager->LoadScene("Game");
}

void Menu::OnLoadSaveButtonClicked(void)
{
	std::cout << "Load Save button clicked!" << std::endl;
}

void Menu::OnSettingsButtonClicked(void)
{
	std::cout << "Settings button clicked!" << std::endl;
}

void Menu::OnLeaveButtonClicked(void)
{
	this->data->render->close();
}