#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "../../Common.hpp"
#include "Tools/Miscellaneous/Delegate.hpp"

DECLARE_DELEGATE_OneParam(OnSliderCursorClicked, float)

enum class SliderType
{
	HORIZONTAL,
	VERTICAL,
	COUNT,
};

enum class SliderUpdateValueType
{
	CONTINUOUS, // Update the value continuously while dragging
	ON_RELEASE, // Update the value only when the mouse button is released
};

class Slider
{
public:

	Slider() = default;
	~Slider() = default;

	void Create(SliderUpdateValueType _updateType, float _minValue, float _maxValue, const sf::Texture* _sliderTexture, const sf::Texture* _cursorTexture, sf::Vector2f pos, SliderType _type = SliderType::VERTICAL, sf::Vector2f normalizedOrigin = { 0.5f, 0.5f }, sf::Vector2f scale = { 1.f, 1.f });

	void Update(sf::Vector2f mousePosition);
	void Draw(sf::RenderTarget& _target);

	// Float required in argument of the function, value of the slider will be send as argument
	template<typename T, typename... Args>
	void DelegateChangeValue(T* object, void(T::* method)(Args...))
	{
		if (!delegateUpdateValue)
		{
			delegateUpdateValue = new OnSliderCursorClicked();
		}
		delegateUpdateValue->Add(object, method);
	}

	//Create a template at the gived path.
	// WARNING: _templateFilePath need to contain the full path + the template file name without extension.
	// example : _templateFilePath + ".png"
	void GenerateTemplateSliderTexture(const std::string& _templateFilePath, sf::Vector2i _sliderSizeInPixel);
	//Create a template at the gived path.
	// WARNING: _templateFilePath need to contain the full path + the template file name without extension.
	// example : _templateFilePath + ".png"
	void GenerateTemplateCursorTexture(const std::string& _templateFilePath, sf::Vector2i _cursorSizeInPixel);

	void CreateName(const sf::Font* _font, const std::string& _name, sf::Vector2f _posOffset, int _size, sf::Vector2f _normalizedOrigin = { 0.5f, 0.5f }, sf::Vector2f _scale = { 1.f,1.f });

	float GetValue() const { return givedValue; }
	void SetNewValue(float _newValue);
	bool show = true;
	bool showName = true;
private:

	void CreateCursor(const sf::Texture* _cursorTexture, sf::Vector2f normalizedOrigin = { 0.5f, 0.5f }, sf::Vector2f scale = { 1.f, 1.f });

private:

	sf::Sprite sliderSprite;
	sf::Sprite cursorSprite;
	bool dragging = false;
	SliderType type = SliderType::HORIZONTAL;
	float value = 0.f;
	float givedValue = 0.f;
	sf::Text name;
	sf::Vector2f nameOffset = { 0.f, 0.f };

	float minValue = 0.f;
	float maxValue = 1.f;
	SliderUpdateValueType updateValueType = SliderUpdateValueType::CONTINUOUS;

	OnSliderCursorClicked* delegateUpdateValue = nullptr;
	sf::RenderWindow* render = nullptr;
};

#endif