#include "Vec2.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

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

Vec2::Vec2(const sf::Vector2f& _vec)
{
	x = _vec.x;
	y = _vec.y;
}

Vec2::Vec2(const sf::Vector2i& _vec)
{
	x = static_cast<float>(_vec.x);
	y = static_cast<float>(_vec.y);
}

Vec2::Vec2(const sf::Vector2u& _vec)
{
	x = static_cast<float>(_vec.x);
	y = static_cast<float>(_vec.y);
}

Vec2::Vec2(const b2Vec2& _vec)
{
	x = _vec.x;
	y = _vec.y;
}

Vec2::Vec2(float _float)
{
	x = _float;
	y = _float;
}

Vec2::Vec2(int _int)
{
	x = static_cast<float>(_int);
	y = static_cast<float>(_int);
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
	float dx = _other.x - x;
	float dy = _other.y - y;
	return sqrt((dx * dx) + (dy * dy));
}

std::string Vec2::ToString(void) const
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

Vec2::operator sf::Vector2f() const
{
	return sf::Vector2f(x, y);
}

Vec2::operator b2Vec2() const
{
	b2Vec2 value{};
	value.x = x;
	value.y = y;
	return value;
}

Vec2::operator sf::Vector2i() const
{
	return sf::Vector2i(static_cast<int>(x), static_cast<int>(y));
}

Vec2::operator sf::Vector2u() const
{
	return sf::Vector2u(static_cast<unsigned int>(x), static_cast<unsigned int>(y));
}

Vec2::operator std::string() const
{
	return ToString();
}

std::string operator + (const std::string& _str, const Vec2& _vec)
{
	return _str + _vec.ToString();
}

std::string operator + (const Vec2& _vec, const std::string& _str)
{
	return _vec.ToString() + _str;
}

Vec2 Vec2::operator + (Vec2 _other)
{
	return Vec2(x + _other.x, y + _other.y);
}

Vec2 Vec2::operator - (Vec2 _other)
{
	return Vec2(x - _other.x, y - _other.y);
}

Vec2 Vec2::operator * (Vec2 _other)
{
	return Vec2(x * _other.x, y * _other.y);
}

Vec2 Vec2::operator / (Vec2 _other)
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

Vec2 Vec2::operator + (float _other)
{
	return Vec2(x + _other, y + _other);
}

Vec2 Vec2::operator - (float _other)
{
	return Vec2(x - _other, y - _other);
}

Vec2 Vec2::operator * (float _other)
{
	return Vec2(x * _other, y * _other);
}

Vec2 Vec2::operator / (float _other)
{
	if (_other == 0.f)
	{
		return Vec2(0.f, 0.f);
	}

	return Vec2(x / _other, y / _other);
}

Vec2& Vec2::operator = (const sf::Vector2f& _vec)
{
	x = _vec.x;
	y = _vec.y;
	return *this;
}

Vec2 Vec2::operator += (Vec2 _vec)
{
	x += _vec.x;
	y += _vec.y;
	return *this;
}

Vec2 Vec2::operator -= (Vec2 _vec)
{
	x -= _vec.x;
	y -= _vec.y;
	return *this;
}

Vec2 Vec2::operator *= (Vec2 _vec)
{
	x *= _vec.x;
	y *= _vec.y;
	return *this;
}

Vec2 Vec2::operator /= (Vec2 _vec)
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

Vec2 Vec2::operator += (float _float)
{
	x += _float;
	y += _float;
	return *this;
}

Vec2 Vec2::operator -= (float _float)
{
	x -= _float;
	y -= _float;
	return *this;
}

Vec2 Vec2::operator *= (float _float)
{
	x *= _float;
	y *= _float;
	return *this;
}

Vec2 Vec2::operator /= (float _float)
{
	if (_float != 0.f)
	{
		x /= _float;
		y /= _float;
	}
	else
	{
		x = 0.f;
		y = 0.f;
	}

	return *this;
}

Vec2 Vec2::operator + (int _int)
{
	return Vec2(x + _int, y + _int);
}

Vec2 Vec2::operator - (int _int)
{
	return Vec2(x - _int, y - _int);
}

Vec2 Vec2::operator * (int _int)
{
	return Vec2(x * _int, y * _int);
}

Vec2 Vec2::operator / (int _int)
{
	if (_int == 0)
	{
		return Vec2(0.f, 0.f);
	}

	return Vec2(x / _int, y / _int);
}

Vec2 Vec2::operator += (int _int)
{
	x += _int;
	y += _int;
	return *this;
}

Vec2 Vec2::operator -= (int _int)
{
	x -= _int;
	y -= _int;
	return *this;
}

Vec2 Vec2::operator *= (int _int)
{
	x *= _int;
	y *= _int;
	return *this;
}

Vec2 Vec2::operator /= (int _int)
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