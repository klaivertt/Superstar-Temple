#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/Debug/Logger.hpp"


Player::Player(GameData* _data) : Actor(_data)
{
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(100, 100), 0.f, Vec2(50, 50) }, nullptr, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(50, 50) });
	b2Body_SetLinearDamping(body, 5.f);

#if _DEBUG
	_data->guiManager->RegisterWindow("Player", true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat("Player", "speed", "translation", &speed, 0.f, 20);
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

	
	Vec2 dir = { 0.f, 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		dir.x += 1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		dir.x -= 1.f;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		dir.y += 1.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		dir.y -= 1.f;
	}
	dir.Normalize();
	
	Physics::ApplyForce(body, dir * speed);

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