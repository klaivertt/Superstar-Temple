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
	std::string sensorId = "";  // Identifier du shape (ex: "sensor_right", "sensor_left")
};

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
