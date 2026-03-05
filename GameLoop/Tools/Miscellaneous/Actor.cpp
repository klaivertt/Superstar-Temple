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
		data->manager->currentScene->RemoveActor(this);
		body = b2BodyId();
		//destroy colliders in the world
	}
}

void Actor::Update(float _dt)
{
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

sf::FloatRect Actor::GetBounds()
{
	//return base bounds, you can override this function to return the correct bounds of your actor
	return sf::FloatRect(position.x, position.y, 32.f, 32.f);
}

Array<Property>* Actor::GetProperties(void)
{
	return &properties;
}
