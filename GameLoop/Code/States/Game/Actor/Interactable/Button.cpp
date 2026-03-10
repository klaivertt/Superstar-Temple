#include "Button.hpp"
#include "Tools/Physics/Physics.hpp"

Button::Button(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Interactable/Button.png");
	sprite.SetTexture(&texture);
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, Vec2(0, 0) }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0,0), 0.f, Vec2(64, 64) });

	//triggerRange = 55.f;
	//SetTriggerRange(triggerRange);
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

}

void Button::OnCollisionExit(ColEvent _col)
{
	isPressed = false;
}

void Button::OnInteract(Actor* _interactingActor)
{
	//if (target != nullptr)
	//{
	//	if (Interactable* interactOwner = dynamic_cast<Interactable*>(target))
	//	{
	//		Interactable* interacte = dynamic_cast<Interactable*>(target);
	//		interacte->OnInteract(nullptr);
	//	}
	//}

	// Test code : zero crash quand le bouton est d�truit alors que le player interagit avec, et l'interactable cibl� ne pose pas de probl�me
	//Interactable::~Interactable();
}

bool Button::ReturnState(void)
{
	return isPressed;
}

void Button::OnTriggerEnter(ColEvent _col)
{
	// only if is a player or a box that collide with the button, it will be pressed
	if (_col.other->GetClassName() == "Player" || _col.other->GetClassName() == "Box")
	{
		isPressed = true;
		Logger::Debug("Button pressed !" + _col.other->GetClassName());

		if (target != nullptr)
		{
			if (Interactable* interactOwner = dynamic_cast<Interactable*>(target))
			{
				Interactable* interacte = dynamic_cast<Interactable*>(target);
				interacte->OnInteract(this);
			}
		}
	}
}

void Button::OnTriggerExit(ColEvent _col)
{
	isPressed = false;
	if (target != nullptr)
	{
		if (Interactable* interactOwner = dynamic_cast<Interactable*>(target))
		{
			Interactable* interacte = dynamic_cast<Interactable*>(target);
			interacte->OnInteract(this);
		}
	}
}
