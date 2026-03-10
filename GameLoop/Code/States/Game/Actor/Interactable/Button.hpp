#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

#include <vector>

class Button : public Interactable
{
private:
	Sprite sprite;
	sf::Texture texture;
	bool isPressed = false;
	int pressingActorsCount = 0;
	std::vector<Interactable*> targets;

	void NotifyTargets();

public:
	Button(GameData* _data, Vec2 _pos);

	std::string GetClassName(void) override { return "Button"; }
	void SetColor(const sf::Color& _color);
	void AddTarget(Interactable* _target);
	virtual void SetTarget(Actor* _target) override;
	virtual void ClearTarget() override;

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	virtual void OnInteract(Actor* _interactingActor) override;

	bool ReturnState(void);

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
};
