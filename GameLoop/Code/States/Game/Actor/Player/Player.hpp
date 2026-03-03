#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/Movement/Walk.hpp"

enum class PlayerState
{
	IDLE,
	WALK,
	JUMP,
	FALL
};

struct Player : public Actor
{
private:
	b2ShapeId collider = b2ShapeId();
	PlayerState m_state = PlayerState::IDLE;
	Movement movement;

private:
	// data values
	float translationSpeed = 7.f;
	float jumpForce = 50.f;

private:
	void UpdateIdle(float _dt);
	void UpdateRunning(float _dt);
	void UpdateJumping(float _dt);
	void UpdateFalling(float _dt);

	void CollisionPress(b2ContactEvents& events, b2Vec2& vec);
	void CollisionRelease(b2ContactEvents& events, b2Vec2& vec);

	bool m_touchedGround = false;
	bool m_touchedLeftWall = false;
	bool m_touchedRightWall = false;

public:
	Player(GameData* _data);
	Player(GameData* _data, std::string _name);
	virtual std::string GetClassName(void) override { return "Player"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;


	// Uncomment the function if you want to use them
	// virtual void OnCollisionEnter(ColEvent _col) override;
	// virtual void OnCollisionExit(ColEvent _col) override;
	// virtual void OnCollisionHit(ColEvent _col) override;
	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
	int groundContacts = 0;

	bool OnGround(void);
};
