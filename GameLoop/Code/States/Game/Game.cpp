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
#include "Code/States/Game/Actor/Interactable/Stair.hpp"
#include "Code/States/Game/Actor/Teleporter.hpp"

sf::Color GetLinkedInteractableColor(int _linkId)
{
	std::array<sf::Color, 15> palette =
	{
		sf::Color(227, 74, 51),
		sf::Color(52, 152, 219),
		sf::Color(46, 204, 113),
		sf::Color(241, 196, 15),
		sf::Color(230, 126, 34),
		sf::Color(155, 89, 182),
		sf::Color(26, 188, 156),
		sf::Color(231, 76, 60),
		sf::Color(52, 73, 94),
		sf::Color(22, 160, 133),
		sf::Color(243, 156, 18),
		sf::Color(142, 68, 173),
		sf::Color(39, 174, 96),
		sf::Color(41, 128, 185),
		sf::Color(192, 57, 43)
	};

	if (_linkId <= 0)
	{
		return sf::Color(220, 220, 220);
	}

	return palette[static_cast<size_t>(_linkId - 1) % palette.size()];
}

int GetPairedStairId(int _linkId)
{
	if (_linkId <= 0)
	{
		return -1;
	}

	return (_linkId % 2 == 0) ? (_linkId - 1) : (_linkId + 1);
}

int GetPairedTeleporterId(int _linkId)
{
	if (_linkId <= 0)
	{
		return -1;
	}

	return (_linkId > 10) ? (_linkId - 10) : (_linkId + 10);
}

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
	b2World_SetGravity(data->physicsWorld, { 0.f, 0.f });
	mappy = new Map("Assets/Map/PlayMap", &data->physicsWorld);

	Scene::Load();
	player = new Player(data, Vec2(mappy->m_playerSpawn[0]), "", sf::Color(125, 200, 125));
	player2 = new Player(data, Vec2(mappy->m_playerSpawn[1]), "P2", sf::Color(220, 170, 90));

	for (const Map::InteractableSpawn& boxSpawn : mappy->m_box)
	{
		new Box(data, Vec2(boxSpawn.position));
	}

	doorsById.clear();
	for (const Map::InteractableSpawn& doorSpawn : mappy->m_doorSpawns)
	{
		Door* spawnedDoor = new Door(data, Vec2(doorSpawn.position), DOOR_LEVEL_1);
		spawnedDoor->SetColor(GetLinkedInteractableColor(doorSpawn.linkId));

		if (doorSpawn.linkId > 0 && doorsById.find(doorSpawn.linkId) == doorsById.end())
		{
			doorsById[doorSpawn.linkId] = spawnedDoor;
		}
	}

	for (const Map::InteractableSpawn& keySpawn : mappy->m_keySpawns)
	{
		Key* spawnedKey = new Key(data, Vec2(keySpawn.position), KEY_LEVEL_1);
		spawnedKey->SetColor(GetLinkedInteractableColor(keySpawn.linkId));

		const std::map<int, Door*>::const_iterator doorIt = doorsById.find(keySpawn.linkId);
		if (doorIt != doorsById.end())
		{
			spawnedKey->SetTarget(doorIt->second);
			Logger::Debug("Key " + keySpawn.name + " linked to door " + doorIt->second->name);
		}
	}

	std::map<int, Stair*> stairsById;
	for (const Map::InteractableSpawn& stairSpawn : mappy->m_stairSpawns)
	{
		Stair* spawnedStair = new Stair(data, Vec2(stairSpawn.position), stairSpawn.linkId);
		spawnedStair->SetColor(GetLinkedInteractableColor(std::min(stairSpawn.linkId, GetPairedStairId(stairSpawn.linkId))));
		stairsById[stairSpawn.linkId] = spawnedStair;
	}

	for (const std::pair<const int, Stair*>& stairEntry : stairsById)
	{
		const int pairedId = GetPairedStairId(stairEntry.first);
		const std::map<int, Stair*>::const_iterator stairIt = stairsById.find(pairedId);
		if (stairIt != stairsById.end())
		{
			stairEntry.second->SetTarget(stairIt->second);
		}
	}

	std::map<int, std::vector<FireTrap*>> fireTrapsById;
	for (const Map::InteractableSpawn& fireTrapSpawn : mappy->m_fireTrap)
	{
		FireTrap* spawnedFireTrap = new FireTrap(data, Vec2(fireTrapSpawn.position));
		fireTrapsById[fireTrapSpawn.linkId].push_back(spawnedFireTrap);
	}

	std::map<int, std::vector<SpikeTrap*>> spikeTrapsById;
	for (const Map::InteractableSpawn& spikeSpawn : mappy->m_spike)
	{
		SpikeTrap* spawnedSpikeTrap = new SpikeTrap(data, Vec2(spikeSpawn.position));
		spikeTrapsById[spikeSpawn.linkId].push_back(spawnedSpikeTrap);
	}

	std::map<int, std::vector<Button*>> buttonsById;
	for (const Map::InteractableSpawn& buttonSpawn : mappy->m_button)
	{
		Button* spawnedButton = new Button(data, Vec2(buttonSpawn.position));
		spawnedButton->SetColor(GetLinkedInteractableColor(buttonSpawn.linkId));
		buttonsById[buttonSpawn.linkId].push_back(spawnedButton);
	}

	for (const std::pair<const int, std::vector<Button*>>& buttonEntry : buttonsById)
	{
		const int linkId = buttonEntry.first;

		const std::map<int, std::vector<FireTrap*>>::const_iterator fireTrapIt = fireTrapsById.find(linkId);
		if (fireTrapIt != fireTrapsById.end())
		{
			for (Button* buttonActor : buttonEntry.second)
			{
				for (FireTrap* fireTrapActor : fireTrapIt->second)
				{
					buttonActor->AddTarget(fireTrapActor);
				}
			}
		}

		const std::map<int, std::vector<SpikeTrap*>>::const_iterator spikeTrapIt = spikeTrapsById.find(linkId);
		if (spikeTrapIt != spikeTrapsById.end())
		{
			for (Button* buttonActor : buttonEntry.second)
			{
				for (SpikeTrap* spikeTrapActor : spikeTrapIt->second)
				{
					buttonActor->AddTarget(spikeTrapActor);
				}
			}
		}
	}

	std::map<int, Teleporter*> teleportersById;
	for (const Map::InteractableSpawn& teleporterSpawn : mappy->m_tp)
	{
		Teleporter* spawnedTeleporter = new Teleporter(data, Vec2(teleporterSpawn.position), teleporterSpawn.linkId);
		spawnedTeleporter->SetColor(GetLinkedInteractableColor(std::min(teleporterSpawn.linkId, GetPairedTeleporterId(teleporterSpawn.linkId))));
		teleportersById[teleporterSpawn.linkId] = spawnedTeleporter;
	}

	for (const std::pair<const int, Teleporter*>& teleporterEntry : teleportersById)
	{
		const int pairedId = GetPairedTeleporterId(teleporterEntry.first);
		const std::map<int, Teleporter*>::const_iterator teleporterIt = teleportersById.find(pairedId);
		if (teleporterIt != teleportersById.end())
		{
			teleporterEntry.second->SetTargetTeleporter(teleporterIt->second);
		}
	}


	playerUi = new PlayerUi(data, player, Vec2(20.f, 20.f));
	player2Ui = new PlayerUi(data, player2, Vec2((SCREEN_W * 0.5f) + 20.f, 20.f));

	// desactivate gravity 


	//temp ground
	/*groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, {Vec2(900, 500), 0.f, Vec2(1800, 50)}, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(1800, 50) });

	//temp wall
	groundBody = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 300), 0.f, Vec2(50, 600) }, nullptr);
	groundShape = Physics::CreateBoxCollider(groundBody, { Vec2(0,0), 0.f, Vec2(50, 600) });
	*/

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
	playerOneView.setCenter(sf::Vector2f(p1Pose.x * 64.f, -p1Pose.y * 64.f));
	playerTwoView.setCenter(sf::Vector2f(p2Pose.x * 64.f, -p2Pose.y * 64.f));
}

void Game::Draw(sf::RenderTarget* _render)
{
	camera = &playerOneView;
	_render->setView(*camera);
	if (mappy)
	{
		mappy->Draw(*_render);
	}
	DrawWorld(_render);

	camera = &playerTwoView;
	_render->setView(*camera);
	if (mappy)
	{
		mappy->Draw(*_render);
	}
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

	if (mappy)
	{
		delete mappy;
		mappy = nullptr;
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
