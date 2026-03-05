#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/Debug/Logger.hpp"
#include "../Interactable/Interactable.hpp"

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
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(100, 100), 0.f, Vec2(50, 50) }, this, true);
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

	dir = Vec2( 0.f, 0.f );

	Actor::Update(_dt);
}

void Player::Draw(sf::RenderTarget* _render)
{

}

void Player::OnTriggerEnter(ColEvent _col)
{
	if (_col.other->GetClassName() == "Key")
	{
		Logger::Log("Trigger Enter with key !");
		currentInteractable = static_cast<Interactable*>(_col.other);
	}

	Logger::Debug("Trigger Enter with " + _col.other->GetClassName() + " !");
}

void Player::OnTriggerExit(ColEvent _col)
{
	// if collision exit with the current interactable, set it to nullptr
	if (currentInteractable == _col.other)
	{
		currentInteractable = nullptr;
	}
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
	if (currentInteractable)
	{
		currentInteractable->OnInteract(this);
		currentInteractable = nullptr;
	}
}