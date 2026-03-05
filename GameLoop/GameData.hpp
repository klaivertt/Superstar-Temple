#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "Common.hpp"

#include "Tools/AssetsManager.hpp"
#include "Tools/Miscellaneous/Actor.hpp"
#include "Tools/Debug/DebugViewer.hpp"
#include "Tools/Debug/Logger.hpp"
#include "Tools/Debug/ImGuiManager.hpp"
#include "Tools/UI/AutoUi.hpp"


class SceneManager;
class Inputs;

// Just a struct to store the collision event data
struct ColEvent
{
	Actor* other = nullptr;
	Vec2 normal = { 0.f, 0.f };
};

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

float VectorLength(sf::Vector2f _vector);

float RadianToDegrees(float _value);
float DegreesToRadian(float _value);

float Random(float _min, float _max);
int Random(int _min, int _max);
Vec2 Random(Vec2 _min, Vec2 _max);

// This function will return the rotation of an actor based on its position, parent rotation and parent scale
Vec2 GetRotationByPosition(Vec2 _position, float _parentRotation, Vec2 _parentScale);

// This function will return the current memory usage of the game in bytes
size_t GetMemoryUsage(void);

class GameData
{
protected:
	GameData(void) {};
	~GameData(void) {};

	GameData(GameData& other) = delete;
	void operator=(const GameData&) = delete;

	static GameData* instance;
public:
	static GameData* GetInstance(void);
	static AssetsManager* assets;
public:
	// Main variables
	sf::RenderWindow* render;
	sf::RenderTexture* renderTexture;
	sf::Clock clock;
	float dt = 0.f;

	bool isDebug = true;

	b2WorldDef defaultWorld = { 0 };
	b2WorldId physicsWorld = { 0 };
	int physicsQuality = 5;

	// Manager
	SceneManager* manager;
	Inputs* inputs;
	Debug::DebugViewer* debugViewer;
	Debug::GuiManager* guiManager;
};

#endif // !GAME_DATA_H
