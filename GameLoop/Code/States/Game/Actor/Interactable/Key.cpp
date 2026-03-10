#include "Key.hpp"
#include "Tools/Physics/Physics.hpp"
#include "../Player/Player.hpp"

Key::Key(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Interactable/Key.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	sprite.SetScale(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { _pos, 0.f, Vec2(64, 64) }, this, true);
	box = Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(32, 32) });

	triggerRange = 64.f;

	SetTriggerRange(triggerRange);
}

void Key::Update(float _dt)
{
	if (owner != nullptr)
	{
		// If the key has an owner, it means that it is being carried by an actor, so we set its position to the owner's position
		position = owner->position;
		Physics::Transform transform = Physics::GetBodyTransform(owner->body);
		//set Key position to the front of the owner (in the direction of the owner's orientation)
		transform.position += Vec2(cos(transform.rotation) * 48.f, sin(transform.rotation) * 48.f);
		Physics::SetBodyTransform(body, transform);

		if (Player* playerOwner = dynamic_cast<Player*>(owner))
		{
			if (playerOwner->currentInteractable != this)
			{
				playerOwner->currentInteractable = this;
			}
		}
	}

	sprite.SetPosition(Physics::GetBodyPosition(body));
	sprite.SetRotation(Physics::GetBodyRotation(body));
}

void Key::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Key::OnCollisionEnter(ColEvent _col)
{
}

void Key::OnCollisionExit(ColEvent _col)
{
}

void Key::OnInteract(Actor* _interactingActor)
{
	if (owner == _interactingActor)
	{
		//Interactable* targetInteractable = dynamic_cast<Interactable*>(target);
		if (ActivateTarget(target))
		{
			ClearTarget();
			delete this;
		}
		else
		{
			owner = nullptr;
			box = Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(32, 32) });
			return;
		}
	}
	else
	{
		// Delete the key when interact with it
		//Actor::~Actor();
		owner = _interactingActor;
		if (owner != nullptr)
		{
			if (b2Shape_IsValid(box))
			{
				b2DestroyShape(box, false);
			}
			// desactivate collision of the body
			//b2Body_Disable(body);
		}
	}
}

void Key::OnTriggerEnter(ColEvent _col)
{
	if (Interactable* interactable = dynamic_cast<Interactable*>(_col.other))
	{
		if (interactable == target)
		{
			isCollidingWithTarget = true;
		}
	}
}

void Key::OnTriggerExit(ColEvent _col)
{
	if (Interactable* interactable = dynamic_cast<Interactable*>(_col.other))
	{
		if (interactable == target)
		{
			isCollidingWithTarget = false;
		}
	}
}