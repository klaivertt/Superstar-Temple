#include "Game.hpp"

#include "Actor/BasicCube.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Animation.hpp"
#include "Tools/Map/Map.hpp"

Map* mappy;
sf::View view;
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
	
	// Les acteurs sont ajout�s automatiquement � la sc�ne donc pas
	// besoin de les g�rer :)

	Scene::Load();
	player = new Player(data);
	key = new Key(data, Vec2(400, 100));
	box = new Box(data, Vec2(500, 100));
	fireButton = new Button(data, Vec2(600, 100));
	spikeButton = new Button(data, Vec2(700, 100));
	button = new Button(data, Vec2(800, 100));
	fireTrap = new FireTrap(data, Vec2(600, 300));
	spikeTrap = new SpikeTrap(data, Vec2(700, 300));
	door = new Door(data, Vec2(800, 300));
	button->SetTarget(door);
	key->SetTarget(door);
	fireButton->SetTarget(fireTrap);
	spikeButton->SetTarget(spikeTrap);
	

	playerUi = new PlayerUi(data, player);

	// desactivate gravity 
	b2World_SetGravity(data->physicsWorld, { 0.f, 0.f });

	//temp ground
	/*groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, {Vec2(900, 500), 0.f, Vec2(1800, 50)}, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(1800, 50) });

	//temp wall
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
	*/

	mappy = new Map("Assets/Map/PlayMap");
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	////temp wall
	//groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	//groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
}

void Game::Update(float _dt)
{
	Scene::Update(_dt);
	b2Vec2 pPose = b2Body_GetPosition(player->body);
	view.setCenter(sf::Vector2f(pPose.x*64, -pPose.y*64 + SCREEN_H /4));
}

void Game::Draw(sf::RenderTarget* _render)
{
	_render->setView(view);
	mappy->Draw(*_render);
	Scene::Draw(_render);
	
}

void Game::Destroy(void)
{
	// Les acteurs sont aussi auto d�truit au changement de sc�ne pas besoin de les g�rer
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
