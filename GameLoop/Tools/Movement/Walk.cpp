#include "Walk.hpp"
#include "Tools/Physics/Physics.hpp"

Movement::Movement(float& _speed, b2BodyId& _body)
{
	this->speed = _speed;
	this->body = _body;
}

void Movement::WalkTo(WalkDirection _direction, float _dt)
{
	b2Vec2 velocity = b2Body_GetLinearVelocity(body);

	switch (_direction)
	{
	case WalkDirection::LEFT:
		velocity.x = -speed;
		break;
	case WalkDirection::RIGHT:
		velocity.x = speed;
		break;
	case WalkDirection::UP:
		velocity.y = -speed;
		break;
	case WalkDirection::DOWN:
		velocity.y = speed;
		break;
	case WalkDirection::NONE:
		velocity.x = 0.f;
		break;
	}

	Physics::SetLinearVelocity(body, ToVec2(velocity));
}