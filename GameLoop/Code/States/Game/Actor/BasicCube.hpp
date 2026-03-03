#pragma once

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

class Cube : public Actor
{
private:
	sf::RectangleShape shape;
	Vec2 position;
	float lifeTime = 0.f;
public:
	Cube(GameData* _data);
	Cube(GameData* _data, std::string _name);
	virtual std::string GetClassName(void) override { return "Cube"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;
};