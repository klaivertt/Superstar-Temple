#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"

class Interactable : public Actor
{
protected:
	float interactRange = 156.f;
	bool isGrabed = false;

public:

	Interactable(GameData* _data);

	virtual std::string GetClassName(void) override { return "Interactable"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	float GetInteractRange(void);
	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
