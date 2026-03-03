#include "Gameover.hpp"

Gameover::Gameover(GameData* _data) : Scene(_data)
{
}

void Gameover::Load(void)
{
	Scene::Load();
}

void Gameover::Update(float _dt)
{
	Scene::Update(_dt);
}

void Gameover::Draw(sf::RenderTarget* _render)
{
	Scene::Draw(_render);
}

void Gameover::Destroy(void)
{
	Scene::Destroy();
}
