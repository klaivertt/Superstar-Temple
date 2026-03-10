#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

class Key : public Interactable
{
private:
	Sprite sprite;

public:
	Key(GameData* _data, Vec2 _pos);
	b2ShapeId box;

	std::string GetClassName(void) override { return "Key"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	virtual void OnInteract(Actor* _interactingActor) override;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	 virtual void OnTriggerEnter(ColEvent _col) override;
	 virtual void OnTriggerExit(ColEvent _col) override;
};
