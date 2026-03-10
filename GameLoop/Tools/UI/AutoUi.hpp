#pragma once

#include "../../Common.hpp"

class GameData;

class AutoUi
{
public:
	// Pointer to data
	GameData* data = nullptr;
	// The name of the ui element
	std::string name = ".";

public:
	AutoUi(GameData* _data);
	AutoUi(GameData* _data, std::string _name);
	virtual ~AutoUi(void);

	virtual std::string GetClassName(void) { return "Ui"; }

	virtual void Update(float _dt);
	virtual void Draw(sf::RenderTarget* _render);

	// Returns the screen-space bounding box of this UI element
	virtual sf::FloatRect GetBounds() const;
};