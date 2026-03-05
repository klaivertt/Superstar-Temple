#include "Button.hpp"
#include "Tools/Physics/Physics.hpp"

Button::Button(GameData* _data) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Interactable/Button.png");
	sprite.SetTexture(&texture);
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(800, 100), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });


	triggerRange = 55.f;
	SetTriggerRange(triggerRange);
}

void Button::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void Button::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Button::OnCollisionEnter(ColEvent _col)
{
	isPressed = true;
}

void Button::OnCollisionExit(ColEvent _col)
{
	isPressed = false;
}

void Button::OnInteract(Actor* _interactingActor)
{
	if (target != nullptr)
	{
		if (Interactable* interactOwner = dynamic_cast<Interactable*>(target))
		{
			Interactable* interacte = dynamic_cast<Interactable*>(target);
			interacte->OnInteract(nullptr);
		}
	}
}

bool Button::ReturnState(void)
{
	return isPressed;
}