#pragma once

#include "../../GameLoop/GameData.hpp"

#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

struct Interactable : public Actor
{
private:
// Variables here
public:
Interactable(GameData* _data);
Interactable(GameData* _data, std::string _name);
virtual std::string GetClassName(void) override { return "Interactable"; }

virtual void Update(float _dt) override;
virtual void Draw(sf::RenderTarget* _render) override;
virtual void Toggle(void) {};

// Uncomment the function if you want to use them
// virtual void OnCollisionEnter(ColEvent _col) override;
// virtual void OnCollisionExit(ColEvent _col) override;
// virtual void OnCollisionHit(ColEvent _col) override;
// virtual void OnTriggerEnter(ColEvent _col) override;
// virtual void OnTriggerExit(ColEvent _col) override;
};
