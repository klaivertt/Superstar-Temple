#pragma once

#include "../../GameLoop/Tools/Scene.hpp"
#include "../../GameLoop/Tools/SceneManager.hpp"
#include "../../GameLoop/GameData.hpp"
#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"

#include "Actors/Inspector.hpp"
#include "EditorCamera.hpp" 
#include "EditorGrid.hpp" 
#include "SelectionManager.hpp" 

class LevelEditor : public Scene
{
protected:
    Inspector* inspec = nullptr;
    Actor* currentInspectedActor = nullptr;

    EditorCamera* editorCamera = nullptr;
    EditorGrid* grid = nullptr;
    SelectionManager* selectionManager = nullptr;

    bool showGrid = false;
    bool snapToGrid = false;

    bool isMousePressed = false;
    sf::Vector2f lastMouseWorldPos;

    bool isDragging = false;
    sf::Vector2f dragStartPos;
    std::vector<sf::Vector2f> actorsInitialPositions;

public:
    LevelEditor(GameData* _data);
    ~LevelEditor();

    LevelEditor(const LevelEditor&) = delete;
    LevelEditor& operator=(const LevelEditor&) = delete;

    void Load(void) override;
    void UpdateHierarchy(void);
    void Update(float _dt) override;
    void Draw(sf::RenderTarget* _render) override;
    void Destroy(void) override;
    void HandleInput(sf::Event& _event);

private:
    void HandleMouseSelection(sf::Event& _event);
    Actor* GetActorAtPosition(const sf::Vector2f& _worldPos);
    void OpenInspector(Actor* _actor);
    void CloseInspector();
    bool IsActorStillValid(Actor* _actor);

    void StartDragging(const sf::Vector2f& _mousePos);
    void UpdateDragging(const sf::Vector2f& _mousePos);
    void EndDragging();
};