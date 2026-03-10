#include "FireTrap.hpp"
#include "Tools/Physics/Physics.hpp"

#include "../Player/Player.hpp"

#include <cmath>

FireTrap::FireTrap(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrapOn.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	CreateCollider();

	sprite.SetPosition(Physics::GetBodyPosition(body));
	
	isFlameDown = false;
}

void FireTrap::Update(float _dt)
{
	visualTimer += _dt;
	sprite.SetPosition(position);

	if (isFlameDesactivatedByPlayer)
	{
		if (!isFlameDown)
		{
			FlameDown();
		}
	}
	else if (isFlameDown)
	{
		FlameUp();
	}
}

bool FireTrap::IsFlameActive() const
{
	return !isFlameDown;
}

void FireTrap::Draw(sf::RenderTarget* _render)
{
	if (IsFlameActive())
	{
		const float pulse = 0.8f + 0.2f * std::sin(visualTimer * 10.f);
		sf::RectangleShape outerFlame(sf::Vector2f(28.f * pulse, 54.f + 10.f * pulse));
		outerFlame.setOrigin(outerFlame.getSize().x * 0.5f, outerFlame.getSize().y);
		outerFlame.setPosition(sprite.GetPosition().x, sprite.GetPosition().y - 14.f);
		outerFlame.setFillColor(sf::Color(255, 104, 32, 150));

		sf::RectangleShape innerFlame(sf::Vector2f(14.f * pulse, 38.f + 8.f * pulse));
		innerFlame.setOrigin(innerFlame.getSize().x * 0.5f, innerFlame.getSize().y);
		innerFlame.setPosition(sprite.GetPosition().x, sprite.GetPosition().y - 12.f);
		innerFlame.setFillColor(sf::Color(255, 214, 84, 220));

		sf::CircleShape glow(24.f);
		glow.setOrigin(24.f, 24.f);
		glow.setPosition(sprite.GetPosition().x, sprite.GetPosition().y - 34.f);
		glow.setFillColor(sf::Color(255, 156, 48, 90));

		_render->draw(glow);
		_render->draw(outerFlame);
		_render->draw(innerFlame);
	}

	sprite.Draw(_render);
}

void FireTrap::OnCollisionEnter(ColEvent _col)
{
	Player* player = dynamic_cast<Player*>(_col.other);

	if (player)
	{
		player->ApplyFire(3.f, 5.f);
	}
}

void FireTrap::OnCollisionExit(ColEvent _col)
{
}

void FireTrap::OnInteract(Actor* _interactingActor)
{
	//si bouton appuy� -> d�sactiver le pi�ge
	CloseFireTrap();
}

void FireTrap::CloseFireTrap()
{
	isFlameDesactivatedByPlayer = !isFlameDesactivatedByPlayer;
}

void FireTrap::FlameDown()
{
	if (b2Body_IsValid(body))
	{
		b2DestroyBody(body);
	}

	isFlameDown = true;

	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrapOff.png"));
	sprite.SetColor(sf::Color(255, 255, 255, 170));

}

void FireTrap::FlameUp()
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrapOn.png"));
	sprite.SetColor(sf::Color::White);
	CreateCollider();

	isFlameDown = false;

}

void FireTrap::CreateCollider()
{
	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, textureSize });
}
