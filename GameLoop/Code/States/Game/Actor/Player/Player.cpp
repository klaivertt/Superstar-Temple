#include "Player.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/Debug/Logger.hpp"
#include "../Interactable/Interactable.hpp"

namespace
{
	std::string BuildPlayerInputName(const std::string& _prefix, const std::string& _baseName)
	{
		if (_prefix.empty())
		{
			return _baseName;
		}

		return _prefix + _baseName;
	}
}

void Player::InitInputs()
{
	data->inputs->GetAxisDelegate(BuildPlayerInputName(inputPrefix, "WalkForward"))->Add(this, &Player::OnWalkForward);
	data->inputs->GetAxisDelegate(BuildPlayerInputName(inputPrefix, "WalkBackward"))->Add(this, &Player::OnWalkBackward);
	data->inputs->GetAxisDelegate(BuildPlayerInputName(inputPrefix, "WalkLeft"))->Add(this, &Player::OnWalkLeft);
	data->inputs->GetAxisDelegate(BuildPlayerInputName(inputPrefix, "WalkRight"))->Add(this, &Player::OnWalkRight);

	data->inputs->GetPressedDelegate(BuildPlayerInputName(inputPrefix, "Interact"))->Add(this, &Player::OnInteract);
}

Player::Player(GameData* _data, Vec2 _spawnPosition, const std::string& _inputPrefix, sf::Color _color) : Actor(_data)
{
	inputPrefix = _inputPrefix;
	debugWindowName = inputPrefix.empty() ? "Player" : "Player " + inputPrefix;
	fireParticles = ParticleSystem(
		{
			data->assets->GetTexture("Assets/Sprites/Spark.png"),
			1,
			1,
			false,
			sf::Color(255, 220, 80, 220),
			sf::Color(180, 30, 10, 0)
		},
		{ 0.f, -80.f },
		{ 28.f, 36.f }
	);
	bloodParticles = ParticleSystem(
		{
			data->assets->GetTexture("Assets/Sprites/Spark.png"),
			1,
			1,
			false,
			sf::Color(180, 10, 10, 220),
			sf::Color(70, 0, 0, 0)
		},
		{ 0.f, 10.f },
		{ 20.f, 20.f }
	);

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { _spawnPosition, 0.f, Vec2(0) }, this, true);
	//Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64.f) });
	Physics::CreateCircleCollider(body, { Vec2(0,0), 0.f, Vec2(0.f) }, 31.f);
	b2Body_SetLinearDamping(body, 5.f);

	sprite = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/Player.png"));
	sprite->SetColor(_color);
	sprite->SetOrigin(Vec2(0.5f));
#if _DEBUG
	_data->guiManager->RegisterWindow(debugWindowName, true, ImGuiWindowFlags_AlwaysAutoResize);
	_data->guiManager->AddSliderFloat(debugWindowName, "speed", "value", &speed, 0.f, 35.f);
	_data->guiManager->AddSliderFloat(debugWindowName, "maxHealth", "Max Value", &maxHealth, 0.f, 200.f);
	// Add a slider to change the health in percentage
	_data->guiManager->AddSliderFloat(debugWindowName, "health", "Health in %", &healthInPercent, 0.f, 100.f, [this](float _health, std::string _n) { SetHealthInPercent(_health); });
#endif

	InitInputs();
}

void Player::Update(float _dt)
{
	if (fire)
	{
		ApplyDamage(fireDamage * _dt, bloodParticleCooldown <= 0.f);

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
	UpdateParticles(_dt);

	dir = Vec2(0.f, 0.f);
}

void Player::Draw(sf::RenderTarget* _render)
{
	bloodParticles.draw(*_render);
	sprite->Draw(_render);
	fireParticles.draw(*_render);
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
	health = std::clamp(_health, 0.f, maxHealth);
}

float Player::GetMaxHealth() const
{
	return maxHealth;
}

void Player::ApplyFire(float _damagePerSecond, float _time)
{
	fire = true;
	fireTime = std::max(fireTime, _time);
	fireDamage = std::max(fireDamage, _damagePerSecond);
}

void Player::TakeDamage(float _damage)
{
	if (_damage <= 0.f)
	{
		return;
	}

	float previousHealth = health;
	health = std::clamp(health - _damage, 0.f, maxHealth);
	float damageTaken = previousHealth - health;

	if (damageTaken > 0.f)
	{
		EmitBloodParticles(std::max(3, static_cast<int>(std::ceil(damageTaken * 0.8f))), 1.f + damageTaken * 0.05f);
		bloodParticleCooldown = 0.1f;
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
		//currentInteractable = nullptr;
	}
}

void Player::SetPlayerDirection(void)
{
	// change the rotation of the player to look in the direction of the movement
	Physics::SetBodyRotation(body, atan2f(lastOrientation.x, lastOrientation.y) * static_cast<float>(180.f / M_PI));
}

void Player::UpdateParticles(float _dt)
{
	if (bloodParticleCooldown > 0.f)
	{
		bloodParticleCooldown -= _dt;
	}

	if (fire)
	{
		fireParticleTimer -= _dt;
		while (fireParticleTimer <= 0.f)
		{
			EmitFireParticles();
			fireParticleTimer += 0.05f;
		}
	}
	else
	{
		fireParticleTimer = 0.f;
	}

	sf::Vector2f particleVelocity = GetParticleVelocity();
	fireParticles.update(_dt, particleVelocity);
	bloodParticles.update(_dt, particleVelocity * 0.35f);
}

void Player::EmitFireParticles(void)
{
	sf::Vector2f spawnPosition = GetParticleSpawnPosition();
	for (int i = 0; i < 3; i++)
	{
		float angle = -90.f + static_cast<float>((rand() % 70) - 35);
		float intensity = 25.f + static_cast<float>(rand() % 70);
		fireParticles.addParticle(spawnPosition, 0.45f + static_cast<float>(rand() % 20) / 100.f, angle, intensity, { 0.5f, 1.2f, 3.5f, 1.6f });
	}
}

void Player::EmitBloodParticles(int _count, float _intensityMultiplier)
{
	sf::Vector2f spawnPosition = GetParticleSpawnPosition();
	for (int i = 0; i < _count; i++)
	{
		float angle = static_cast<float>(rand() % 360);
		float intensity = (70.f + static_cast<float>(rand() % 90)) * _intensityMultiplier;
		bloodParticles.addParticle(spawnPosition, 0.4f + static_cast<float>(rand() % 30) / 100.f, angle, intensity, { 0.9f, 4.f, 4.f, 1.8f });
	}
}

void Player::ApplyDamage(float _damage, bool _spawnBlood)
{
	if (_damage <= 0.f || health <= 0.f)
	{
		return;
	}

	float newHealth = std::clamp(health - _damage, 0.f, maxHealth);
	float damageTaken = health - newHealth;
	health = newHealth;

	if (_spawnBlood && damageTaken > 0.f)
	{
		EmitBloodParticles(std::max(1, static_cast<int>(std::ceil(damageTaken * 0.35f))), 0.7f);
		bloodParticleCooldown = 0.15f;
	}
}

sf::Vector2f Player::GetParticleSpawnPosition(void) const
{
	Vec2 bodyPosition = Physics::GetBodyPosition(body);
	return sf::Vector2f(bodyPosition.x, bodyPosition.y);
}

sf::Vector2f Player::GetParticleVelocity(void) const
{
	b2Vec2 velocity = b2Body_GetLinearVelocity(body);
	return sf::Vector2f(velocity.x * METERS_TO_PIXELS, -velocity.y * METERS_TO_PIXELS);
}

void Player::SetHealthInPercent(float _health)
{
	health = maxHealth * (_health / 100);
	Logger::Log("Health set to " + std::to_string(health) + " / " + std::to_string(maxHealth) + " (" + std::to_string(_health) + "%)");
}
