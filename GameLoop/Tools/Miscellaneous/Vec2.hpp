#pragma once

#include <string>

#include <box2d/box2d.h>
#include "SFML/System/Vector2.hpp"

class Vec2
{
public:
	float x = 0.f;
	float y = 0.f;

	Vec2(float _x, float _y);
	Vec2(void);
	Vec2(const sf::Vector2f& _vec);
	Vec2(const sf::Vector2i& _vec);
	Vec2(const sf::Vector2u& _vec);
	Vec2(const b2Vec2& _vec);
	Vec2(float _float);
	Vec2(int _int);

	void Normalize(void);
	float GetLenght(void);
	float GetLenghtSquared(void);
	float GetDistance(Vec2 _other);
	std::string ToString(void) const;
	void Lerp(Vec2 _value, Vec2 _desired, float _speed, float _dt);

	operator sf::Vector2f() const;
	operator b2Vec2() const;
	operator sf::Vector2i() const;
	operator sf::Vector2u() const;
	operator std::string() const;

	friend std::string operator + (const std::string& _str, const Vec2& _vec);
	friend std::string operator + (const Vec2& _vec, const std::string& _str);

	Vec2 operator + (Vec2 _other);
	Vec2 operator - (Vec2 _other);
	Vec2 operator * (Vec2 _other);
	Vec2 operator / (Vec2 _other);

	Vec2 operator + (float _other);
	Vec2 operator - (float _other);
	Vec2 operator * (float _other);
	Vec2 operator / (float _other);

	Vec2& operator = (const sf::Vector2f& _vec);
	Vec2 operator += (Vec2 _vec);
	Vec2 operator -= (Vec2 _vec);
	Vec2 operator *= (Vec2 _vec);
	Vec2 operator /= (Vec2 _vec);

	Vec2 operator += (float _float);
	Vec2 operator -= (float _float);
	Vec2 operator *= (float _float);
	Vec2 operator /= (float _float);

	Vec2 operator + (int _int);
	Vec2 operator - (int _int);
	Vec2 operator * (int _int);
	Vec2 operator / (int _int);

	Vec2 operator += (int _int);
	Vec2 operator -= (int _int);
	Vec2 operator *= (int _int);
	Vec2 operator /= (int _int);
};