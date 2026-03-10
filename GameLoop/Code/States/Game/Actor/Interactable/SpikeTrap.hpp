#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

class SpikeTrap : public Interactable
{
private:
	Sprite sprite;
	float visualTimer = 0.f;

public:
	SpikeTrap(GameData* _data, Vec2 _pos);

	std::string GetClassName(void) override { return "SpikeTrap"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	virtual void OnInteract(Actor* _interactingActor) override;

	void CloseSpikeTrap();

	void SpikeDown();
	void SpikeUp();
	void CreateCollider();
	bool IsDangerous() const;


	bool isSpikeDown = false;
	bool isSpikeDesactivatedByPlayer = false;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	 virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
