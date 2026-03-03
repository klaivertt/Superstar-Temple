#pragma once

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

class Elevator : public Actor
{
public:
	Vec2 size = { 16.f, 48.f };
	float rotation = 0.f;
	Vec2 scale = { 1.f,1.f };
	// Paramètres du levier

	bool isActivated = false;
	
	// Visuel
	Sprite* sprite;
	std::string spritePath = "Assets/Textures/Elevator.png";
	
	
public:
	Elevator(GameData* _data, Vec2 _pos);
	virtual ~Elevator();
	
	virtual std::string GetClassName(void) override { return "Elevator"; }

	void ResetBox(void);
	void Activate(void);
	void Deactivate(void);
	
	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
};