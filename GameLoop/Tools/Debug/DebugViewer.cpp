#include "DebugViewer.hpp"

#include "../Miscellaneous/Text.hpp"

namespace Debug
{
	DebugViewer::DebugViewer()
	{
		fps = new Text(FontType::BOLD, { 0.f,0.f }, sf::Color::White, 22);
		maxFps = new Text(FontType::LIGHT, { 0.f,0.f }, sf::Color::White, 16);
		avgFps = new Text(FontType::LIGHT, { 0.f,0.f }, sf::Color::White, 16);
		minFps = new Text(FontType::LIGHT, { 0.f,0.f }, sf::Color::White, 16);
		memoryUsage = new Text(FontType::LIGHT, { 0.f,0.f }, sf::Color::White, 16);

		fps->SetPosition(15.f, 15.f);
		maxFps->SetPosition(15.f, 50.f);
		avgFps->SetPosition(15.f, 80.f);
		minFps->SetPosition(15.f, 110.f);
		memoryUsage->SetPosition(15.f, 140.f);

		showFPS = true;
		showDetails = true;

		fpsBackground.setFillColor(sf::Color(0, 0, 0, 100));
		UpdateSizeOfFpsBackground();
	}

	void DebugViewer::Update(float _dt)
	{
		UpdateFPS(_dt);
		clearTime += _dt;
		if (clearTime > CLEAR_FPS_TIME)
		{
			ClearData();
		}
	}

	void DebugViewer::Draw(sf::RenderTarget& _window)
	{
		if (showFPS)
		{
			_window.draw(fpsBackground);
			fps->Draw(_window);
		}
		if (showDetails)
		{
			maxFps->Draw(_window);
			avgFps->Draw(_window);
			minFps->Draw(_window);
			memoryUsage->Draw(_window);
		}
	}

	void DebugViewer::ToggleFPS(void)
	{
		if (!showFPS && !showDetails)
		{
			showFPS = true;
		}
		else if (showFPS && !showDetails)
		{
			showDetails = true;
		}
		else
		{
			showFPS = false;
			showDetails = false;
		}

		UpdateSizeOfFpsBackground();
	}

	void DebugViewer::UpdateFPS(float _dt)
	{
		if (showFPS)
		{
			int currentFps = static_cast<int>(1.f / _dt);
			fps->SetString("Fps : " + std::to_string(currentFps));

			// Update max fps
			if (currentFps > maxFpsValue)
			{
				maxFpsValue = currentFps;
				maxFps->SetString("Max Fps : " + std::to_string(maxFpsValue));
			}

			// Update min fps
			if (minFpsValue == 0 || currentFps < minFpsValue)
			{
				minFpsValue = currentFps;
				minFps->SetString("Min Fps : " + std::to_string(minFpsValue));
			}

			// Update average fps
			totalFps += currentFps;
			frameCount++;
			avgFpsValue = static_cast<int>(totalFps / frameCount);
			avgFps->SetString("Avg Fps : " + std::to_string(avgFpsValue));

			int memory = GetMemoryUsage();
			memoryUsage->SetString("Mem use : " + std::to_string(memory) + "MB");
		}
	}

	void DebugViewer::ClearData()
	{
		clearTime = 0.f;
		totalFps = 0.f;
		frameCount = 0;
		maxFpsValue = 0;
		minFpsValue = 0;
	}

	void DebugViewer::UpdateSizeOfFpsBackground()
	{
		if (showDetails)
		{
			fpsBackground.setSize(sf::Vector2f(250.f, 165.f));
		}
		else
		{
			fpsBackground.setSize(sf::Vector2f(120.f, 40.f));
		}

		fpsBackground.setPosition(10.f, 15.f);
	}
}
//void Debug::StartTimer(const std::string& id)
//{
//	timers[id] = std::chrono::high_resolution_clock::now();
//}
//
//double Debug::StopTimer(const std::string& id)
//{
//	auto it = timers.find(id);
//	if (it == timers.end())
//	{
//		return -1.0;
//	}
//	auto startTime = it->second;
//	auto endTime = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<double, std::milli> duration = endTime - startTime;
//	timers.erase(it);
//	return duration.count();
//}
