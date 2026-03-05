#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Tools/AssetsManager.hpp"


class Interactable;
struct Player : public Actor
{

public:
	enum State
	{
		IDLE,
		WALK
	};
	Player(GameData* _data);
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

private:
	b2ShapeId collider = b2ShapeId();
	State state = State::IDLE;
	Vec2 dir = { 0.f, 0.f };
	float speed = 15.f;
	Interactable* currentInteractable = nullptr;
	float health = 100.f;
	float maxHealth = 100.f;

	//functions 
	void InitInputs();

	void UpdateIdle(float _dt);
	void UpdateRunning(float _dt);

	void OnWalkForward(Input _input);
	void OnWalkBackward(Input _input);
	void OnWalkLeft(Input _input);
	void OnWalkRight(Input _input);
	void OnInteract(Input _input);

	void SetHealthInPercent(float _health);
};
