#include "Slider.hpp"


#ifdef _WIN32
inline std::ofstream nullStream("NUL");      // Windows
#else
inline std::ofstream nullStream("/dev/null"); // Linux / macOS
#endif

void Slider::Create(SliderUpdateValueType _updateType, float _minValue, float _maxValue, const sf::Texture* _sliderTexture, const sf::Texture* _cursorTexture, sf::Vector2f pos, SliderType _type, sf::Vector2f normalizedOrigin, sf::Vector2f scale)
{
	if (_sliderTexture == nullptr)
	{
		throw std::invalid_argument("Slider::create : _sliderTexture cannot be nullptr.");
		return;
	}

	this->sliderSprite.setTexture(*_sliderTexture);
	this->sliderSprite.setPosition(pos);
	this->sliderSprite.setOrigin({ this->sliderSprite.getGlobalBounds().width * normalizedOrigin.x, this->sliderSprite.getGlobalBounds().height * normalizedOrigin.y });
	this->sliderSprite.setScale(scale);
	this->type = _type;
	this->minValue = _minValue;
	this->maxValue = _maxValue;
	this->updateValueType = _updateType;
	this->value = (_minValue + _maxValue) / 2.f;

	normalizedOrigin = { 0.5f, 0.5f };
	CreateCursor(_cursorTexture, normalizedOrigin, scale);
}

void Slider::CreateCursor(const sf::Texture* _cursorTexture, sf::Vector2f normalizedOrigin, sf::Vector2f scale)
{
	if (_cursorTexture == nullptr)
	{
		throw std::invalid_argument("Slider::addCursor : _cursorTexture cannot be nullptr.");
		return;
	}

	this->cursorSprite.setTexture(*_cursorTexture);
	this->cursorSprite.setPosition(this->sliderSprite.getPosition());
	this->cursorSprite.setOrigin({ this->cursorSprite.getGlobalBounds().width * normalizedOrigin.x,this->cursorSprite.getGlobalBounds().height * normalizedOrigin.y });
	this->cursorSprite.setScale(scale);
	SetNewValue(this->value);
}

void Slider::Update(sf::Vector2f mousePosition)
{
	if (!this->show)return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!dragging)
		{
			// Start dragging if the mouse is over the cursor
			if (cursorSprite.getGlobalBounds().contains(mousePosition))
			{
				dragging = true;
			}
		}
		else
		{
			// Update cursor position based on slider type
			float newValue = 0.f;

			if (type == SliderType::HORIZONTAL)
			{
				float clampedX = std::clamp(mousePosition.x, sliderSprite.getGlobalBounds().left, sliderSprite.getGlobalBounds().left + sliderSprite.getGlobalBounds().width);
				cursorSprite.setPosition(clampedX, sliderSprite.getGlobalBounds().top + sliderSprite.getGlobalBounds().height / 2.f);
				newValue = (clampedX - sliderSprite.getGlobalBounds().left) / sliderSprite.getGlobalBounds().width;
			}
			else if (type == SliderType::VERTICAL)
			{
				// up = 1 and down = 0
				float clampedY = std::clamp(mousePosition.y, sliderSprite.getGlobalBounds().top, sliderSprite.getGlobalBounds().top + sliderSprite.getGlobalBounds().height);
				clampedY = sliderSprite.getGlobalBounds().top + sliderSprite.getGlobalBounds().height - (clampedY - sliderSprite.getGlobalBounds().top);
				cursorSprite.setPosition(sliderSprite.getGlobalBounds().left + sliderSprite.getGlobalBounds().width / 2.f, clampedY);
				newValue = (clampedY - sliderSprite.getGlobalBounds().top) / sliderSprite.getGlobalBounds().height;
			}

			newValue = newValue * (maxValue - minValue) + minValue;

			SetNewValue(newValue);

			if (this->updateValueType == SliderUpdateValueType::CONTINUOUS)
			{
				givedValue = value;
				if (this->delegateUpdateValue)
				{
					this->delegateUpdateValue->Broadcast(this->givedValue);
				}
			}
		}
	}
	else
	{
		dragging = false; // Stop dragging when mouse button is released

		if (this->updateValueType == SliderUpdateValueType::ON_RELEASE)
		{
			if (this->givedValue != this->value)
			{
				this->givedValue = this->value;
				if (this->delegateUpdateValue)
				{
					this->delegateUpdateValue->Broadcast(this->givedValue);
				}
			}
		}
	}
}

void Slider::Draw(sf::RenderTarget& _target)
{
	if (show)
	{
		_target.draw(sliderSprite);
		_target.draw(cursorSprite);
		if (showName)
		{
			name.setPosition(sliderSprite.getPosition() + nameOffset);
			_target.draw(name);
		}
	}
}

void Slider::GenerateTemplateSliderTexture(const std::string& _templateFilePath, sf::Vector2i _sliderSizeInPixel)
{
	//filePath creation
	std::filesystem::path path(_templateFilePath);
	std::filesystem::create_directories(path.parent_path());

	//Check if template already exist
	{
		// Disable console output temporarily
		sf::err().rdbuf(nullStream.rdbuf());

		sf::Texture templateTexture;
		if (templateTexture.loadFromFile(_templateFilePath + ".png"))
		{
			std::cout << "GenerateTemplateSliderTexture : Template texture already exist at path : " << _templateFilePath + ".png" << std::endl;
			// Restore console output
			sf::err().rdbuf(std::cerr.rdbuf());
			return;
		}
		// Restore console output
		sf::err().rdbuf(std::cerr.rdbuf());
	}

	sf::Image templateImage;
	templateImage.create(_sliderSizeInPixel.x, _sliderSizeInPixel.y, sf::Color::White);
	//Save texture to file
	templateImage.saveToFile(_templateFilePath + ".png");
}

void Slider::GenerateTemplateCursorTexture(const std::string& _templateFilePath, sf::Vector2i _cursorSizeInPixel)
{
	//filePath creation
	std::filesystem::path path(_templateFilePath);
	std::filesystem::create_directories(path.parent_path());

	//Check if template already exist
	{
		// Disable console output temporarily
		sf::err().rdbuf(nullStream.rdbuf());

		sf::Texture templateTexture;
		if (templateTexture.loadFromFile(_templateFilePath + ".png"))
		{
			std::cout << "GenerateTemplateCursorTexture : Template texture already exist at path : " << _templateFilePath + ".png" << std::endl;
			// Restore console output
			sf::err().rdbuf(std::cerr.rdbuf());
			return;
		}
		// Restore console output
		sf::err().rdbuf(std::cerr.rdbuf());
	}

	sf::Image templateImage;
	templateImage.create(_cursorSizeInPixel.x, _cursorSizeInPixel.y, sf::Color::Red);
	//Save texture to file
	templateImage.saveToFile(_templateFilePath + ".png");
}

void Slider::CreateName(const sf::Font* _font, const std::string& _name, sf::Vector2f _posOffset, int _size, sf::Vector2f _normalizedOrigin, sf::Vector2f _scale)
{
	name = sf::Text();
	name.setFont(*_font);
	name.setString(_name);
	name.setCharacterSize(_size);
	name.setFillColor(sf::Color::White);
	name.setPosition(sliderSprite.getPosition() + _posOffset);
	name.setOrigin(name.getLocalBounds().width * _normalizedOrigin.x, name.getLocalBounds().height * _normalizedOrigin.y);
	name.setScale(_scale);
	nameOffset = _posOffset;
}

void Slider::SetNewValue(float _newValue)
{
	this->value = std::clamp(_newValue, this->minValue, this->maxValue);

	float normalizedValue = (this->value - this->minValue) / (this->maxValue - this->minValue);

	if (type == SliderType::HORIZONTAL)
	{
		float newX = sliderSprite.getGlobalBounds().left + (normalizedValue * sliderSprite.getGlobalBounds().width);
		cursorSprite.setPosition(newX, cursorSprite.getPosition().y);
	}
	else if (type == SliderType::VERTICAL)
	{
		float newY = sliderSprite.getGlobalBounds().top + (normalizedValue * sliderSprite.getGlobalBounds().height);
		newY = sliderSprite.getGlobalBounds().top + sliderSprite.getGlobalBounds().height - (newY - sliderSprite.getGlobalBounds().top);
		cursorSprite.setPosition(cursorSprite.getPosition().x, newY);
	}
}