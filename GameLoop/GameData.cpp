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


size_t GetMemoryUsage(void)
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize / 1024 / 1024; // RAM utilisée en MB
}
