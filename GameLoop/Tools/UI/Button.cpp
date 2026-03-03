#include "Button.hpp"
#include "GameData.hpp"

#ifdef _WIN32
inline std::ofstream nullStream("NUL");      // Windows
#else
inline std::ofstream nullStream("/dev/null"); // Linux / macOS
#endif

void Button::Create(const sf::Texture* _texture, sf::Vector2f _pos, sf::Vector2f _normalizedOrigin, sf::Vector2f _scale, sf::Vector2f _normalizedCollisionSize)
{
	this->position = _pos;

	sf::IntRect rect{
		0,
		0,
		(int)_texture->getSize().x,
		(int)(_texture->getSize().y / (unsigned long long)ButtonState::COUNT)
	};

	this->sprite.setTexture(*_texture);
	this->sprite.setTextureRect(rect);
	this->sprite.setPosition(_pos);
	this->sprite.setOrigin({ rect.width * _normalizedOrigin.x,rect.height * _normalizedOrigin.y });
	this->sprite.setScale(_scale);

	sf::Vector2f size{
		this->sprite.getGlobalBounds().width,
		this->sprite.getGlobalBounds().height
	};

	this->hitBox.setSize({ size.x * _normalizedCollisionSize.x, size.y * _normalizedCollisionSize.y });
	this->hitBox.setPosition(_pos);
	this->hitBox.setOrigin({ size.x * _normalizedOrigin.x,size.y * _normalizedOrigin.y });
	this->hitBox.setScale(_scale);
	this->hitBox.setFillColor(sf::Color::Transparent);

	this->maintained = false;
	this->freezed = false;
	SetState(ButtonState::NONE);

	this->render = GameData::GetInstance()->render;
}

void Button::Update(void)
{
	//Update position
	this->sprite.setPosition(this->position + this->positionOffset);
	this->hitBox.setPosition(this->position + this->positionOffset);

	//mouse position
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(*this->render);

	this->clicked = false;

	if (this->freezed || !this->show)return;

	bool buttonClicked = false;

	if (this->Hovered(mousePos))
	{
		if (this->maintained)
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->maintained = false;
				buttonClicked = true;
			}
		}
		else
		{
			this->maintained = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		}

		if (this->maintained)
			SetState(ButtonState::CLICKED);
		else
			SetState(ButtonState::HOVERED);
	}
	else
	{
		SetState(ButtonState::NONE);
		this->maintained = false;
	}

	if (buttonClicked)
	{
		if (this->clickActionDelegate)
		{
			this->clickActionDelegate->Broadcast();
		}
		this->clicked = true;
	}
}

bool Button::Clicked(void)
{
	return this->clicked;
}

bool Button::Hovered(sf::Vector2f position)
{
	if (this->freezed)return false;

	sf::FloatRect rect = this->sprite.getGlobalBounds();
	rect.width = this->sprite.getScale().x * (float)this->sprite.getTextureRect().width;
	rect.height = this->sprite.getScale().y * (float)this->sprite.getTextureRect().height;

	return rect.contains(position);
}

void Button::Draw(sf::RenderTarget& _target)
{
	if (this->show)_target.draw(this->sprite);
	if (this->showDebugHitbox)_target.draw(this->hitBox);
}

void Button::Freeze(bool _freezed, ButtonState _freezedState)
{
	this->freezed = _freezed;
	if (this->freezed)
	{
		SetState(_freezedState);
	}
}

void Button::GenerateTemplateTexture(const std::string& _templateFilePath, const std::string& _buttonText, const sf::Font* _font, sf::Vector2i _buttonSizeInPixel)
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
			std::cout << "GenerateTemplateTexture : Template texture already exist at path : " << _templateFilePath + ".png" << std::endl;
			// Restore console output
			sf::err().rdbuf(std::cerr.rdbuf());
			return;
		}
		// Restore console output
		sf::err().rdbuf(std::cerr.rdbuf());
	}

	sf::Image templateImage;
	templateImage.create(_buttonSizeInPixel.x, _buttonSizeInPixel.y * int(ButtonState::COUNT), sf::Color::Transparent);
	for (int i = 0; i < (int)ButtonState::COUNT; i++)
	{
		//Drawing state separator lines
		for (int y = 0; y < _buttonSizeInPixel.y; y++)
		{
			for (int x = 0; x < _buttonSizeInPixel.x; x++)
			{
				sf::Color color = sf::Color::White;
				switch (ButtonState(i))
				{
				case ButtonState::NONE:
					color = sf::Color::Red;
					break;
				case ButtonState::HOVERED:
					color = sf::Color::Blue;
					break;
				case ButtonState::CLICKED:
					color = sf::Color::Green;
					break;
				default:
					break;
				}

				templateImage.setPixel(x, y + i * _buttonSizeInPixel.y, color);
			}
		}

		sf::Text buttonText;
		buttonText.setString(_buttonText);

		buttonText.setFont(*_font);
		buttonText.setCharacterSize(24);
		buttonText.setFillColor(sf::Color::Black);
		buttonText.setOrigin(buttonText.getLocalBounds().width / 2.f, buttonText.getLocalBounds().height / 2.f);
		buttonText.setPosition(_buttonSizeInPixel.x / 2.f, _buttonSizeInPixel.y / 2.f);

		//Render text to texture
		sf::RenderTexture renderTexture;
		renderTexture.create(_buttonSizeInPixel.x, _buttonSizeInPixel.y);
		renderTexture.clear(sf::Color::Transparent);
		renderTexture.draw(buttonText);
		renderTexture.display();
		//Copy rendered texture to image
		templateImage.copy(renderTexture.getTexture().copyToImage(), 0, i * _buttonSizeInPixel.y, sf::IntRect{ 0,0,_buttonSizeInPixel.x,_buttonSizeInPixel.y }, true);

	}
	//Save texture to file
	templateImage.saveToFile(_templateFilePath + ".png");
}

void Button::SetState(ButtonState newState)
{
	this->state = newState;
	sf::IntRect rect{
		0,
		(int)((int)state * (this->sprite.getTexture()->getSize().y / (unsigned long long)ButtonState::COUNT)),
		(int)this->sprite.getTexture()->getSize().x,
		(int)(this->sprite.getTexture()->getSize().y / (unsigned long long)ButtonState::COUNT)
	};
	this->sprite.setTextureRect(rect);
}