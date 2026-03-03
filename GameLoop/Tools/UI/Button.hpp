#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../../Common.hpp"
#include "../../Tools/Miscellaneous/Delegate.hpp"

DECLARE_DELEGATE(OnButtonClicked)

enum class ButtonState
{
	NONE,
	HOVERED,
	CLICKED,
	COUNT,
};

class Button
{
public:
	Button() = default;
	~Button() = default;

	// _texture cannot be nullptr. if you dont have a texture, use the function [ GenerateTemplateTexture ] to create one.
	// _texture is splitted in equal parts vertically depending on the number of ButtonState.
	void Create(const sf::Texture* _texture, sf::Vector2f pos, sf::Vector2f normalizedOrigin = { 0.5f, 0.5f }, sf::Vector2f scale = { 1.f, 1.f }, sf::Vector2f _collitionSize = { 1.f, 1.f });

	void Update(void);

	bool Clicked(void);
	bool Hovered(sf::Vector2f position);
	void Draw(sf::RenderTarget& _target);
	void Freeze(bool freezed, ButtonState freezedState = ButtonState::HOVERED);

	//check if delegate .hpp exist 

	template<typename T, typename... Args>
	void DelegateClickAction(T* object, void(T::* method)(Args...))
	{
		if (!clickActionDelegate)
		{
			clickActionDelegate = new OnButtonClicked();
		}
		clickActionDelegate->Add(object, method);
	}

	//Create a template at the gived path.
	// WARNING: _templateFilePath need to contain the full path + the template file name without extension.
	// result example : _templateFilePath + ".png"
	void GenerateTemplateTexture(const std::string& _templateFilePath, const std::string& _buttonText, const sf::Font* _font, sf::Vector2i _buttonSizeInPixel);

	bool showDebugHitbox = false;
	bool show = true;
	void SetState(ButtonState state);

	sf::Vector2f position = { 0.f, 0.f };
	sf::Vector2f positionOffset = { 0.f, 0.f };

private:
	sf::Sprite sprite;
	sf::RectangleShape hitBox;

	ButtonState state = ButtonState::NONE;
	bool freezed = false;
	bool maintained = false;

	OnButtonClicked* clickActionDelegate = nullptr;
	sf::RenderWindow* render = nullptr;

	bool clicked = false;
};

#endif // BUTTON_H