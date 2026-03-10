#include "Interactable.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/Debug/Logger.hpp"

void Interactable::SetTriggerRange(float _range, Vec2 _decal)
{
	Physics::Transform transform = Physics::GetBodyTransform(body);

	transform.position += _decal;

	Physics::CreateCircleTrigger(body, transform, _range);
}


bool Interactable::ActivateTarget(Actor* _interactingActor)
{
	Interactable* targetInteractable = dynamic_cast<Interactable*>(target);

	if (targetInteractable == nullptr)
	{
		return false;
	}

	if (isCollidingWithTarget)
	{
		targetInteractable->OnInteract(_interactingActor);
		return true;
	}
	else
	{
		return false;
	}

	return false;
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

void Interactable::SetTarget(Actor* _target)
{
	target = _target;
	Interactable* targetInteractable = dynamic_cast<Interactable*>(target);
	if (targetInteractable != nullptr)
	{
		targetInteractable->SetOwner(this);
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
