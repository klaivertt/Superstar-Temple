#include "Door.hpp"
#include "Tools/Physics/Physics.hpp"

Door::Door(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/Door.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	CreateCollider();
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
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });
}
