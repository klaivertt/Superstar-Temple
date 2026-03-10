#include "Game.hpp"

#include "Actor/BasicCube.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Animation.hpp"
#include "Tools/Map/Map.hpp"
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
	player = new Player(data, Vec2(100.f, 100.f), "", sf::Color(125, 200, 125));
	player2 = new Player(data, Vec2(220.f, 100.f), "P2", sf::Color(220, 170, 90));
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
	

	playerUi = new PlayerUi(data, player, Vec2(20.f, 20.f));
	player2Ui = new PlayerUi(data, player2, Vec2((SCREEN_W * 0.5f) + 20.f, 20.f));

	// desactivate gravity 
	b2World_SetGravity(data->physicsWorld, { 0.f, 0.f });

	//temp ground
	/*groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, {Vec2(900, 500), 0.f, Vec2(1800, 50)}, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(1800, 50) });

	//temp wall
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
	*/

	map = new Map("Assets/Map/PlayMap", &data->physicsWorld);
	playerOneView.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
	playerTwoView.setViewport(sf::FloatRect(0.5f, 0.f, 0.5f, 1.f));
	playerOneView.setSize(SCREEN_W * 0.5f, SCREEN_H);
	playerTwoView.setSize(SCREEN_W * 0.5f, SCREEN_H);
	separator = sf::RectangleShape(sf::Vector2f(2.f, static_cast<float>(SCREEN_H)));
	separator.setFillColor(sf::Color(20, 20, 20, 220));
	separator.setPosition((SCREEN_W * 0.5f) - 1.f, 0.f);

	////temp wall
	//groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	//groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
}

void Game::Update(float _dt)
{
	Scene::Update(_dt);
	b2Vec2 p1Pose = b2Body_GetPosition(player->body);
	b2Vec2 p2Pose = b2Body_GetPosition(player2->body);
	playerOneView.setCenter(sf::Vector2f(p1Pose.x * 64.f, -p1Pose.y * 64.f + SCREEN_H / 4.f));
	playerTwoView.setCenter(sf::Vector2f(p2Pose.x * 64.f, -p2Pose.y * 64.f + SCREEN_H / 4.f));
}

void Game::Draw(sf::RenderTarget* _render)
{
	camera = &playerOneView;
	//map->Draw(*_render);
	DrawWorld(_render);

	camera = &playerTwoView;
	//map->Draw(*_render);
	DrawWorld(_render);

	camera = nullptr;
	_render->setView(_render->getDefaultView());

	
	_render->draw(separator);

	DrawUi(_render);
	
}

void Game::Destroy(void)
{
	// Les acteurs sont aussi auto d�truit au changement de sc�ne pas besoin de les g�rer
	// destroy colliders

	groundBody = b2BodyId();
	groundShape = b2ShapeId();

	if (map)
	{
		delete map;
		map = nullptr;
	}

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
