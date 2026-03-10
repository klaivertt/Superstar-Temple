#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"

class Interactable : public Actor
{
protected:
<<<<<<< Updated upstream
	void SetTriggerRange(float _range, bool _centered = true);

	float triggerRange = 0.f;
=======
	void SetTriggerRange(float _range, Vec2 _decal = Vec2(0.f));
	bool ActivateTarget(Actor* _interactingActor = nullptr);

	float triggerRange = 0.f;
	Actor* target = nullptr;
	Actor* owner = nullptr;
	bool isCollidingWithTarget = false;
>>>>>>> Stashed changes
private:

public:
	Interactable(GameData* _data);
	~Interactable(void);

	virtual std::string GetClassName(void) override { return "Interactable"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
