#include "Actor.hpp"

#include "../../GameData.hpp"
#include "../SceneManager.hpp"

Actor::Actor(GameData* _data)
{
	data = _data;

	data->manager->currentScene->AddActor(this);
}

Actor::Actor(GameData* _data, Vec2 _pos)
{
	data = _data;
	position = _pos;

	data->manager->currentScene->AddActor(this);
}

Actor::Actor(GameData* _data, std::string _name)
{
	data = _data;
	name = _name;

	data->manager->currentScene->AddActor(this);
}

Actor::~Actor(void)
{
	if (this)
	{
		// Note : Important :
		// Quand l'acteur est détruite, on notifie tous les acteurs qui sont en train de le suivre que cet acteur est détruit, pour éviter les pointeurs invalides
		onDestroyed.Broadcast(this);

		data->manager->currentScene->RemoveActor(this);

		if (b2Body_IsValid(body))
		{
			b2DestroyBody(body);
		}
	}
}

void Actor::Update(float _dt)
{
	position = Physics::GetBodyPosition(body);
}

void Actor::Draw(sf::RenderTarget* _render)
{
}

void Actor::OnCollisionEnter(ColEvent _col)
{
}

void Actor::OnCollisionExit(ColEvent _col)
{
}

void Actor::OnCollisionHit(ColEvent _col)
{
}

void Actor::OnTriggerEnter(ColEvent _col)
{
}

void Actor::OnTriggerExit(ColEvent _col)
{
}

sf::FloatRect Actor::GetBounds() const
{
	//return base bounds, you can override this function to return the correct bounds of your actor
	sf::FloatRect bound = Physics::GetBodyBound(body);

	if (bound != sf::FloatRect(0.f, 0.f, 0.f, 0.f))
	{
		return bound;
	}

	return sf::FloatRect(position.x, position.y, 32.f, 32.f);
}

Physics::Transform Actor::GetTransform()
{
	Physics::Transform transform = { position, 0.f, Vec2(1.f, 1.f) };
	if (b2Body_IsValid(body))
	{
		transform = Physics::GetBodyTransform(body);
	}
	return transform;
}

Array<Property>* Actor::GetProperties(void)
{
	return &properties;
}
