#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

// Forward declaration
class Actor;

class SelectionManager
{
private:
    std::vector<Actor*> selectedActors;
    
    // Rectangle de sélection
    bool isDrawingSelectionRect;
    sf::Vector2f selectionStart;
    sf::Vector2f selectionEnd;
    sf::RectangleShape selectionRect;

public:
    SelectionManager();
    
    // Sélection simple
    void Select(Actor* _actor, bool _addToSelection = false);
    void Deselect(Actor* _actor);
    void DeselectAll();
    bool IsSelected(Actor* _actor) const;
    
    // Sélection multiple
    void AddToSelection(Actor* _actor);
    void RemoveFromSelection(Actor* _actor);
    std::vector<Actor*>& GetSelectedActors() { return selectedActors; }
    int GetSelectionCount() const { return selectedActors.size(); }
    
    // Rectangle de sélection
    void StartSelectionRect(const sf::Vector2f& _position);
    void UpdateSelectionRect(const sf::Vector2f& _position);
    void EndSelectionRect(const std::vector<Actor*>& _allActors, bool _additive);
    bool IsDrawingRect() const { return isDrawingSelectionRect; }
    
    // Rendu
    void DrawSelection(sf::RenderTarget* _render);
    void DrawSelectionRect(sf::RenderTarget* _render);
};