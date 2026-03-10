#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

enum DoorLevel
{
	DOOR_LEVEL_1,
	DOOR_LEVEL_2,

	NB_DOOR_LEVELS
};

class Door : public Interactable
{
private:
	DoorLevel doorLevel;
	Sprite sprite;

	bool isOpen = false;
public:
	Door(GameData* _data, Vec2 _pos, DoorLevel _level);
	void SetColor(const sf::Color& _color);

	std::string GetClassName(void) override { return "Box"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	virtual void OnInteract(Actor* _interactingActor) override;

	void ToggleDoor();
	void OpenDoor();
	void CloseDoor();

	void CreateCollider();
	          
	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
