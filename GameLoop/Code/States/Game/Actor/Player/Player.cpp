#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"

Player::Player(GameData* _data) : Actor(_data)
{
	// c merdique vos mieux tout recommencer.

	// je suis fatigué de faire du code de merde, je vais faire du code de merde pour le fun, et après je recommencerai le code proprement, parce que là c'est vraiment de la merde mais ca restera de la merde, mais c'est pas grave, parce que c'est de la merde, la merde c'est la vie, la vie c'est de la merde. En dirais une ref a shrek mais non, shrek c'est de la merde, et la merde c'est pas bon, la merde ça pue, la merde ça rend malade, la merde c'est pas bon pour la santé, comme shrek. Je me souviens meme plus du but de ce message a la base facon personne va le lui jusqu'ici. Ah moin que tu sois une merde O_O

	//Shape
	{
		Physics::Transform transform;
		transform.position = Vec2(900, 0);
		transform.size = Vec2(50.f, 100.f);
		transform.rotation = 0.f;

		this->body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, transform, this);

		transform = Physics::Transform();
		transform.size = Vec2(50.f, 100.f);
	}
	const float groundCollisionRadius = 15.f;

	//Ground collider
	{
		Physics::Transform transform;
		transform.position = Vec2(0, 0);
		transform.size = Vec2(50.f, 100.f);
		transform.rotation = 0.f;

		b2ShapeId groundCollision = Physics::CreateCircleCollider(this->body, transform, groundCollisionRadius);
		Physics::SetBodyFixedRotation(this->body, true);
		Physics::SetShapePreset(groundCollision, Physics::CollisionPreset::PLAYER);
	}

	//Ground sensor
	{
		Physics::Transform transform;
		transform.size = Vec2(3.f, 3);
		transform.rotation = 90.f;
		transform.position = Vec2(0.f, groundCollisionRadius);
		b2ShapeId groundSensor = Physics::CreateBoxTrigger(this->body, transform);
	}

	//body collider
	{
		Physics::Transform transform;
		transform.size = Vec2(25.f, 50);
		transform.rotation = 0.f;
		transform.position = Vec2(0.f, -groundCollisionRadius * 2);
		b2ShapeId bodyCollision = Physics::CreateBoxCollider(this->body, transform);
		Physics::SetBodyFixedRotation(this->body, true);
		Physics::SetShapePreset(bodyCollision, Physics::CollisionPreset::PLAYER);
	}


	//Inputs
	Input input;


	input.key = sf::Keyboard::D;
	this->data->inputs->CreateInput("WalkRight", input);

	//Walk
	this->movement = Movement(translationSpeed, this->body);


#if _DEBUG
	_data->guiManager->RegisterWindow("Player", true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat("Player", "speed", "translation", &translationSpeed, 0.f, 20);
	_data->guiManager->AddSliderFloat("Player", "speed", "jump", &jumpForce, 0.f, 1000.f);
#endif
}

Player::Player(GameData* _data, std::string _name) : Actor(_data, _name)
{

}

void Player::Update(float _dt)
{
	switch (this->m_state)
	{
	case PlayerState::IDLE:
		this->UpdateIdle(_dt);
		break;
	case PlayerState::WALK:
		this->UpdateRunning(_dt);
		break;
	case PlayerState::JUMP:
		this->UpdateJumping(_dt);
		break;
	case PlayerState::FALL:
		this->UpdateFalling(_dt);
		break;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		movement.WalkTo(WalkDirection::RIGHT, _dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		movement.WalkTo(WalkDirection::LEFT, _dt);
	}
	else
	{
		movement.WalkTo(WalkDirection::NONE, _dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (OnGround())
		{
			movement.Jump(jumpForce);
		}
	}

	//COntact
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

void Player::UpdateJumping(float _dt)
{

}

void Player::UpdateFalling(float _dt)
{

}

void Player::CollisionPress(b2ContactEvents& events, b2Vec2& vec)
{
}

void Player::CollisionRelease(b2ContactEvents& events, b2Vec2& vec)
{
}

#pragma region Collisions
void Player::OnTriggerEnter(ColEvent _col)
{
	groundContacts += 1;
}

void Player::OnTriggerExit(ColEvent _col)
{
	groundContacts -= 1;
}

bool Player::OnGround(void)
{
	return groundContacts > 0;
}

#pragma endregion