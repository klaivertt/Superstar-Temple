#include "Door.hpp"
#include "Tools/Physics/Physics.hpp"

Door::Door(GameData* _data, Vec2 _pos, DoorLevel _level) : Interactable(_data)
{
	doorLevel = _level;

	switch (doorLevel)
	{
	case DOOR_LEVEL_1:
		sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/Door.png"));
		break;
	case DOOR_LEVEL_2:
		sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/Door2.png"));
		break;
	case NB_DOOR_LEVELS:
		break;
	default:
		break;
	}

	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	CreateCollider();
}

void Door::SetColor(const sf::Color& _color)
{
	sprite.SetColor(_color);
}

void Door::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void Door::Draw(sf::RenderTarget* _render)
{
	if (b2Body_IsValid(body))
	{
		sprite.Draw(_render);
	}
}

void Door::OnCollisionEnter(ColEvent _col)
{
}

void Door::OnCollisionExit(ColEvent _col)
{
}

void Door::OnInteract(Actor* _interactingActor)
{
	ToggleDoor();
}

void Door::ToggleDoor()
{
	isOpen = !isOpen;
	if (isOpen)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

void Door::OpenDoor()
{
	if (b2Body_IsValid(body))
	{
		b2DestroyBody(body);
	}
}

void Door::CloseDoor()
{
	CreateCollider();
}

void Door::CreateCollider()
{
	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, textureSize }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, textureSize });
}
