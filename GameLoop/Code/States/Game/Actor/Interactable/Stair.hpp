#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

class Stair : public Interactable
{
private:
	Sprite sprite;
	int stairId = -1;

public:
	Stair(GameData* _data, Vec2 _pos, int _stairId = -1);

	std::string GetClassName(void) override { return "Stair"; }
	void SetColor(const sf::Color& _color);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;
	virtual void OnInteract(Actor* _interactingActor) override;

	// Note: to be notified when this interactable is destroyed, use
	// the inherited  onDestroyed  delegate (defined on Actor).

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	 virtual void OnTriggerEnter(ColEvent _col) override;
	 virtual void OnTriggerExit(ColEvent _col) override;
};