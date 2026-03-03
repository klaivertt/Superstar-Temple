#pragma once

#include "common.hpp"

class EditorGrid
{
private:
    float gridSize;
    sf::Color gridColor;
    sf::Color axisColor;
    bool visible;
    bool snapEnabled;
    
    std::vector<sf::Vertex> gridLines;
    
    void GenerateGrid(const sf::View& _view);

public:
    EditorGrid();
    
    void Draw(sf::RenderTarget* _render, const sf::View& _view);
    
    void SetGridSize(float _size) 
    { 
        gridSize = _size;
    }

    float& GetGridSize() 
    { 
        return gridSize; 
	}

    float GetGridSize() const 
    {
        return gridSize; 
    }
    
    void SetVisible(bool _visible) 
    {
        visible = _visible; 
    }
    bool IsVisible() const 
    {
        return visible; 
    }
    
    void SetSnapEnabled(bool _enabled) 
    {
        snapEnabled = _enabled;
    }
    bool IsSnapEnabled() const
    { 
        return snapEnabled;
    }
    
    // Snap une position à la grille
    sf::Vector2f SnapToGrid(const sf::Vector2f& _position) const;
};