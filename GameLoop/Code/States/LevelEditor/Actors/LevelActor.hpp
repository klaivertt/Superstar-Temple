#pragma once

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

template <typename T>
class LevelActor : public Actor
{
private:
    // Variables here

public:
    LevelActor(GameData* _data, Vec2 _pos) : Actor(_data, _pos) 
    {

    }

    LevelActor(GameData* _data, std::string _name) : Actor(_data, _name)
    {
    }

    virtual std::string GetClassName(void) override { return "LevelActor"; }

    // Uncomment the function if you want to use them
    // virtual void OnCollisionEnter(ColEvent _col) override;
    // virtual void OnCollisionExit(ColEvent _col) override;
    // virtual void OnCollisionHit(ColEvent _col) override;
    // virtual void OnTriggerEnter(ColEvent _col) override;
    // virtual void OnTriggerExit(ColEvent _col) override;
};