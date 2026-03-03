#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "../../GameLoop/Tools/Scene.hpp"
#include "../../GameLoop/GameData.hpp"

struct Gameover : public Scene
{
protected:

public:
	Gameover(GameData* _scene);

	void Load(void) override;
	void Update(float _dt) override;
	void Draw(sf::RenderTarget* _render) override;
	void Destroy(void) override;
};

#endif // !GAMEOVER_H