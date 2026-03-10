#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Tools/AssetsManager.hpp"
#include "Tools/Effects/Particle.hpp"


class Interactable;
class Player : public Actor
{

public:
	Interactable* currentInteractable = nullptr;

	enum State
	{
		IDLE,
		WALK
	};
	Player(GameData* _data, Vec2 _spawnPosition = Vec2(100.f, 100.f), const std::string& _inputPrefix = "", sf::Color _color = sf::Color(125, 200, 125));
	virtual std::string GetClassName(void) override { return "Player"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;


	// Uncomment the function if you want to use them
	// virtual void OnCollisionEnter(ColEvent _col) override;
	// virtual void OnCollisionExit(ColEvent _col) override;
	// virtual void OnCollisionHit(ColEvent _col) override;
	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;

	float GetHealth() const;
	void SetHealth(float _health);
	float GetMaxHealth() const;
	void TakeDamage(float _damage);
	void Respawn();

	void ApplyFire(float _damagePerSecond, float _time);
private:
	bool fire = false;
	float fireTime = 0.f;
	float fireDamage = 0.f;

	b2ShapeId collider = b2ShapeId();
	State state = State::IDLE;
	Vec2 dir = { 0.f, 0.f };
	float speed = 15.f;
	float health = 100.f;
	float maxHealth = 100.f;
	float healthInPercent = 100.f;
	Vec2 spawnPosition = { 100.f, 100.f };
	std::string inputPrefix;
	std::string debugWindowName = "Player";
	ParticleSystem fireParticles;
	ParticleSystem bloodParticles;
	float fireParticleTimer = 0.f;
	float bloodParticleCooldown = 0.f;

	Sprite* sprite;
	Vec2 lastOrientation = { 0.f, 0.f };

	//functions 
	void InitInputs();

	void UpdateIdle(float _dt);
	void UpdateRunning(float _dt);

	void OnWalkForward(Input _input);
	void OnWalkBackward(Input _input);
	void OnWalkLeft(Input _input);
	void OnWalkRight(Input _input);
	void OnInteract(Input _input);

	void SetPlayerDirection(void);
	void UpdateParticles(float _dt);
	void EmitFireParticles(void);
	void EmitBloodParticles(int _count, float _intensityMultiplier = 1.f);
	void ApplyDamage(float _damage, bool _spawnBlood = true);
	sf::Vector2f GetParticleSpawnPosition(void) const;
	sf::Vector2f GetParticleVelocity(void) const;

	void SetHealthInPercent(float _health);
};
