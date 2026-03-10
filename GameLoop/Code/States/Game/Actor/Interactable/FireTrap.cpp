#include "FireTrap.hpp"
#include "Tools/Physics/Physics.hpp"

#include "../Player/Player.hpp"

FireTrap::FireTrap(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrapOn.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	CreateCollider();

	sprite.SetPosition(Physics::GetBodyPosition(body));
	
	isFlameDown = false;
	flameTimer = 0.f;
}

void FireTrap::Update(float _dt)
{
	if (isFlameDesactivatedByPlayer)
	{
		if (!isFlameDown)
		{
			FlameDown();
			flameTimer = 0.f;
		}
	}
	else
	{
		if (!isFlameDown)
		{
			flameTimer += _dt;
			if (flameTimer >= FIRE_DURATION)
			{
				FlameDown();
				flameTimer = 0.f;
			}
		}
		else
		{
			flameTimer += _dt;
			if (flameTimer >= FLAME_COOLDOWN)
			{
				FlameUp();
				flameTimer = 0.f;
			}
		}
	}
}

void FireTrap::Draw(sf::RenderTarget* _render)
{
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
	//si bouton appuyé -> désactiver le piège
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

}

void FireTrap::FlameUp()
{
	CreateCollider();

	isFlameDown = false;

	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrapOn.png"));

}

void FireTrap::CreateCollider()
{
	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, textureSize });
}
