#pragma once

#include "Interactable.hpp"

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

class LevelDoor : public Interactable
{
public:
	Vec2 size = { 0.f, 0.f };
	float rotation = 0.f;
	Vec2 scale = { 1.f, 1.f };

	// Paramètres de la porte
	float moveSpeed = 128.f;

	bool isOpen = false;
	bool wasFullyClosed = false;
	bool hasCollider = true;

	// Animation
	int currentFrame = 0; 
	float animationSpeed = 1.f; 

	// Visuel
	Sprite* doorSprite;
	sf::Color colorClosed = sf::Color::White;
	sf::Color colorOpen = sf::Color::White;

public:
	LevelDoor(GameData* _data, Vec2 _pos);
	virtual ~LevelDoor();

	virtual std::string GetClassName(void) override
	{
		return "LevelDoor"; 
	}

	void ResetBox(void);
	void Open(void);
	void Close(void);
	virtual void Toggle(void) override;
	void SetDoorState(bool _open); // Nouvelle fonction pour contrôler l'état

	bool IsFullyOpen(void);
	bool IsFullyClosed(void);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
	virtual sf::FloatRect GetBounds() override;
};