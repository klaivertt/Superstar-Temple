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

std::string Vec2::ToString (void) const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(3);
	oss << "Vec : (" << x << ", " << y << ")";
	return oss.str();
}

void Vec2::Lerp(Vec2 _value, Vec2 _desired, float _speed, float _dt)
{
	if (_speed < 0.f)
	{
		_speed = -_speed;
	}

	float distX = (_desired.x - this->x);
	float distY = (_desired.y - this->y);

	float distance = sqrt(distX * distX + distY * distY);

	Vec2 direction;
	if (distance > 0.f)
	{
		direction.x = distX / distance;
		direction.y = distY / distance;
		this->x += direction.x * _speed * _dt;
		this->y += direction.y * _speed * _dt;
	}
}

std::string operator + (const std::string& _str, const Vec2& _vec)
{
	return _str + _vec.ToString();
}

std::string operator + (const Vec2& _vec, const std::string& _str)
{
	return _vec.ToString() + _str;
}

#pragma endregion

float GetVectorDistance(Vec2 _a, Vec2 _b)
{
	float dx = _b.x - _a.x;
	float dy = _b.y - _a.y;
	return sqrt((dx * dx) + (dy * dy));
}