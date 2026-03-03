#pragma once
#ifndef WALKABLE_HPP
#define WALKABLE_HPP

#include "Common.hpp"

enum class WalkDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class Movement
{
public:
	Movement() = default;
	Movement(float& _speed, b2BodyId& _body);

	void WalkTo(WalkDirection _direction, float _dt);

private:
	float speed = 0.f;
	b2BodyId body = b2BodyId();
};


#endif