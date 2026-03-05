#include "Scene.hpp"

#include "SceneManager.hpp"

#include <unordered_set>

Scene::Scene(GameData* _data)
{
	data = _data;

	actors = new Array<Actor*>();
	//data->manager->CreateWorld();
}

Scene::~Scene(void)
{

}

void Scene::Load(void)
{
	actorsCount = 0;
	ordered = false;
	actorTypeCounters.clear();
}

void Scene::Update(float _dt)
{
	//data->guiManager->Update(_dt);

	for (int i = 0; i < actors->Size(); i++)
	{
		Actor* actor = actors->Get(i);
		actor->Update(_dt);
	}

	UpdateCollisions(_dt);
}

void Scene::PollEvents(sf::Event& _event)
{

}

void Scene::Draw(sf::RenderTarget* _render)
{
	//Array<Actor*>* actorOrdred = new Array<Actor*>();
	//*actorOrdred = *actors;
	if (ordered == false)
	{
		for (int i = 0; i < actors->Size() - 1; i++)
		{
			for (int j = i + 1; j < actors->Size(); j++)
			{
				if (actors->Get(i)->z > actors->Get(j)->z)
				{
					Actor* temp = actors->Get(i);
					(*actors)[i] = actors->Get(j);
					(*actors)[j] = temp;
				}
			}
		}
		ordered = true;
	}

	if (camera)
	{
		_render->setView(*camera);
	}

	for (int i = 0; i < actors->Size(); i++)
	{
		Actor* actor = actors->Get(i);
		actor->Draw(_render);
	}

	b2World_Draw(data->physicsWorld, &data->manager->debugDraw);

	_render->setView(_render->getDefaultView());

	//delete actorOrdred;
}

void Scene::Destroy(void)
{
	for (int i = 0; i < actors->Size(); i++)
	{
		delete (*actors)[i];
	}
	actors->Clear();
	delete actors;

	//data->guiManager->Clear();
	actorTypeCounters.clear();
}

void Scene::AddActor(Actor* _actor)
{
	actors->Add(_actor);
	ordered = false;

	_actor->name = _actor->GetClassName() + "_" + std::to_string(actorsCount);
	Logger::Success(_actor->name + " Created");
	actorsCount++;


	//data->guiManager->AddHierarchyButton("Actor list", _actor->name, _actor, [this](Actor* newAct)
	//	{
	//		ChangeObjectSeeker(newAct);
	//	});

}

void Scene::RemoveActor(Actor* _actor)
{
	int i = 0;
	for (int i = 0; i < actors->Size(); i++)
	{
		Actor* actor = actors->Get(i);
		if (actor == _actor)
		{
			actors->Remove(i);
			break;
		}
	}
	ordered = false;
}

void Scene::KeyPressed(sf::Keyboard::Key _key)
{

}

void Scene::KeyReleased(sf::Keyboard::Key _key)
{
}

void Scene::MousePressed(sf::Event::MouseButtonEvent _button)
{
}

void Scene::MouseReleased(sf::Event::MouseButtonEvent _button)
{
}

void Scene::MouseMoved(sf::Event::MouseMoveEvent _moved)
{
}

void Scene::TextEntered(sf::Event::TextEvent _text)
{
}

void Scene::ChangeObjectSeeker(Actor* _actor)
{
}

int Scene::GetNextActorID(const std::string& _className)
{
	if (actorTypeCounters.find(_className) == actorTypeCounters.end())
	{
		actorTypeCounters[_className] = 0;
	}

	int currentID = actorTypeCounters[_className];

	actorTypeCounters[_className]++;

	return currentID;
}
void Scene::UpdateCollisions(float _dt)
{
	Physics::Update(data->physicsWorld, _dt, data->physicsQuality);


	b2ContactEvents events = b2World_GetContactEvents(data->physicsWorld);

	// Start touch collisions
	for (int i = 0; i < events.beginCount; i++)
	{
		b2ContactBeginTouchEvent* event = events.beginEvents + i;

		if (b2Shape_IsValid(event->shapeIdA) && b2Shape_IsValid(event->shapeIdB))
		{
			Actor* actorA = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdA));
			Actor* actorB = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdB));

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(event->manifold.normal.x, event->manifold.normal.y);

			if (actorA != nullptr)
			{
				actorA->OnCollisionEnter(col);
			}
			if (actorB != nullptr)
			{
				col.other = actorA;
				actorB->OnCollisionEnter(col);
			}
		}
	}

	// End touch collisions
	for (int i = 0; i < events.endCount; i++)
	{
		b2ContactEndTouchEvent* event = events.endEvents + i;

		if (b2Shape_IsValid(event->shapeIdA) && b2Shape_IsValid(event->shapeIdB))
		{
			Actor* actorA = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdA));
			Actor* actorB = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdB));

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);

			if (actorA != nullptr)
			{
				actorA->OnCollisionExit(col);
			}
			if (actorB != nullptr)
			{
				col.other = actorA;
				actorB->OnCollisionExit(col);
			}
		}
	}

	// Hit touch collisions
	for (int i = 0; i < events.hitCount; i++)
	{
		b2ContactHitEvent* event = events.hitEvents + i;

		if (b2Shape_IsValid(event->shapeIdA) && b2Shape_IsValid(event->shapeIdB))
		{
			Actor* actorA = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdA));
			Actor* actorB = static_cast<Actor*>(b2Shape_GetUserData(event->shapeIdB));

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(event->normal.x, event->normal.y);

			if (actorA != nullptr)
			{
				actorA->OnCollisionHit(col);
			}
			if (actorB != nullptr)
			{
				col.other = actorA;
				actorB->OnCollisionHit(col);
			}
		}
	}

	b2SensorEvents sensors = b2World_GetSensorEvents(data->physicsWorld);

	// Start trigger collisions
	for (int i = 0; i < sensors.beginCount; i++)
	{

		b2SensorBeginTouchEvent* event = sensors.beginEvents + i;
		if (b2Shape_IsValid(event->sensorShapeId) && b2Shape_IsValid(event->visitorShapeId))
		{
			Actor* actorA = static_cast<Actor*>(b2Shape_GetUserData(event->sensorShapeId));
			Actor* actorB = static_cast<Actor*>(b2Shape_GetUserData(event->visitorShapeId));
			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);


			if (actorA != nullptr)
			{
				actorA->OnTriggerEnter(col);
			}
			if (actorB != nullptr)
			{
				col.other = actorA;
				actorB->OnTriggerEnter(col);
			}
		}
	}

	// End trigger collisions
	for (int i = 0; i < sensors.endCount; i++)
	{
		b2SensorEndTouchEvent* event = sensors.endEvents + i;
		if (b2Shape_IsValid(event->sensorShapeId) && b2Shape_IsValid(event->visitorShapeId))
		{
			Actor* actorA = static_cast<Actor*>(b2Shape_GetUserData(event->sensorShapeId));
			Actor* actorB = static_cast<Actor*>(b2Shape_GetUserData(event->visitorShapeId));
			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);
			if (actorA != nullptr)
			{
				actorA->OnTriggerExit(col);
			}
			if (actorB != nullptr)
			{
				actorB->OnTriggerExit(col);
			}
		}
	}
}

