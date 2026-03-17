#include "Duel.hpp"
#include "Actor/Player/Player.hpp"

Duel::Duel(GameData* _data) : Actor(_data)
{
	spriteP1.SetTexture("Assets/Sprites/Game/Player.png");
	spriteP1.SetPosition(Vec2(SCREEN_W / 4, SCREEN_H / 2));

	spriteP2.SetTexture("Assets/Sprites/Game/Player.png");
	spriteP2.SetColor(sf::Color(125, 200, 125));
	spriteP2.SetPosition(Vec2(SCREEN_W / 4 * 3, SCREEN_H / 2));

	for (int i = 0; i < 2; i++)
	{
		player[i].spriteBullet.SetTexture("Assets/Sprites/Game/Duel/Bullet.png");
		rect[i].setSize(sf::Vector2f(SCREEN_W, SCREEN_H / 2));
		rect[i].setFillColor(sf::Color::Black);
	}

	player[0].spriteBullet.SetPosition(Vec2(spriteP1.GetPosition().x + spriteP1.GetGlobalBounds().width, spriteP1.GetPosition().y + spriteP1.GetGlobalBounds().height / 2));
	player[1].spriteBullet.SetPosition(Vec2(spriteP2.GetPosition().x - player[1].spriteBullet.GetGlobalBounds().width, spriteP2.GetPosition().y + spriteP2.GetGlobalBounds().height / 2));
	rect[0].setPosition(0, -rect[0].getGlobalBounds().height);
	rect[1].setPosition(0, SCREEN_H);

	timer = 5.f;
	isFinished = false;
	isWinScreen = false;

	text = new Text(FontType::BOLD, Vec2(0.5f, 0.5f), sf::Color::White, 150);
	text->CreateShadow(Vec2(2.f, 2.f), sf::Color::White);
	text->SetPosition(SCREEN_W / 2, SCREEN_H / 20);
}

Duel::~Duel()
{
}

void Duel::Update(float _dt)
{
	if (timer > 0.f)
	{
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(1) << timer;
		text->SetString(stream.str());
	}

	timer -= _dt;

	if (timer < 0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			player[0].timerShoot = timer;
			player[0].isShooting = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
		{
			player[1].timerShoot = timer;
			player[1].isShooting = true;
		}
	}

	if (player[0].timerShoot < player[1].timerShoot)
	{
		winner = 0;
	}
	else
	{
		winner = 1;
	}

	if (rect[0].getPosition().y <= -rect[0].getGlobalBounds().height / 2)
	{
		rect[0].move(0, 100 * _dt);
	}
	if (rect[1].getPosition().y >= SCREEN_H - rect[1].getGlobalBounds().height / 2)
	{
		rect[1].move(0, -100 * _dt);
	}

	StartDuel(_dt);

	if (isWinScreen)
	{
		EndDuel(_dt, winner);
	}
}

void Duel::Draw(sf::RenderTarget* _render)
{
	spriteP1.Draw(_render);
	spriteP2.Draw(_render);

	for (int i = 0; i < 2; i++)
	{
		if (!isWinScreen)
		{
			player[i].spriteBullet.Draw(_render);
		}
		_render->draw(rect[i]);
	}
	text->Draw(*_render);
}

void Duel::StartDuel(float _dt)
{
	if (player[0].isShooting)
	{
		if (player[0].spriteBullet.GetPosition().x < spriteP2.GetPosition().x - 100)
		{
			float tempPosX = player[0].spriteBullet.GetPosition().x;
			tempPosX += 8000 * _dt;
			player[0].spriteBullet.SetPosition(Vec2(tempPosX, player[0].spriteBullet.GetPosition().y));
		}
		else
		{
			isFinished = true;
		}
	}

	if (player[1].isShooting)
	{
		if (player[1].spriteBullet.GetPosition().x > spriteP1.GetPosition().x + 100)
		{
			float tempPosX = player[1].spriteBullet.GetPosition().x;
			tempPosX -= 8000 * _dt;
			player[1].spriteBullet.SetPosition(Vec2(tempPosX, player[1].spriteBullet.GetPosition().y));
		}
		else
		{
			isFinished = true;
		}
	}

	if (isFinished)
	{
		rect[0].setPosition(0, 0);
		rect[1].setPosition(0, SCREEN_H - rect[1].getLocalBounds().height);


		if (timerTransition > 0)
		{
			timerTransition -= _dt;
		}
		else
		{
			isWinScreen = true;
		}
	}
}

void Duel::EndDuel(float _dt, int _player)
{
	text->SetString("WINNER");
	for (int i = 0; i < 2; i++)
	{
		if (rect[i].getFillColor().a > 0)
		{
			rect[i].setFillColor(sf::Color(rect[i].getFillColor().r, rect[i].getFillColor().g, rect[i].getFillColor().b, rect[i].getFillColor().a - 100 * _dt));
		}
	}

	switch (winner)
	{
	case 0:
		spriteP1.SetPosition(Vec2(SCREEN_W / 2 - spriteP1.GetGlobalBounds().width / 2, SCREEN_H / 2 - spriteP1.GetGlobalBounds().height / 2));
		spriteP1.SetScale(Vec2(2.f, 2.f));
		spriteP2.SetPosition(Vec2(SCREEN_W, SCREEN_H));
		break;
	case 1:
		spriteP1.SetPosition(Vec2(SCREEN_W, SCREEN_H));
		spriteP2.SetPosition(Vec2(SCREEN_W / 2 - spriteP2.GetGlobalBounds().width / 2, SCREEN_H / 2 - spriteP2.GetGlobalBounds().height / 2));
		spriteP2.SetScale(Vec2(2.f, 2.f));
		break;
	default:
		break;
	}
}