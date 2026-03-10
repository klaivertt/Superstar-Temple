#include "Box.hpp"
<<<<<<< Updated upstream
=======
#include "Tools/Physics/Physics.hpp"
#include "../Player/Player.hpp"
>>>>>>> Stashed changes

Box::Box(GameData* _data) : Interactable(_data)
{

}

void Box::Update(float _dt)
{
<<<<<<< Updated upstream

=======
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
>>>>>>> Stashed changes
}

void Box::Draw(sf::RenderTarget* _render)
{

}

void Box::OnCollisionEnter(ColEvent _col)
{
}

void Box::OnCollisionExit(ColEvent _col)
{
}
<<<<<<< Updated upstream
=======

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
>>>>>>> Stashed changes
