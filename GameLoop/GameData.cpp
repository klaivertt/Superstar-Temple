#include "GameData.hpp"

#include "Tools/SceneManager.hpp"

#include <Windows.h>
#include <Psapi.h>

GameData* GameData::instance = nullptr;
AssetsManager* GameData::assets = nullptr;

GameData* GameData::GetInstance(void)
{
	if (instance == nullptr)
	{
		instance = new GameData();
		assets = new AssetsManager();
	}
	return instance;
}

sf::Vector2f NormalizeVector2f(sf::Vector2f _vector)
{
	sf::Vector2f normalized = { 0.f,0.f };

	float lenght;
	if (abs(_vector.x) == 0.f || abs(_vector.y) == 0.f)
	{
		lenght = 1.f;
	}
	else
	{
		lenght = sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
	}

	normalized.x = _vector.x / lenght;
	normalized.y = _vector.y / lenght;

	return normalized;
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

Vec2 Lerp(Vec2 _value, Vec2 _desired, float _speed, float _dt)
{
	if (_speed < 0.f)
	{
		_speed = -_speed;
	}

	float distX = (_value.x - _desired.x);
	float distY = (_value.y - _desired.y);

	float distance = sqrt(distX * distX + distY * distY);

	Vec2 direction;
	if (distance > 0.f)
	{
		direction.x = distX / distance;
		direction.y = distY / distance;

		_value += direction * _speed * _dt;
	}

	return _value;
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

float Random(float _min, float _max)
{
	int value = rand();
	return (((float)value / RAND_MAX) * (_max - _min)) + _min;
}

int Random(int _min, int _max)
{
	int value = _min + (rand() % _max);
	return value;
}

Vec2 Random(Vec2 _min, Vec2 _max)
{
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

size_t GetMemoryUsage(void)
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize / 1024 / 1024; // RAM utilisée en MB
}
