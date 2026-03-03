#ifndef COMMON__H
#define COMMON__H

#include <iostream>
#include <fstream>
#include <time.h>

#include <box2d/box2d.h>
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <stdlib.h>
#include <ctime>
#include <map>
#include <functional>
#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include <string>
#include "Tools/Miscellaneous/Array.hpp"
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>
#include "Tools/Overload/OperatorOverloading.hpp"
#include <chrono>


#define _USE_MATH_DEFINES
#include <math.h>
#include "Tools/Miscellaneous/Delegate.hpp"

#define VSYNC true
#define FULLSCREEN false
#define MAX_FPS 500
#define KEY_REPEAT false

#define SCREEN_W 1920
#define SCREEN_H 1080
#define SCREEN_SIZE sf::Vector2f(SCREEN_W, SCREEN_H)

#define GAME_NAME "SFML Engine"

#define GRAVITY 9.81f

DECLARE_DELEGATE(PropertyDelegate);

enum PropertiesType
{
	P_FLOAT,
	P_INT,
	P_VECTOR2,
	P_CONNECTED_VECTOR2,
	P_BOOL,
	P_OBJECT,
};

enum ObjectType
{
	OT_ACTOR,
	OT_SCENE,
	OT_MANAGER,
	OT_INTERACTABLE,
};

class Vec2
{
public:
	float x = 0.f;
	float y = 0.f;

	Vec2(float _x, float _y);
	Vec2(void);

	void Normalize(void);
	float GetLenght(void);
	float GetLenghtSquared(void);
	float GetDistance(Vec2 _other);
	std::string ToString(void);
	sf::Vector2f ToSFML(void);

	// Surchages

	// Vector 2
	Vec2 operator + (Vec2 _other)
	{
		return Vec2(x + _other.x, y + _other.y);
	}

	Vec2 operator - (Vec2 _other)
	{
		return Vec2(x - _other.x, y - _other.y);
	}

	Vec2 operator * (Vec2 _other)
	{
		return Vec2(x * _other.x, y * _other.y);
	}

	Vec2 operator / (Vec2 _other)
	{
		Vec2 vector(0.f, 0.f);

		if (_other.x)
		{
			vector.x = 0.f;
		}
		else
		{
			vector.x = x / _other.x;
		}

		if (_other.y)
		{
			vector.x = 0.f;
		}
		else
		{
			vector.y = y / _other.y;
		}

		return vector;
	}

	// Float
	Vec2 operator + (float _other)
	{
		return Vec2(x + _other, y + _other);
	}

	Vec2 operator - (float _other)
	{
		return Vec2(x - _other, y - _other);
	}

	Vec2 operator * (float _other)
	{
		return Vec2(x * _other, y * _other);
	}

	Vec2 operator / (float _other)
	{
		Vec2 vector(0.f, 0.f);

		if (_other == 0.f)
		{
			vector.x = 0.f;
			vector.y = 0.f;
		}
		else
		{
			vector.x = x / _other;
			vector.y = y / _other;
		}

		return vector;
	}

	Vec2 operator = (sf::Vector2f _vec)
	{

		return Vec2(_vec.x, _vec.y);
	}

	Vec2 operator += (Vec2 _vec)
	{
		x += _vec.x;
		y += _vec.y;
		return *this;
	}

	Vec2 operator -= (Vec2 _vec)
	{
		x -= _vec.x;
		y -= _vec.y;
		return *this;
	}
	
	Vec2 operator *= (Vec2 _vec)
		{
		x *= _vec.x;
		y *= _vec.y;
		return *this;
	}

	Vec2 operator /= (Vec2 _vec)
	{
		if (_vec.x)
		{
			x /= _vec.x;
		}
		else
		{
			x = 0.f;
		}
		if (_vec.y)
		{
			y /= _vec.y;
		}
		else
		{
			y = 0.f;
		}
		return *this;
	}
};

struct Property
{
	std::string name;
	PropertiesType type;
	ObjectType objectType;
	void* value = nullptr;
	PropertyDelegate callback;

	// X is the minimum, Y the maximum
	Vec2 range = { 0.f,0.f };
};

inline b2Vec2 ToB2Vec2(const Vec2& v)
{
	return b2Vec2{ v.x, v.y };
}

inline Vec2 ToVec2(const b2Vec2& v)
{
	return Vec2{ v.x, v.y };
}

inline Vec2 ToVec2(const sf::Vector2f& _vec)
{
	return Vec2(_vec.x, _vec.y);
}

inline sf::Vector2f ToSFML(const Vec2& _vec)
{
	return sf::Vector2f(_vec.x, _vec.y);
}

// Scalar product
float GetVectorDistance(Vec2 _a, Vec2 _b);

#endif