#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/Movement/Walk.hpp"

struct Player : public Actor
{

public:
	enum State
	{
		IDLE,
		WALK
	};
	Player(GameData* _data);
	Player(GameData* _data, std::string _name);
	virtual std::string GetClassName(void) override { return "Player"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;


	// Uncomment the function if you want to use them
	// virtual void OnCollisionEnter(ColEvent _col) override;
	// virtual void OnCollisionExit(ColEvent _col) override;
	// virtual void OnCollisionHit(ColEvent _col) override;
	//virtual void OnTriggerEnter(ColEvent _col) override;
	//virtual void OnTriggerExit(ColEvent _col) override;

private:
	b2ShapeId collider = b2ShapeId();
	State state = State::IDLE;
	Movement movement;

	float translationSpeed = 7.f;

	//functions 
	void UpdateIdle(float _dt);
	void UpdateRunning(float _dt);

	void CollisionPress(b2ContactEvents& events, b2Vec2& vec);
	void CollisionRelease(b2ContactEvents& events, b2Vec2& vec);
};
