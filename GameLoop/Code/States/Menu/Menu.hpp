#ifndef MENU_H
#define MENU_H

#include "Tools/Scene.hpp"
#include "GameData.hpp"
#include "Background/MenuBackground.hpp"
#include "Tools/UI/Button.hpp"
#include "Tools/UI/Slider.hpp"

enum class MenuButtonsType
{
	CONTINUE,
	LOAD_SAVE,
	SETTINGS,
	LEAVE,
	COUNT,
	NONE,
};


struct Menu : public Scene
{
protected:
	MenuBackground background;
	Button button[int(MenuButtonsType::COUNT)];
	sf::Vector2f parralaxOffset;
	
public:
	Menu(GameData* _scene);

	void Load(void) override;
	void Update(float _dt) override;
	void Draw(sf::RenderTarget* _render) override;
	void Destroy(void);	

	void OnContinueButtonClicked(void);
	void OnLoadSaveButtonClicked(void);
	void OnSettingsButtonClicked(void);
	void OnLeaveButtonClicked(void);
};

#endif // !MENU_H