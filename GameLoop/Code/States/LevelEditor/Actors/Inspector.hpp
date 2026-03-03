#pragma once

#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Sprite.hpp"

#define VALUE 2

class Inspector
{
private:
    Actor* actor = nullptr;
    GameData* data = nullptr;

    bool actorIsValid = false;

public:
    Inspector(GameData* _data, Actor* _actor);
    ~Inspector(void);

    Inspector(const Inspector&) = delete;
    Inspector& operator=(const Inspector&) = delete;

    void Update(float _dt);
    void Draw(sf::RenderTarget* _render);

    Actor* GetInspectedActor() { return actorIsValid ? actor : nullptr; }
    bool IsActorValid() const { return actorIsValid && actor != nullptr; }
    void InvalidateActor() { actorIsValid = false; actor = nullptr; }

private:
    void LoadObjectProperties(void);

    void CreateFloatProperty(Property _property, int _i);
    void CreateIntProperty(Property _property, int _i);
    void CreateVector2Property(Property _property, int _i);
    void CreateConnectedVector2Property(Property _property, int _i);
    void CreateBooleanProperty(Property _property, int _i);
    void CreateObjectProperty(Property _property, int _i);
};