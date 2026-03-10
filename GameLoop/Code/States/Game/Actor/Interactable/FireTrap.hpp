#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

class FireTrap : public Interactable
{
private:
	Sprite sprite;
	float visualTimer = 0.f;

public:
	FireTrap(GameData* _data, Vec2 _pos);

	std::string GetClassName(void) override { return "FireTrap"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	virtual void OnInteract(Actor* _interactingActor) override;

	void CloseFireTrap();

	void FlameDown();
	void FlameUp();
	void CreateCollider();
	bool IsFlameActive() const;

	bool isFlameDown = false;
	bool isFlameDesactivatedByPlayer = false;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
