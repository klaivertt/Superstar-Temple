#include "Scene.hpp"

#include "SceneManager.hpp"

#include <unordered_set>

Scene::Scene(GameData* _data)
{
	data = _data;

	actors = new Array<Actor*>();
	//data->manager->CreateWorld();
	uiElements = new Array<AutoUi*>();
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

	for (int i = 0; i < uiElements->Size(); i++)
	{
		AutoUi* ui = uiElements->Get(i);
		ui->Update(_dt);
	}

	UpdateCollisions(_dt);
}

void Scene::PollEvents(sf::Event& _event)
{

}

void Scene::Draw(sf::RenderTarget* _render)
{
	DrawWorld(_render);
	_render->setView(_render->getDefaultView());
	DrawUi(_render);
}

void Scene::DrawWorld(sf::RenderTarget* _render)
{
	EnsureActorsOrder();

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
}

void Scene::DrawUi(sf::RenderTarget* _render)
{
	for (int i = 0; i < uiElements->Size(); i++)
	{
		AutoUi* ui = uiElements->Get(i);
		ui->Draw(_render);
	}
}

void Scene::EnsureActorsOrder()
{
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
}

void Scene::Destroy(void)
{
	for (int i = 0; i < actors->Size(); i++)
	{
		delete (*actors)[i];
	}
	actors->Clear();
	delete actors;

	for (int i = 0; i < uiElements->Size(); i++)
	{
		delete (*uiElements)[i];
	}
	uiElements->Clear();
	delete uiElements;

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

void Scene::AddUi(AutoUi* _ui)
{
	uiElements->Add(_ui);
	_ui->name = _ui->GetClassName() + "_" + std::to_string(uiElements->Size());
	Logger::Success(_ui->name + " Created");
}

void Scene::RemoveUi(AutoUi* _ui)
{
	int i = 0;
	for (int i = 0; i < uiElements->Size(); i++)
	{
		AutoUi* ui = uiElements->Get(i);
		if (ui == _ui)
		{
			uiElements->Remove(i);
			break;
		}
	}
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
			void* userDataA = b2Shape_GetUserData(event->shapeIdA);
			void* userDataB = b2Shape_GetUserData(event->shapeIdB);

			// Check if userData is ShapeData or just Actor pointer
			Actor* actorA = nullptr;
			Actor* actorB = nullptr;
			std::string sensorIdA = "";
			std::string sensorIdB = "";

			// Try to get ShapeData, fallback to Actor pointer for backward compatibility
			if (userDataA != nullptr)
			{
				ShapeData* shapeDataA = static_cast<ShapeData*>(userDataA);
				actorA = shapeDataA->actor;
				sensorIdA = shapeDataA->sensorId;
			}

			if (userDataB != nullptr)
			{
				ShapeData* shapeDataB = static_cast<ShapeData*>(userDataB);
				actorB = shapeDataB->actor;
				sensorIdB = shapeDataB->sensorId;
			}

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(event->manifold.normal.x, event->manifold.normal.y);
			col.sensorId = sensorIdA;

			if (actorA != nullptr)
			{
				actorA->OnCollisionEnter(col);
			}

			if (actorB != nullptr)
			{
				col.other = actorA;
				col.sensorId = sensorIdB;
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
			void* userDataA = b2Shape_GetUserData(event->shapeIdA);
			void* userDataB = b2Shape_GetUserData(event->shapeIdB);

			Actor* actorA = nullptr;
			Actor* actorB = nullptr;
			std::string sensorIdA = "";
			std::string sensorIdB = "";

			if (userDataA != nullptr)
			{
				ShapeData* shapeDataA = static_cast<ShapeData*>(userDataA);
				actorA = shapeDataA->actor;
				sensorIdA = shapeDataA->sensorId;
			}

			if (userDataB != nullptr)
			{
				ShapeData* shapeDataB = static_cast<ShapeData*>(userDataB);
				actorB = shapeDataB->actor;
				sensorIdB = shapeDataB->sensorId;
			}

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);
			col.sensorId = sensorIdA;

			if (actorA != nullptr)
			{
				actorA->OnCollisionExit(col);
			}

			if (actorB != nullptr)
			{
				col.other = actorA;
				col.sensorId = sensorIdB;
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
			void* userDataA = b2Shape_GetUserData(event->shapeIdA);
			void* userDataB = b2Shape_GetUserData(event->shapeIdB);

			Actor* actorA = nullptr;
			Actor* actorB = nullptr;
			std::string sensorIdA = "";
			std::string sensorIdB = "";

			if (userDataA != nullptr)
			{
				ShapeData* shapeDataA = static_cast<ShapeData*>(userDataA);
				actorA = shapeDataA->actor;
				sensorIdA = shapeDataA->sensorId;
			}

			if (userDataB != nullptr)
			{
				ShapeData* shapeDataB = static_cast<ShapeData*>(userDataB);
				actorB = shapeDataB->actor;
				sensorIdB = shapeDataB->sensorId;
			}

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(event->normal.x, event->normal.y);
			col.sensorId = sensorIdA;

			if (actorA != nullptr)
			{
				actorA->OnCollisionHit(col);
			}

			if (actorB != nullptr)
			{
				col.other = actorA;
				col.sensorId = sensorIdB;
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
			void* userDataSensor = b2Shape_GetUserData(event->sensorShapeId);
			void* userDataVisitor = b2Shape_GetUserData(event->visitorShapeId);

			Actor* actorA = nullptr;
			Actor* actorB = nullptr;
			std::string sensorIdA = "";
			std::string sensorIdB = "";

			if (userDataSensor != nullptr)
			{
				ShapeData* shapeDataA = static_cast<ShapeData*>(userDataSensor);
				actorA = shapeDataA->actor;
				sensorIdA = shapeDataA->sensorId;
			}

			if (userDataVisitor != nullptr)
			{
				ShapeData* shapeDataB = static_cast<ShapeData*>(userDataVisitor);
				actorB = shapeDataB->actor;
				sensorIdB = shapeDataB->sensorId;
			}

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);
			col.sensorId = sensorIdA;

			if (actorA != nullptr)
			{
				actorA->OnTriggerEnter(col);
			}

			if (actorB != nullptr)
			{
				col.other = actorA;
				col.sensorId = sensorIdB;
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
			void* userDataSensor = b2Shape_GetUserData(event->sensorShapeId);
			void* userDataVisitor = b2Shape_GetUserData(event->visitorShapeId);

			Actor* actorA = nullptr;
			Actor* actorB = nullptr;
			std::string sensorIdA = "";
			std::string sensorIdB = "";

			if (userDataSensor != nullptr)
			{
				ShapeData* shapeDataA = static_cast<ShapeData*>(userDataSensor);
				actorA = shapeDataA->actor;
				sensorIdA = shapeDataA->sensorId;
			}

			if (userDataVisitor != nullptr)
			{
				ShapeData* shapeDataB = static_cast<ShapeData*>(userDataVisitor);
				actorB = shapeDataB->actor;
				sensorIdB = shapeDataB->sensorId;
			}

			ColEvent col = { 0 };
			col.other = actorB;
			col.normal = Vec2(0.f, 0.f);
			col.sensorId = sensorIdA;

			if (actorA != nullptr)
			{
				actorA->OnTriggerExit(col);
			}

			if (actorB != nullptr)
			{
				col.other = actorA;
				col.sensorId = sensorIdB;
				actorB->OnTriggerExit(col);
			}
		}
	}
}

