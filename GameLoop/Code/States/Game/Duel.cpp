#include "Duel.hpp"
#include "Actor/Player/Player.hpp"

Duel::Duel(GameData* _data) : Actor(_data)
{
	sf::Texture texture1;
	texture1.loadFromFile("Assets/Sprites/Game/Duel/P1.png");
	spriteP1.setTexture(texture1);
	spriteP1.setPosition(SCREEN_W / 4, SCREEN_H / 2);

	sf::Texture texture2;
	texture2.loadFromFile("Assets/Sprites/Game/Duel/P2.png");
	spriteP2.setTexture(texture2);
	spriteP2.setPosition(SCREEN_W/ 4 * 3, SCREEN_H / 2);

	sf::Texture textureBullet;
	textureBullet.loadFromFile("Assets/Sprites/Game/Duel/Bullet.png");
	for (int i = 0; i < 2; i++)
	{
		player[i].spriteBullet.setTexture(textureBullet);
		rect[i].setSize(sf::Vector2f(SCREEN_W, SCREEN_H / 2));
		rect[i].setFillColor(sf::Color::Black);
	}

	player[0].spriteBullet.setPosition(spriteP1.getPosition().x + spriteP1.getGlobalBounds().width, spriteP1.getPosition().y + spriteP1.getGlobalBounds().height / 2);
	player[1].spriteBullet.setPosition(spriteP2.getPosition().x - player[1].spriteBullet.getGlobalBounds().width, spriteP2.getPosition().y + spriteP2.getGlobalBounds().height / 2);
	rect[0].setPosition(0, -rect[0].getGlobalBounds().height);
	rect[1].setPosition(0, SCREEN_H);

	timer = 5.f;
}

Duel::~Duel()
{
}

void Duel::Update(float _dt)
{
	if (rect[0].getPosition().y <= -rect[0].getGlobalBounds().height / 2)
	{
		rect[0].move(0, 100 * _dt);
	}
	if (rect[1].getPosition().y >= SCREEN_H - rect[1].getGlobalBounds().height / 2)
	{
		rect[1].move(0, -100 * _dt);
	}

	if (!player[0].isShooting && !player[1].isShooting)
	{
		timer -= _dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		player[0].isShooting = true;
		
		float tempPosX = spriteP1.getPosition().x;
		tempPosX += 2 * _dt;
		player[0].spriteBullet.setPosition(tempPosX, spriteP1.getPosition().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player[1].isShooting = true;
	}
}

void Duel::Draw(sf::RenderTarget* _render)
{
	_render->draw(spriteP1);
	_render->draw(spriteP2);

	for (int i = 0; i < 2; i++)
	{
		_render->draw(player[i].spriteBullet);
		_render->draw(rect[i]);
	}
}