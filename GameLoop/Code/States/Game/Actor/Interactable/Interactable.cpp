#include "Interactable.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/Debug/Logger.hpp"

void Interactable::SetTriggerRange(float _range, bool _centered)
{
	Physics::Transform transform = Physics::GetBodyTransform(body);
	
	if (_centered)
	{
		transform.position += Vec2(_range, -_range);
	}

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
