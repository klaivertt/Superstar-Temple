#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

#define SPIKE_DURATION 1.f
#define SPIKE_COOLDOWN 2.f

class SpikeTrap : public Interactable
{
private:
	Sprite sprite;

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


	float spikeTimer = 0.f;
	bool isSpikeDown = false;
	bool isSpikeDesactivatedByPlayer = false;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	 virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
