#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/Debug/Logger.hpp"


Player::Player(GameData* _data) : Actor(_data)
{
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(100, 100), 0.f, Vec2(50, 50) }, nullptr);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(50, 50) });

	movement = Movement(translationSpeed, body);

#if _DEBUG
	_data->guiManager->RegisterWindow("Player", true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat("Player", "speed", "translation", &translationSpeed, 0.f, 20);
#endif
}

Player::Player(GameData* _data, std::string _name) : Actor(_data, _name)
{

}

void Player::Update(float _dt)
{
	switch (state)
	{
	case State::IDLE:
		UpdateIdle(_dt);
		break;
	case State::WALK:
		UpdateRunning(_dt);
		break;
	}

	sf::Vector2f position = ToSFML(Physics::GetBodyPosition(body));
	//Logger::Debug(Logger::Vec2(position, "Player position : "), false);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movement.WalkTo(WalkDirection::RIGHT, _dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		movement.WalkTo(WalkDirection::LEFT, _dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		movement.WalkTo(WalkDirection::UP, _dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		movement.WalkTo(WalkDirection::DOWN, _dt);
	}
	else
	{
		movement.WalkTo(WalkDirection::NONE, _dt);
	}

	//Contact
	b2ContactEvents contactEvents = b2World_GetContactEvents(data->physicsWorld);
	b2Vec2 impulseVec = { 0.f, 0.f };
	CollisionPress(contactEvents, impulseVec);
	CollisionRelease(contactEvents, impulseVec);

}

void Player::Draw(sf::RenderTarget* _render)
{

}

void Player::UpdateIdle(float _dt)
{

}

void Player::UpdateRunning(float _dt)
{

}


void Player::CollisionPress(b2ContactEvents& events, b2Vec2& vec)
{
}

void Player::CollisionRelease(b2ContactEvents& events, b2Vec2& vec)
{
}