#include "SelectionManager.hpp"
#include "Actors/LevelActor.hpp"

SelectionManager::SelectionManager()
    : isDrawingSelectionRect(false)
{
    selectionRect.setFillColor(sf::Color(100, 150, 255, 50));
    selectionRect.setOutlineColor(sf::Color(100, 150, 255, 200));
    selectionRect.setOutlineThickness(2.0f);
}

void SelectionManager::Select(Actor* _actor, bool _addToSelection)
{
    if (!_addToSelection)
    {
        DeselectAll();
    }
    
    if (!IsSelected(_actor))
    {
        selectedActors.push_back(_actor);
    }
}

void SelectionManager::Deselect(Actor* _actor)
{
    auto it = std::find(selectedActors.begin(), selectedActors.end(), _actor);
    if (it != selectedActors.end())
    {
        selectedActors.erase(it);
    }
}

void SelectionManager::DeselectAll()
{
    selectedActors.clear();
}

bool SelectionManager::IsSelected(Actor* _actor) const
{
    return std::find(selectedActors.begin(), selectedActors.end(), _actor) != selectedActors.end();
}

void SelectionManager::AddToSelection(Actor* _actor)
{
    if (!IsSelected(_actor))
    {
        selectedActors.push_back(_actor);
    }
}

void SelectionManager::RemoveFromSelection(Actor* _actor)
{
    Deselect(_actor);
}

void SelectionManager::StartSelectionRect(const sf::Vector2f& _position)
{
    isDrawingSelectionRect = true;
    selectionStart = _position;
    selectionEnd = _position;
}

void SelectionManager::UpdateSelectionRect(const sf::Vector2f& _position)
{
    selectionEnd = _position;
    
    sf::Vector2f topLeft(
        std::min(selectionStart.x, selectionEnd.x),
        std::min(selectionStart.y, selectionEnd.y)
    );
    
    sf::Vector2f size(
        std::abs(selectionEnd.x - selectionStart.x),
        std::abs(selectionEnd.y - selectionStart.y)
    );
    
    selectionRect.setPosition(topLeft);
    selectionRect.setSize(size);
}

void SelectionManager::EndSelectionRect(const std::vector<Actor*>& _allActors, bool _additive)
{
    if (!isDrawingSelectionRect) return;
    
    sf::FloatRect selBounds = selectionRect.getGlobalBounds();
    
    if (!_additive)
    {
        DeselectAll();
    }
    
    for (Actor* actor : _allActors)
    {
        // Obtenir les bounds de l'acteur (position + taille)
        sf::FloatRect actorBounds = actor->GetBounds();
        
        if (selBounds.intersects(actorBounds))
        {
            AddToSelection(actor);
        }
    }
    
    isDrawingSelectionRect = false;
}

void SelectionManager::DrawSelection(sf::RenderTarget* _render)
{
    for (Actor* actor : selectedActors)
    {
        sf::FloatRect bounds = actor->GetBounds();
        
        sf::RectangleShape outline;
        outline.setPosition(bounds.left, bounds.top);
        outline.setSize(sf::Vector2f(bounds.width, bounds.height));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Yellow);
        outline.setOutlineThickness(2.0f);
        
        _render->draw(outline);
        
        // Dessiner aussi des petits carrés aux coins
        float handleSize = 6.0f;
        sf::RectangleShape handle;
        handle.setSize(sf::Vector2f(handleSize, handleSize));
        handle.setFillColor(sf::Color::Yellow);
        handle.setOrigin(handleSize / 2.f, handleSize / 2.f);
        
        // Coin haut-gauche
        handle.setPosition(bounds.left, bounds.top);
        _render->draw(handle);
        
        // Coin haut-droit
        handle.setPosition(bounds.left + bounds.width, bounds.top);
        _render->draw(handle);
        
        // Coin bas-gauche
        handle.setPosition(bounds.left, bounds.top + bounds.height);
        _render->draw(handle);
        
        // Coin bas-droit
        handle.setPosition(bounds.left + bounds.width, bounds.top + bounds.height);
        _render->draw(handle);
    }
}

void SelectionManager::DrawSelectionRect(sf::RenderTarget* _render)
{
    if (isDrawingSelectionRect)
    {
        _render->draw(selectionRect);
    }
}