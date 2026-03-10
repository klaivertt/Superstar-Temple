#include "Button.hpp"
#include "Tools/Physics/Physics.hpp"

Button::Button(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Interactable/Button.png");
	sprite.SetTexture(&texture);
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, Vec2(0, 0) }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0,0), 0.f, textureSize });

	//triggerRange = 55.f;
	//SetTriggerRange(triggerRange);
}

void Button::NotifyTargets()
{
	for (Interactable* interactable : targets)
	{
		if (interactable != nullptr)
		{
			interactable->OnInteract(this);
		}
	}
}

void Button::SetColor(const sf::Color& _color)
{
	sprite.SetColor(_color);
}

void Button::AddTarget(Interactable* _target)
{
	if (_target == nullptr)
	{
		return;
	}

	for (Interactable* existingTarget : targets)
	{
		if (existingTarget == _target)
		{
			return;
		}
	}

	targets.push_back(_target);
	Interactable::SetTarget(_target);
}

void Button::SetTarget(Actor* _target)
{
	AddTarget(dynamic_cast<Interactable*>(_target));
}

void Button::ClearTarget()
{
	targets.clear();
	Interactable::ClearTarget();
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
}

bool Button::ReturnState(void)
{
	return isPressed;
}

void Button::OnTriggerEnter(ColEvent _col)
{
	// only if is a player or a box that collide with the button, it will be pressed
	if (_col.other != nullptr)
	{
		if (Actor* actor = dynamic_cast<Actor*>(_col.other))
		{
			if (actor->GetClassName() == "Player" || actor->GetClassName() == "Box")
			{
				pressingActorsCount++;
				if (!isPressed)
				{
					isPressed = true;
					NotifyTargets();
				}
			}
		}
	}
}

void Button::OnTriggerExit(ColEvent _col)
{
	if (_col.other != nullptr)
	{
		if (Actor* actor = dynamic_cast<Actor*>(_col.other))
		{
			if ((actor->GetClassName() == "Player" || actor->GetClassName() == "Box") && pressingActorsCount > 0)
			{
				pressingActorsCount--;
				if (pressingActorsCount == 0 && isPressed)
				{
					isPressed = false;
					NotifyTargets();
				}
			}
		}
	}
}
