#pragma once

#include "Interactable.hpp"

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

class LevelLever : public Interactable
{
public:
	Interactable* linkedObject = nullptr; // Objet à contrôler (ex: une porte)

	Vec2 size = { 16.f, 48.f };
	float rotation = 0.f;
	Vec2 scale = { 1.f,1.f };
	// Paramètres du levier
	float minAngle = -45.f;  // Angle minimum (position OFF)
	float maxAngle = 45.f;   // Angle maximum (position ON)
	float currentAngle = 45.f;
	float targetAngle = 45.f;
	float rotationSpeed = 360.f; // Degrés par seconde
	
	bool isActivated = false;
	bool canInteract = true;
	float interactionRadius = 64.f;
	
	// Visuel
	Sprite* leverSprite;
	Sprite* leverBaseSprite;
	sf::Color colorOff = sf::Color::Red;
	sf::Color colorOn = sf::Color::Green;
	
	// Événement
	
public:
	LevelLever(GameData* _data, Vec2 _pos);
	virtual ~LevelLever();
	
	virtual std::string GetClassName(void) override { return "LevelLever"; }

	void ResetBox(void);
	void Activate(void);
	void Deactivate(void);
	virtual void Toggle(void) override;
	
	bool IsPlayerNearby(void);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
	virtual sf::FloatRect GetBounds() override;
};