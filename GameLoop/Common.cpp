#include "Common.hpp"

float GetVectorDistance(Vec2 _a, Vec2 _b)
{
	float dx = _b.x - _a.x;
	float dy = _b.y - _a.y;
	return sqrt((dx * dx) + (dy * dy));
}

sf::Vector2f NormalizeVector2f(sf::Vector2f _vector)
{
	float lenght = sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
	if (lenght <= 0.f)
	{
		return { 0.f, 0.f };
	}

	return { _vector.x / lenght, _vector.y / lenght };
}

float Clamp(float _value, float _min, float _max)
{
	if (_value > _max)
	{
		_value = _max;
	}
	else if (_value < _min)
	{
		_value = _min;
	}
	return _value;
}

int Clamp(int _value, int _min, int _max)
{
	if (_value > _max)
	{
		_value = _max;
	}
	else if (_value < _min)
	{
		_value = _min;
	}
	return _value;
}

float Lerp(float _value, float _desired, float _speed, float _dt)
{
	if (_speed < 0.f)
	{
		_speed = -_speed;
	}

	if (_value > _desired)
	{
		_value -= _speed * _dt;
		if (_value < _desired)
		{
			_value = _desired;
		}
	}
	else if (_value < _desired)
	{
		_value += _speed * _dt;
		if (_value > _desired)
		{
			_value = _desired;
		}
	}

	return _value;
}

sf::Vector2f Lerp(sf::Vector2f _value, sf::Vector2f _desired, float _speed, float _dt)
{
	if (_speed < 0.f)
	{
		_speed = -_speed;
	}

	float distX = (_value.x - _desired.x);
	float distY = (_value.y - _desired.y);

	float distance = sqrt(distX * distX + distY * distY);

	sf::Vector2f direction;
	if (distance > 0.f)
	{
		direction.x = distX / distance;
		direction.y = distY / distance;

		_value += direction * _speed * _dt;
	}

	return _value;
}

sf::Color LerpColor(const sf::Color& _a, const sf::Color& _b, float _t)
{
	_t = std::max(0.f, std::min(1.f, _t));
	return sf::Color(
		sf::Uint8(_a.r + (_b.r - _a.r) * _t),
		sf::Uint8(_a.g + (_b.g - _a.g) * _t),
		sf::Uint8(_a.b + (_b.b - _a.b) * _t),
		sf::Uint8(_a.a + (_b.a - _a.a) * _t)
	);
}

float VectorLength(sf::Vector2f _vector)
{
	return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
}

float RadianToDegrees(float _value)
{
	return _value * (float)(180 / M_PI);
}

float DegreesToRadian(float _value)
{
	return _value * (float)(M_PI / 180.f);
}

float DegToRad(float _value)
{
	return _value * (float)(M_PI / 180.f);
}

float RadToDeg(float _value)
{
	return _value * (float)(180 / M_PI);
}

sf::Vector2f RotateVec(sf::Vector2f _v, float _angleDeg)
{
	float rad = DegToRad(_angleDeg);
	float c = std::cos(rad);
	float s = std::sin(rad);
	return { _v.x * c - _v.y * s, _v.x * s + _v.y * c };
}

float Random(float _min, float _max)
{
	if (_min > _max)
	{
		std::swap(_min, _max);
	}

	int value = rand();
	return (((float)value / RAND_MAX) * (_max - _min)) + _min;
}

int Random(int _min, int _max)
{
	if (_min > _max)
	{
		std::swap(_min, _max);
	}

	if (_min == _max)
	{
		return _min;
	}

	return _min + (rand() % (_max - _min + 1));
}

Vec2 Random(Vec2 _min, Vec2 _max)
{
	if (_min.x > _max.x)
	{
		std::swap(_min.x, _max.x);
	}

	if (_min.y > _max.y)
	{
		std::swap(_min.y, _max.y);
	}

	int x = rand();
	int y = rand();

	Vec2 value;

	value.x = (((float)x / RAND_MAX) * (_max.x - _min.x)) + _min.x;
	value.y = (((float)y / RAND_MAX) * (_max.y - _min.y)) + _min.y;

	return value;
}

Vec2 GetRotationByPosition(Vec2 _position, float _parentRotation, Vec2 _parentScale)
{
	float rad = DegreesToRadian(_parentRotation);

	Vec2 position = { 0.f,0.f };

	// Appliquer la rotation du parent à la position relative de l'enfant
	float cosR = cos(rad);
	float sinR = sin(rad);

	float rotatedX = _position.x * cosR - _position.y * sinR;
	float rotatedY = _position.x * sinR + _position.y * cosR;

	// Appliquer la position, l'échelle et la rotation
	position.x = (rotatedX * _parentScale.x);
	position.y = (rotatedY * _parentScale.y);

	return position;
}



