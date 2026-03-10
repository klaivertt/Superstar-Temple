#include "Box.hpp"
#include "Tools/Physics/Physics.hpp"
#include "../Player/Player.hpp"

Box::Box(GameData* _data, Vec2 _pos): Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Interactable/Box.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { _pos, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });

	triggerRange = 100.f;

	SetTriggerRange(triggerRange);
}

void Box::Update(float _dt)
{
	if (owner != nullptr)
	{
		// If the Box has an owner, it means that it is being carried by an actor, so we set its position to the owner's position
		position = owner->position;
		Physics::Transform transform = Physics::GetBodyTransform(owner->body);

		//set Box position to the front of the owner (in the direction of the owner's orientation) and on front on him (player = 64.f, 64.f)
		transform.position += Vec2(cos(transform.rotation) * 64.f, sin(transform.rotation) * 64.f);

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

void Box::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Box::OnCollisionEnter(ColEvent _col)
{
}

void Box::OnCollisionExit(ColEvent _col)
{
}

void Box::OnInteract(Actor* _interactingActor)
{
	if (owner == _interactingActor)
	{
		// If the interacting actor is the owner of the key, we drop the key
		owner = nullptr;
		b2Body_Enable(body);
		return;
	}
	// Delete the key when interact with it
	//Actor::~Actor();
	owner = _interactingActor;
	if (owner != nullptr)
	{
		// desactivate collision of the body
		b2Body_Disable(body);
	}
}