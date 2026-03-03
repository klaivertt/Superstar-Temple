#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "Interactable.hpp"

struct Key : public Interactable
{
private:
	Sprite sprite;
	sf::Texture texture;

public:
	Key(GameData* _data);

	std::string GetClassName(void) override { return "Key"; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
