#pragma once

#include "../../GameLoop/GameData.hpp"
#include "Tools/Miscellaneous/Inputs.hpp"

class EditorCamera
{
private:
    sf::View* view;
    GameData* data;
    
    float zoomLevel;
    float minZoom;
    float maxZoom;
    float moveSpeed;

public:
    EditorCamera(GameData* _data);
    ~EditorCamera();
    
    void Update(float _dt);
    
    void MoveBy(sf::Vector2f _offset);
    
    sf::Vector2f ScreenToWorld(sf::Vector2i _screenPos) const;
    
    sf::View* GetView() { return view; }
    float GetZoom() const { return zoomLevel; }
    
    void Zoom(float _delta);
    
private:
    sf::Vector2f currentMovement;
  
    
    void OnMoveUp(Input _input);
    void OnMoveDown(Input _input);
    void OnMoveLeft(Input _input);
    void OnMoveRight(Input _input);
    void ZoomUp(Input _input);
    void ZoomDown(Input _input);
};