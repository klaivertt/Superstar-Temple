#pragma once

#include "../../GameData.hpp"
#include "Delegate.hpp"

class InputDelegate;
class SceneManager;

typedef struct Input
{
	// Delegates
	InputDelegate* delegatePressed = nullptr;
	InputDelegate* delegateReleased = nullptr;
	InputDelegate* axisDelegate = nullptr;


	// Key or Mouse Button
	sf::Keyboard::Key key = sf::Keyboard::Unknown;
	sf::Mouse::Button button = sf::Mouse::Button::ButtonCount;

	// Type
	bool mouse = false; // When true the code while use the mouse button
	bool ctrl = false; // WIP
	bool shift = false; // WIP
	bool axis = false; // When used the code while give you the axis value of the input every frames
	float axisValue = 0.f; // Don't touch

	bool isPressed = false; // Don't touch
	Vec2 mousePos = { 0.f,0.f }; // Don't touch
}Input;

DECLARE_DELEGATE_OneParam(InputDelegate, Input)

class Inputs
{
	std::map<std::string, Input> inputs;

	GameData* data = nullptr;

public:
	Inputs(GameData* _data);

	// Choose a new name for you new input, and send the struct Input you create
	void CreateInput(std::string _name, Input _input);
	// Don't call this
	void UpdateInputs(float _dt);
	// Get the struct Input of the input with the name you send
	const Input GetInputByName(std::string _name);
	// Modify the struct Input of the input with the name you send, with the new struct Input you send
	void ModifyInputByName(std::string _name, Input _input);

	// Get the delegate of the input with the name you send, and the type of delegate you want (Pressed, Released or Axis)
	InputDelegate* GetPressedDelegate(std::string _name);
	// Get the delegate of the input with the name you send, and the type of delegate you want (Pressed, Released ou Axis)
	InputDelegate* GetReleasedDelegate(std::string _name);
	// Get the delegate of the input with the name you send, and the type of delegate you want (Pressed, Released ou Axis)
	InputDelegate* GetAxisDelegate(std::string _name);
};