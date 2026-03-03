#include "EditorGrid.hpp"
#include <cmath>

EditorGrid::EditorGrid()
    : gridSize(32.0f), 
      gridColor(sf::Color(70, 70, 70, 100)), 
      axisColor(sf::Color(120, 120, 120, 180)), 
      visible(false), 
      snapEnabled(false)
{
}

void EditorGrid::GenerateGrid(const sf::View& _view)
{
    gridLines.clear();
    
    sf::Vector2f center = _view.getCenter();
    sf::Vector2f size = _view.getSize();
    
    // Calculer les limites visibles
    float left = center.x - size.x / 2.f;
    float right = center.x + size.x / 2.f;
    float top = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;
    
    // Lignes verticales
    float startX = std::floor(left / gridSize) * gridSize;
    for (float x = startX; x <= right; x += gridSize)
    {
        // Axes principaux en couleur différente
        sf::Color color = (std::abs(x) < 0.1f) ? axisColor : gridColor;
        
        gridLines.push_back(sf::Vertex(sf::Vector2f(x, top), color));
        gridLines.push_back(sf::Vertex(sf::Vector2f(x, bottom), color));
    }
    
    // Lignes horizontales
    float startY = std::floor(top / gridSize) * gridSize;
    for (float y = startY; y <= bottom; y += gridSize)
    {
        // Axes principaux en couleur différente
        sf::Color color = (std::abs(y) < 0.1f) ? axisColor : gridColor;
        
        gridLines.push_back(sf::Vertex(sf::Vector2f(left, y), color));
        gridLines.push_back(sf::Vertex(sf::Vector2f(right, y), color));
    }
}

void EditorGrid::Draw(sf::RenderTarget* _render, const sf::View& _view)
{
    if (!visible) return;
    
    GenerateGrid(_view);
    
    if (!gridLines.empty())
    {
        _render->draw(&gridLines[0], gridLines.size(), sf::Lines);
    }
}

sf::Vector2f EditorGrid::SnapToGrid(const sf::Vector2f& _position) const
{
    if (!snapEnabled) 
        return _position;
    
    return sf::Vector2f(
        std::round(_position.x / gridSize) * gridSize,
        std::round(_position.y / gridSize) * gridSize
    );
}