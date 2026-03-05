#pragma once

#include "../Common.hpp"

#include "../GameData.hpp"

#include "Scene.hpp"
#include "Physics/Physics.hpp"

class SceneManager
{
public:
	SceneManager(GameData* _data)
	{
		data = _data;
		currentScene = nullptr;
		render = _data->render;

		data->defaultWorld = Physics::CreateDefaultWorldDef();
		debugDraw = b2DefaultDebugDraw();
		debugDraw.context = (void*)data; //Pointeur qui permet aux fonctions de dessin de SFMLDebugDraw d'accéder au gameData parce que besoin de divers infos dedans
		debugDraw.DrawPointFcn = &Physics::DebugDrawPoint;
		debugDraw.DrawPolygonFcn = &Physics::DebugDrawPolygon;
		debugDraw.DrawSolidPolygonFcn = &Physics::DebugDrawSolidPolygon;
		debugDraw.DrawCircleFcn = &Physics::DebugDrawCircle;
		debugDraw.DrawSolidCircleFcn = &Physics::DebugDrawSolidCircle;
		debugDraw.DrawSegmentFcn = &Physics::DebugDrawSegment;
		debugDraw.DrawTransformFcn = &Physics::DebugDrawTransform;
		debugDraw.DrawStringFcn = &Physics::DebugDrawString;
		debugDraw.drawJoints = true; //Dessine les joints
		debugDraw.drawShapes = true; //Dessine les colliders
		debugDraw.drawMass = true; //Dessine les centres de gravité des bodies
	}

	void CreateWorld(void)
	{
		data->physicsWorld = Physics::CreateWorld(&data->defaultWorld);
	}

	GameData* data = nullptr;
	Scene* currentScene = nullptr;
	std::map<std::string, Scene*> scenes_list;
	sf::RenderWindow* render = nullptr;
public:

	b2DebugDraw debugDraw = { 0 };

	float dt = 0.f;

	void Update(float _dt)
	{
		if (currentScene)
		{
			currentScene->Update(_dt);
		}
	}
	void PollEvents(sf::Event& _event)
	{
		if (currentScene)
		{
			currentScene->PollEvents(_event);
		}
	}
	void Draw(sf::RenderTarget* _render)
	{
		if (currentScene)
		{
			currentScene->Draw(_render);
		}
	}

	// Create a new scene of the type you want (herited from Scene) and add a UNIQUE name
	template<class T>
	bool Create(const char* _sceneName)
	{
		if (!std::is_base_of_v<Scene, T>)
		{
			return false;
		}

		if (scenes_list.contains(_sceneName))
		{
			return false;
		}

		scenes_list[_sceneName] = new T(data);

		return true;
	}

	// Load the scene you previously created by sending the name you gave it, if the scene is already loaded it will be destroyed and reloaded
	bool LoadScene(std::string _sceneName)
	{
		if (currentScene)
		{
			Physics::Destroy(data->physicsWorld);
			currentScene->Destroy();
			GameData::GetInstance()->assets->Destroy();
		}

		currentScene = scenes_list[_sceneName];

		data->physicsWorld = Physics::CreateWorld(&data->defaultWorld);
		currentScene->Load();

		return true;
	}
};