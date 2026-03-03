#include "Inspector.hpp"

#include "Interactable/Interactable.hpp"
#include "Tools/Physics/Physics.hpp"
#include "LevelBoxCollider.hpp"

#include "../LevelEditor.hpp"

#define INSPECTOR_POS_RANGE 1920.f

Inspector::Inspector(GameData* _data, Actor* _actor)
    : data(_data), actor(_actor), actorIsValid(false)
{
    if (!data)
    {
        return;
    }

    if (!actor)
    {
        Logger::Error("Inspector: Cannot inspect null actor");
        return;
    }

    actorIsValid = true;

    ImGui::SetNextWindowSize(ImVec2(200.f, 100.f), ImGuiCond_FirstUseEver);
    data->guiManager->RegisterWindow("Inspector", true, ImGuiWindowFlags_AlwaysAutoResize);

    data->guiManager->AddInputText("Inspector", "0 Actor Info", "## Actor name",
        (char*)actor->name.c_str(), sizeof(char) * 256,
        [this](const char* _name)
        {
            if (IsActorValid())
            {
                actor->name = std::string(_name);
            }
        });

    data->guiManager->AddButton("Inspector", "0 Actor Info", "Save", [this](std::string _n)
        {
            if (!IsActorValid() || !data || !data->manager)
            {
                return;
            }

            LevelEditor* editor = dynamic_cast<LevelEditor*>(data->manager->currentScene);
            if (editor)
            {
                editor->UpdateHierarchy();
            }
        });

    data->guiManager->AddSliderFloat("Inspector", "1 Position", "X",
        &actor->position.x, -0.f, INSPECTOR_POS_RANGE,
        [this](float _pos, std::string _n)
        {
            if (!IsActorValid()) return;

            actor->position.x = _pos;
            if (B2_IS_NON_NULL(actor->body))
            {
                b2Body_SetTransform(actor->body,
                    b2Vec2(actor->position.x / METERS_TO_PIXELS,
                        -actor->position.y / METERS_TO_PIXELS),
                    b2Rot());
            }
        }, sf::Color::Red);

    data->guiManager->AddSliderFloat("Inspector", "1 Position", "Y",
        &actor->position.y, -0.f, INSPECTOR_POS_RANGE,
        [this](float _pos, std::string _n)
        {
            if (!IsActorValid())
            {
                return;
            }

            actor->position.y = _pos;
            if (B2_IS_NON_NULL(actor->body))
            {
                b2Body_SetTransform(actor->body,
                    b2Vec2(actor->position.x / METERS_TO_PIXELS,
                        -actor->position.y / METERS_TO_PIXELS),
                    b2Rot());
            }
        }, sf::Color::Yellow);

    data->guiManager->AddSliderFloat("Inspector", "1 Position", "Z",
        &actor->z, -10.f, 10.f,
        [this](float _pos, std::string _n)
        {
            if (!IsActorValid() || !data || !data->manager || !data->manager->currentScene)
            {
                return;
            }

            data->manager->currentScene->ordered = false;
            actor->z = _pos;
        }, sf::Color::Green);

    LoadObjectProperties();
}

Inspector::~Inspector(void)
{
    if (data && data->guiManager)
    {
        data->guiManager->RemoveWindow("Inspector");
    }

    actorIsValid = false;
    actor = nullptr;
}

void Inspector::Update(float _dt)
{
    // Vérifier si l'acteur existe toujours
    if (!IsActorValid())
    {
        return;
    }
}

void Inspector::Draw(sf::RenderTarget* _render)
{
    // Rien à dessiner, tout est dans ImGui
}

void Inspector::LoadObjectProperties(void)
{
    if (!IsActorValid())
    {
        return;
    }

    Array<Property>* properties = actor->GetProperties();
    if (!properties) 
    {
        return;
    }

    for (int i = 0; i < properties->Size(); i++)
    {
        PropertiesType type = properties->Get(i).type;
        int id = i + VALUE;

        switch (type)
        {
        case P_FLOAT:
            CreateFloatProperty(properties->Get(i), id);
            break;
        case P_INT:
            CreateIntProperty(properties->Get(i), id);
            break;
        case P_VECTOR2:
            CreateVector2Property(properties->Get(i), id);
            break;
        case P_CONNECTED_VECTOR2:
            CreateConnectedVector2Property(properties->Get(i), id);
            break;
        case P_BOOL:
            CreateBooleanProperty(properties->Get(i), id);
            break;
        case P_OBJECT:
            CreateObjectProperty(properties->Get(i), id);
            break;
        default:
            break;
        }
    }
}

void Inspector::CreateFloatProperty(Property _property, int _i)
{
    if (!_property.value)
    {
        return;
    }

    float* rot = static_cast<float*>(_property.value);
    std::string cat = std::to_string(_i) + " " + _property.name;

    data->guiManager->AddSliderFloat("Inspector", cat, "##" + _property.name,
        rot, 0.f, 360.f,
        [this](float _pos, std::string _n)
        {
            if (!IsActorValid()) 
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties)
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    prop.callback.Broadcast();
                }
            }
        }, sf::Color::Red);
}

void Inspector::CreateIntProperty(Property _property, int _i)
{
    // À implémenter si nécessaire
}

void Inspector::CreateVector2Property(Property _property, int _i)
{
    if (!_property.value)
    {
        return;
    }

    Vec2* size = static_cast<Vec2*>(_property.value);
    std::string cat = std::to_string(_i) + " " + _property.name;

    data->guiManager->AddSliderFloat("Inspector", cat, "##" + _property.name + "X",
        &size->x, _property.range.x, _property.range.y,
        [this](float _v, std::string _n)
        {
            if (!IsActorValid()) 
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties)
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    prop.callback.Broadcast();
                }
            }
        }, sf::Color::Red);

    data->guiManager->AddSliderFloat("Inspector", cat, "##" + _property.name + "Y",
        &size->y, _property.range.x, _property.range.y,
        [this](float _v, std::string _n)
        {
            if (!IsActorValid())
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties)
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    prop.callback.Broadcast();
                }
            }
        }, sf::Color::Yellow);
}

void Inspector::CreateConnectedVector2Property(Property _property, int _i)
{
    if (!_property.value) 
    {
        return;
    }

    Vec2* size = static_cast<Vec2*>(_property.value);
    std::string cat = std::to_string(_i) + " " + _property.name;

    data->guiManager->AddSliderFloat("Inspector", cat, "##" + _property.name + "X",
        &size->x, _property.range.x, _property.range.y,
        [this](float _v, std::string _n)
        {
            if (!IsActorValid())
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties) 
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    Vec2* value = static_cast<Vec2*>(prop.value);
                    if (value)
                    {
                        value->x = _v;
                        value->y = _v;
                        prop.callback.Broadcast();
                    }
                }
            }
        }, sf::Color::Red);
}

void Inspector::CreateBooleanProperty(Property _property, int _i)
{
    if (!_property.value) 
    {
        return;
    }

    bool* value = static_cast<bool*>(_property.value);
    std::string cat = std::to_string(_i) + " " + _property.name;

    data->guiManager->AddCheckbox("Inspector", cat, "##" + _property.name, value,
        [this](bool _v, std::string _n)
        {
            if (!IsActorValid())
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties) 
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    prop.callback.Broadcast();
                }
            }
        });
}

void Inspector::CreateObjectProperty(Property _property, int _i)
{
    if (!data || !data->manager || !data->manager->currentScene)  
    {
        return;
    }

    int size = 0;
    Actor** actors = nullptr;
    const char** names = nullptr;

    switch (_property.objectType)
    {
    case OT_ACTOR:
        size = data->manager->currentScene->GetAllActorsOfClass<Actor>().Size();
        if (size > 0)
        {
            actors = new Actor * [size];
            names = new const char* [size];

            for (int i = 0; i < size; i++)
            {
                actors[i] = data->manager->currentScene->GetAllActorsOfClass<Actor>().Get(i);
                names[i] = actors[i] ? actors[i]->name.c_str() : "null";
            }
        }
        break;

    case OT_INTERACTABLE:
        size = data->manager->currentScene->GetAllActorsOfClass<Interactable>().Size();
        if (size > 0)
        {
            actors = new Actor * [size];
            names = new const char* [size];

            for (int i = 0; i < size; i++)
            {
                actors[i] = data->manager->currentScene->GetAllActorsOfClass<Interactable>().Get(i);
                names[i] = actors[i] ? actors[i]->name.c_str() : "null";
            }
        }
        break;

    default:
        return;
    }

    if (size == 0 || !actors || !names)
    {
        // Nettoyer si allocation a échoué
        delete[] actors;
        delete[] names;
        return;
    }

    std::string cat = std::to_string(_i) + " " + _property.name;

    data->guiManager->AddComboBox("Inspector", cat, "##" + _property.name,
        (void**)actors, names, size,
        [this, actors, names](void* _v, std::string _n)
        {
            if (!IsActorValid())
            {
                return;
            }

            Array<Property>* properties = actor->GetProperties();
            if (!properties)
            {
                return;
            }

            for (int i = 0; i < properties->Size(); i++)
            {
                Property prop = properties->Get(i);
                if (std::to_string(i + VALUE) + " " + prop.name == _n)
                {
                    switch (prop.objectType)
                    {
                    case OT_ACTOR:
                    {
                        Actor** actor = static_cast<Actor**>(prop.value);
                        if (actor) *actor = static_cast<Actor*>(_v);
                        break;
                    }
                    case OT_INTERACTABLE:
                    {
                        Interactable** inter = static_cast<Interactable**>(prop.value);
                        if (inter) *inter = static_cast<Interactable*>(_v);
                        break;
                    }
                    default:
                        break;
                    }
                    prop.callback.Broadcast();
                }
            }

            // IMPORTANT : Nettoyer la mémoire allouée
            delete[] actors;
            delete[] names;
        });
}