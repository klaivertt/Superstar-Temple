#ifndef DEBUG_VIEWER_H
#define DEBUG_VIEWER_H

#include "../../Common.hpp"

class Text;

namespace Debug
{

#define CLEAR_FPS_TIME 10.f
	// Classe d'utilitaires de debug (affichage FPS, logs, print d'objets simples, timers)
	class DebugViewer
	{
		// SFML texts / background
		Text* fps;
		Text* maxFps;
		Text* avgFps;
		Text* minFps;
		Text* memoryUsage;
		sf::RectangleShape fpsBackground;

		// toggles
		bool showFPS = true;
		bool showDetails = true;

		// fps stats
		int maxFpsValue = 0;
		int minFpsValue = 0;
		float totalFps = 0.f;
		int avgFpsValue = 0;
		int frameCount = 0;
		float clearTime = 0.f;

	public:
		DebugViewer();
		// Initialisation, update and draw (noms raccourcis)
		void Update(float dt);
		void Draw(sf::RenderTarget& window);

		// FPS
		void ToggleFPS(); // bascule l'affichage FPS

	private:
		void UpdateFPS(float _dt);
		void ClearData();
		void UpdateSizeOfFpsBackground();
	};
}
#endif // DEBUG_VIEWER_H