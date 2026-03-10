#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/Debug/Logger.hpp"
#include "Tools/Physics/Physics.hpp"

void Player::InitInputs()
{
	data->inputs->GetAxisDelegate("WalkForward")->Add(this, &Player::OnWalkForward);
	data->inputs->GetAxisDelegate("WalkBackward")->Add(this, &Player::OnWalkBackward);
	data->inputs->GetAxisDelegate("WalkLeft")->Add(this, &Player::OnWalkLeft);
	data->inputs->GetAxisDelegate("WalkRight")->Add(this, &Player::OnWalkRight);

	data->inputs->GetPressedDelegate("Interact")->Add(this, &Player::OnInteract);
}

Player::Player(GameData* _data): Actor(_data)
{
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(100, 100), 0.f, Vec2(50, 50) }, nullptr, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(50, 50) });
	b2Body_SetLinearDamping(body, 5.f);

#if _DEBUG
	_data->guiManager->RegisterWindow("Player", true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat("Player", "speed", "translation", &speed, 0.f, 35.f);
#endif

	InitInputs();
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

	dir.Normalize();
	Physics::ApplyForce(body, dir * speed);

	//Contact
	b2ContactEvents contactEvents = b2World_GetContactEvents(data->physicsWorld);
	b2Vec2 impulseVec = { 0.f, 0.f };
	CollisionPress(contactEvents, impulseVec);
	CollisionRelease(contactEvents, impulseVec);
	dir = Vec2( 0.f, 0.f );

}

void Player::Draw(sf::RenderTarget* _render)
{

<<<<<<< Updated upstream
=======
void Player::OnTriggerEnter(ColEvent _col)
{
	if (Interactable* interactable = dynamic_cast<Interactable*>(_col.other))
	{
		currentInteractable = interactable;

		// ce delegate permet au player de savoir quand l'interactable qu'il a en pointeur est détruit, pour éviter les pointeurs invalides
		// ce que cette fonction fait réelement, c'est de dire que quand l'interactable est détruit, le player met donc ensuite son pointeur currentInteractable à nullptr
		interactable->onDestroyed.Add([this](Actor*) { currentInteractable = nullptr; });
	}

	Logger::Debug("Trigger Enter with " + _col.other->GetClassName() + " !");
}

void Player::OnTriggerExit(ColEvent _col)
{
	if (currentInteractable == _col.other)
	{	
		currentInteractable = nullptr;
		Logger::Debug("Trigger Exit with " + _col.other->GetClassName() + " !");
	}
}

float Player::GetHealth() const
{
	// return the health in percentage
	return (health / maxHealth) * 100.f;
}

void Player::SetHealth(float _health)
{
}

float Player::GetMaxHealth() const
{
	return 0.0f;
>>>>>>> Stashed changes
}

void Player::UpdateIdle(float _dt)
{

}

void Player::UpdateRunning(float _dt)
{

}

void Player::OnWalkForward(Input _input)
{
	dir.y -= _input.axisValue;
}

void Player::OnWalkBackward(Input _input)
{
	dir.y -= _input.axisValue;
}

void Player::OnWalkLeft(Input _input)
{
	dir.x += _input.axisValue;
}

void Player::OnWalkRight(Input _input)
{
	dir.x += _input.axisValue;
}

void Player::OnInteract(Input _input)
{
	Logger::Log("Interact !");

}


void Player::CollisionPress(b2ContactEvents& events, b2Vec2& vec)
{
}

void Player::CollisionRelease(b2ContactEvents& events, b2Vec2& vec)
{
}