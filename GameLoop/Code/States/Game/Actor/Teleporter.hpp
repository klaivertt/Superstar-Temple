#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"

#define TP_LEFT "left tp"
#define TP_RIGHT "right tp"

class Teleporter : public Actor
{
private:
	Sprite* first;
	Sprite* second;
	Vec2 secondTeleporterLocal;
public:
Teleporter(GameData* _data, std::string _name = ".");
virtual std::string GetClassName(void) override { return "Teleporter"; }

virtual void Update(float _dt) override;
virtual void Draw(sf::RenderTarget* _render) override;


// Uncomment the function if you want to use them
 virtual void OnCollisionEnter(ColEvent _col) override;
 virtual void OnCollisionExit(ColEvent _col) override;
// virtual void OnCollisionHit(ColEvent _col) override;
 virtual void OnTriggerEnter(ColEvent _col) override;
 virtual void OnTriggerExit(ColEvent _col) override;
};
