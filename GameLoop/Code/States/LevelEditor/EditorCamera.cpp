#include "EditorCamera.hpp"

EditorCamera::EditorCamera(GameData* _data)
    : data(_data), zoomLevel(1.0f), minZoom(0.2f), maxZoom(3.0f), 
      moveSpeed(400.0f), currentMovement(0.f, 0.f)
{
    view = new sf::View(sf::FloatRect(0.f, 0.f, SCREEN_W, SCREEN_H));

    data->inputs->GetAxisDelegate("WalkForward")->Add(this, &EditorCamera::OnMoveUp);
    data->inputs->GetAxisDelegate("WalkBackward")->Add(this, &EditorCamera::OnMoveDown);
    data->inputs->GetAxisDelegate("WalkLeft")->Add(this, &EditorCamera::OnMoveLeft);
    data->inputs->GetAxisDelegate("WalkRight")->Add(this, &EditorCamera::OnMoveRight);
    data->inputs->GetPressedDelegate("ZoomUp")->Add(this, &EditorCamera::ZoomUp);
    data->inputs->GetPressedDelegate("ZoomDown")->Add(this, &EditorCamera::ZoomDown);
}

EditorCamera::~EditorCamera()
{
    delete view;
}

void EditorCamera::Update(float _dt)
{
    // Appliquer le mouvement accumulé
    if (currentMovement.x != 0.f || currentMovement.y != 0.f)
    {
        sf::Vector2f movement = currentMovement * moveSpeed * _dt;
        
        // Vitesse x2 avec Shift
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            movement *= 2.0f;
        
        view->move(movement);
    }
    
    // Reset du mouvement pour le prochain frame
    currentMovement = sf::Vector2f(0.f, 0.f);
}

void EditorCamera::Zoom(float _delta)
{
    // Modifier le zoom
    zoomLevel += _delta;
    
    // Limites
    if (zoomLevel < minZoom) zoomLevel = minZoom;
    if (zoomLevel > maxZoom) zoomLevel = maxZoom;
    
    // Appliquer
    view->setSize(SCREEN_W * zoomLevel, SCREEN_H * zoomLevel);
}

void EditorCamera::MoveBy(sf::Vector2f _offset)
{
    view->move(_offset);
}


sf::Vector2f EditorCamera::ScreenToWorld(sf::Vector2i _screenPos) const
{
    return data->render->mapPixelToCoords(_screenPos, *view);
}

// Callbacks pour le système d'inputs
void EditorCamera::OnMoveUp(Input _input)
{
    currentMovement.y += _input.axisValue * 4;
}

void EditorCamera::OnMoveDown(Input _input)
{
    currentMovement.y += _input.axisValue * 4;
}

void EditorCamera::OnMoveLeft(Input _input)
{
    currentMovement.x += _input.axisValue * 4;
}

void EditorCamera::OnMoveRight(Input _input)
{
    currentMovement.x += _input.axisValue * 4;
}

void EditorCamera::ZoomUp(Input _input)
{
    Zoom(0.1f);
}

void EditorCamera::ZoomDown(Input _input)
{
    Zoom(-0.1f);
}
