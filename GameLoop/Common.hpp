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
	std::string ToString(void) const;
	void Lerp(Vec2 _value, Vec2 _desired, float _speed, float _dt);

	// sf::Vector2f -> Vec2
	Vec2(const sf::Vector2f& _vec)
	{
		x = _vec.x;
		y = _vec.y;
	}

	// sf::Vector2i -> Vec2
	Vec2(const sf::Vector2i& _vec)
	{
		x = static_cast<float>(_vec.x);
		y = static_cast<float>(_vec.y);
	}

	// b2Vec2 -> Vec2
	Vec2(const b2Vec2& _vec)
	{
		x = _vec.x;
		y = _vec.y;
	}

	Vec2(const float _float)
	{
		x = _float;
		y = _float;
	}

	// Vec2 -> sf::Vector2f
	operator sf::Vector2f() const
	{
		return sf::Vector2f(x, y);
	}

	// Vec2 -> b2Vec2 
	operator b2Vec2() const
	{
		return b2Vec2(x, y);
	}

	// Vec2 -> sf::Vector2i
	operator sf::Vector2i() const
	{
		return sf::Vector2i(static_cast<int>(x), static_cast<int>(y));
	}

	// Vec2 -> std::string
	operator std::string() const
	{
		return ToString();
	}

	friend std::string operator + (const std::string& _str, const Vec2& _vec);
	friend std::string operator + (const Vec2& _vec, const std::string& _str);

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
		if (_other.x != 0.f)
		{
			vector.x = x / _other.x;
		}

		if (_other.y != 0.f)
		{
			vector.y = y / _other.y;
		}

		return vector;
	}

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
		if (_other == 0.f)
		{
			return Vec2(0.f, 0.f);
		}
		return Vec2(x / _other, y / _other);
	}

	Vec2& operator = (const sf::Vector2f& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		return *this;
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
		if (_vec.x != 0.f)
		{
			x /= _vec.x;
		}
		else
		{
			x = 0.f;
		}
		if (_vec.y != 0.f)
		{
			y /= _vec.y;
		}
		else
		{
			y = 0.f;
		}
		return *this;
	}

	Vec2 operator += (float _float)
	{
		x += _float;
		y += _float;
		return *this;
	}

	Vec2 operator -= (float _float)
	{
		x -= _float;
		y -= _float;
		return *this;
	}

	Vec2 operator *= (float _float)
	{
		x *= _float;
		y *= _float;
		return *this;
	}

	Vec2 operator /= (float _float)
	{
		if (_float != 0.f) { x /= _float; y /= _float; }
		else { x = 0.f; y = 0.f; }
		return *this;
	}

	Vec2 operator + (int _int)
	{
		return Vec2(x + _int, y + _int);
	}

	Vec2 operator - (int _int)
	{
		return Vec2(x - _int, y - _int);
	}

	Vec2 operator * (int _int)
	{
		return Vec2(x * _int, y * _int);
	}

	Vec2 operator / (int _int)
	{
		if (_int == 0)
		{
			return Vec2(0.f, 0.f);
		}

		return Vec2(x / _int, y / _int);
	}

	Vec2 operator += (int _int)
	{
		x += _int;
		y += _int;
		return *this;
	}

	Vec2 operator -= (int _int)
	{
		x -= _int;
		y -= _int;
		return *this;
	}
	Vec2 operator *= (int _int)
	{
		x *= _int;
		y *= _int;
		return *this;
	}

	Vec2 operator /= (int _int)
	{
		if (_int != 0)
		{
			x /= _int;
			y /= _int;
		}
		else
		{
			x = 0.f;
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

// Scalar product
float GetVectorDistance(Vec2 _a, Vec2 _b);

#endif