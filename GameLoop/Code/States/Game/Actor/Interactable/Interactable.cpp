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
	// onDestroyed is broadcast automatically by Actor::~Actor().
	// No need to call it here.

	// clear the target of the owner if exist
	if (owner != nullptr)
	{
		if (Interactable* interactOwner = dynamic_cast<Interactable*>(owner))
		{
			Interactable* targetInteractable = dynamic_cast<Interactable*>(owner);
			targetInteractable->ClearTarget();
		}
	}

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

void Interactable::SetTarget(Actor* _target)
{
	target = _target;
	if (target != nullptr)
	{
		if (Interactable* interactOwner = dynamic_cast<Interactable*>(target))
		{
			Interactable* targetInteractable = dynamic_cast<Interactable*>(target);
			targetInteractable->SetOwner(this);
		}
	}
}

void Interactable::ClearTarget()
{
	target = nullptr;
}

void Interactable::SetOwner(Actor* _owner)
{
	owner = _owner;
}

void Interactable::OnInteract(Actor* _interactingActor)
{
}
