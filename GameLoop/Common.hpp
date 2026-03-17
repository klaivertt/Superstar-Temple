#ifndef COMMON__H
#define COMMON__H

#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <box2d/box2d.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

#include "Tools/Miscellaneous/Delegate.hpp"
#include "Tools/Miscellaneous/Array.hpp"
#include "Tools/Miscellaneous/Vec2.hpp"
#include "Tools/Overload/OperatorOverloading.hpp"

#define VSYNC true
#define FULLSCREEN false
#define MAX_FPS 500
#define KEY_REPEAT false

#define SCREEN_W 1920
#define SCREEN_H 1080
#define SCREEN_SIZE sf::Vector2f(SCREEN_W, SCREEN_H)

#define GAME_NAME "SuperStar Temple"
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

sf::Vector2f NormalizeVector2f(sf::Vector2f _vector);

// You can clamp your value between a min and max value
float Clamp(float _value, float _min, float _max);
// You can clamp your value between a min and max value
int Clamp(int _value, int _min, int _max);

// This function will lerp your value smoothly
// @param
// _speed : the speed is the desired value per second you want
float Lerp(float _value, float _desired, float _speed, float _dt);
sf::Vector2f Lerp(sf::Vector2f _value, sf::Vector2f _desired, float _speed, float _dt);
sf::Color LerpColor(const sf::Color& _a, const sf::Color& _b, float _t);

float VectorLength(sf::Vector2f _vector);

float RadianToDegrees(float _value);
float DegreesToRadian(float _value);

float DegToRad(float _value);
float RadToDeg(float _value);

sf::Vector2f RotateVec(sf::Vector2f _v, float _angleDeg);

float Random(float _min, float _max);
int Random(int _min, int _max);
Vec2 Random(Vec2 _min, Vec2 _max);

// This function will return the rotation of an actor based on its position, parent rotation and parent scale
Vec2 GetRotationByPosition(Vec2 _position, float _parentRotation, Vec2 _parentScale);
#endif