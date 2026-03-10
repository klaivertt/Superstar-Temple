#include "Game.hpp"

#include "Actor/BasicCube.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Animation.hpp"
#include "Tools/Map/Map.hpp"

Map* mappy;
sf::View view;

Game::Game(GameData* _data) : Scene(_data)
{
	// Ne faire spawn AUCUN acteurs ici, seulement dans Load()
}

void Game::Load(void)
{
	data->inputs->GetPressedDelegate("DEBUG")->Add(this, &Game::OnPressedDebugKey);
	// Créer les acteurs ici avec un new Actor() (ou de votre sous classe)
	// Les acteurs sont ajoutés automatiquement à la scène donc pas
	// besoin de les gérer :)

	Scene::Load();
	player = new Player(data);

	// desactivate gravity 
	b2World_SetGravity(data->physicsWorld, { 0.f, -1.f });

	//temp ground
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(900, 500), 0.f, Vec2(1800, 50) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(1800, 50) });

	//temp wall
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });

	mappy = new Map("Assets/Map/PlayMap");
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

void Game::Update(float _dt)
{
	Scene::Update(_dt);
	b2Vec2 pPose = b2Body_GetPosition(player->body);
	view.setCenter(sf::Vector2f(pPose.x*64, -pPose.y*64));
}

void Game::Draw(sf::RenderTarget* _render)
{
	_render->setView(view);
	mappy->Draw(*_render);
	Scene::Draw(_render);
	
}

void Game::Destroy(void)
{
	// Les acteurs sont aussi auto détruit au changement de scène pas besoin de les gérer
	Scene::Destroy();
}

void Game::OnPressedDebugKey(Input _input)
{
	//data->guiManager->SetDebugMode(!data->guiManager->isDegbugMode);
}