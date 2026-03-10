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

Player::Player(GameData* _data, Vec2 _pos) : Actor(_data)
{
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { _pos, 0.f, Vec2(0) }, this, true);
	//Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64.f) });
	Physics::CreateCircleCollider(body, { Vec2(0,0), 0.f, Vec2(0.f) }, 31.f);
	b2Body_SetLinearDamping(body, 5.f);

	sprite = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/Player.png"));
	sprite->SetColor(sf::Color(125, 200, 125));
	sprite->SetOrigin(Vec2(0.5f));
#if _DEBUG
	_data->guiManager->RegisterWindow("Player", true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat("Player", "speed", "value", &speed, 0.f, 35.f);
	_data->guiManager->AddSliderFloat("Player", "maxHealth", "Max Value", &maxHealth, 0.f, 200.f);
	// Add a slider to change the health in percentage
	_data->guiManager->AddSliderFloat("Player", "health", "Health in %", &healthInPercent, 0.f, 100.f, [this](float _health , std::string _n) { SetHealthInPercent(_health); });
#endif

	InitInputs();
}

void Player::Update(float _dt)
{
	if (fire)
	{
		health -= fireDamage * _dt;

		fireTime -= _dt;
		if (fireTime <= 0.f)
		{
			fire = false;
		}
	}

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

	if (dir.x != 0.f || dir.y != 0.f)
	{
		lastOrientation = dir;
		SetPlayerDirection();
	}

	Actor::Update(_dt);

	sprite->SetPosition(Physics::GetBodyPosition(body));
	sprite->SetRotation(Physics::GetBodyRotation(body));

	dir = Vec2(0.f, 0.f);
}

void Player::Draw(sf::RenderTarget* _render)
{
	sprite->Draw(_render);
}

void Player::OnTriggerEnter(ColEvent _col)
{
	if (Interactable* interactable = dynamic_cast<Interactable*>(_col.other))
	{
		currentInteractable = interactable;

		// ce delegate permet au player de savoir quand l'interactable qu'il a en pointeur est d�truit, pour �viter les pointeurs invalides
		// ce que cette fonction fait r�element, c'est de dire que quand l'interactable est d�truit, le player met donc ensuite son pointeur currentInteractable � nullptr
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
	health = _health;
}

float Player::GetMaxHealth() const
{
	return maxHealth;
}

void Player::ApplyFire(float _damagePerSecond, float _time)
{
	fire = true;
	fireTime = _time;
	fireDamage = _damagePerSecond;
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
		//currentInteractable = nullptr;
	}
}

void Player::SetPlayerDirection(void)
{
	// change the rotation of the player to look in the direction of the movement
	Physics::SetBodyRotation(body, atan2f(lastOrientation.x, lastOrientation.y) * static_cast<float>(180.f / M_PI));
}

void Player::SetHealthInPercent(float _health)
{
	health = maxHealth * (_health / 100);
	Logger::Log("Health set to " + std::to_string(health) + " / " + std::to_string(maxHealth) + " (" + std::to_string(_health) + "%)");
}
