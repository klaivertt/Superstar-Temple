#include "Game.hpp"

#include "Actor/BasicCube.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Animation.hpp"

#include "Code/States/Game/Actor/Player/Player.hpp"
#include "Code/States/Game/Actor/Player/PlayerUi.hpp"
#include "Code/States/Game/Actor/Interactable/Key.hpp"
#include "Code/States/Game/Actor/Interactable/Box.hpp"
#include "Code/States/Game/Actor/Interactable/Button.hpp"
#include "Code/States/Game/Actor/Interactable/FireTrap.hpp"
#include "Code/States/Game/Actor/Interactable/SpikeTrap.hpp"
#include "Code/States/Game/Actor/Interactable/Door.hpp"

Game::Game(GameData* _data) : Scene(_data)
{
	// Ne faire spawn AUCUN acteurs ici, seulement dans Load()
}

void Game::Load(void)
{
	data->guiManager->AddButton("Game", "Scene", "Reset", [this](std::string _n) { ResetScene(); });
	data->inputs->GetPressedDelegate("DEBUG")->Add(this, &Game::OnPressedDebugKey);
	
	// Les acteurs sont ajoutés automatiquement à la scène donc pas
	// besoin de les gérer :)

	Scene::Load();
	player = new Player(data);
	key = new Key(data);
	box = new Box(data);
	button = new Button(data);
	fireTrap = new FireTrap(data);
	spikeTrap = new SpikeTrap(data);
	door = new Door(data);
	button->SetTarget(door);
	key->SetTarget(door);
	

	playerUi = new PlayerUi(data, player);

	// desactivate gravity 
	b2World_SetGravity(data->physicsWorld, { 0.f, 0.f });

	//temp ground
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(900, 500), 0.f, Vec2(1800, 50) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(1800, 50) });

	////temp wall
	//groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	//groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
}

void Game::Update(float _dt)
{
	Scene::Update(_dt);
}

void Game::Draw(sf::RenderTarget* _render)
{
	Scene::Draw(_render);
}

void Game::Destroy(void)
{
	// Les acteurs sont aussi auto détruit au changement de scène pas besoin de les gérer
	// destroy colliders

	groundBody = b2BodyId();
	groundShape = b2ShapeId();

	Scene::Destroy();
}

void Game::OnPressedDebugKey(Input _input)
{
	//data->guiManager->SetDebugMode(!data->guiManager->isDegbugMode);
}

void Game::ResetScene(void)
{
	this->Destroy();
	this->Load();
}
