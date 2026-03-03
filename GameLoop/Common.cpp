#include "Common.hpp"

#pragma region Vector2

Vec2::Vec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vec2::Vec2(void)
{
	x = 0.f;
	y = 0.f;
}

void Vec2::Normalize(void)
{
	float lenght = GetLenght();

	if (lenght == 0.f)
	{
		x = 0.f;
		y = 0.f;
		return;
	}

	x /= lenght;
	y /= lenght;
}

float Vec2::GetLenght(void)
{
	return sqrt((x * x) + (y * y));
}

float Vec2::GetLenghtSquared(void)
{
	return (x * x) + (y * y);
}

float Vec2::GetDistance(Vec2 _other)
{
	return GetVectorDistance(*this, _other);
}

std::string Vec2::ToString(void)
{
	std::string string = "Vec : " + std::to_string(x) + ", " + std::to_string(y);
	return string;
}

sf::Vector2f Vec2::ToSFML(void)
{
	return sf::Vector2f(x, y);
}

#pragma endregion

float GetVectorDistance(Vec2 _a, Vec2 _b)
{
	return sqrt(_a.x * _b.x + _a.y * _b.y);
}