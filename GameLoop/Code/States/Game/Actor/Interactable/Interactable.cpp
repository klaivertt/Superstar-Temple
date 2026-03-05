#include "Interactable.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/Debug/Logger.hpp"

void Interactable::SetTriggerRange(float _range, Vec2 _decal)
{
	Physics::Transform transform = Physics::Transform();

	transform.position += _decal;

	Physics::CreateCircleTrigger(body, transform, _range);
}

Interactable::Interactable(GameData* _data) : Actor(_data)
{
}

Interactable::~Interactable(void)
{
	Actor::~Actor();
}

void Interactable::Update(float _dt)
{
}

void Interactable::Draw(sf::RenderTarget* _render)
{
}

void Interactable::OnCollisionEnter(ColEvent _col)
{
}

void Interactable::OnCollisionExit(ColEvent _col)
{
}

void Interactable::OnInteract(Actor* _interactingActor)
{
}
