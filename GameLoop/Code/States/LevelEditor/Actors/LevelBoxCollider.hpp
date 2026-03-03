#pragma once

#include "../../GameLoop/GameData.hpp"

#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

class LevelBoxCollider : public Actor
{
public:
	Vec2 size = { 32.f, 32.f };
	float rotation = 0.f;
public:
	LevelBoxCollider(GameData* _data, Vec2 _pos);
	LevelBoxCollider(GameData* _data, std::string _name);
	virtual std::string GetClassName(void) override { return "LevelBoxCollider"; }

	void ResetBox(void);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;


	// Uncomment the function if you want to use them
	// virtual void OnCollisionEnter(ColEvent _col) override;
	// virtual void OnCollisionExit(ColEvent _col) override;
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};
