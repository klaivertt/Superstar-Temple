#include "Inputs.hpp"

Inputs::Inputs(GameData* _data)
{
	data = _data;
}

void Inputs::CreateInput(std::string _name, Input _input)
{
	Input input = _input;
	if (input.delegatePressed != nullptr)
	{
		delete input.delegatePressed;
	}
	if (input.delegateReleased != nullptr)
	{
		delete input.delegateReleased;
	}
	if (input.axisDelegate != nullptr)
	{
		delete input.axisDelegate;
	}
	input.delegatePressed = new InputDelegate;
	input.delegateReleased = new InputDelegate;
	input.axisDelegate = new InputDelegate;
	input.isPressed = false;

	inputs[_name] = input;
}

void Inputs::UpdateInputs(float _dt)
{
	for (auto& [name, input] : inputs)
	{
		if (input.axis == false) // Gestion des touches évènementiels
		{
			if (input.mouse) // Gestion des boutons de la souris
			{
				if (input.isPressed)
				{
					if (sf::Mouse::isButtonPressed(input.button) == false)
					{
						// Appelle toutes les fonctions attaché au Delegate
						input.isPressed = false;
						input.mousePos = Vec2(sf::Mouse::getPosition(*data->render).x, sf::Mouse::getPosition(*data->render).y);
						input.delegateReleased->Broadcast(input);
					}
				}
				else
				{
					if (sf::Mouse::isButtonPressed(input.button) == true)
					{
						// Appelle toutes les fonctions attaché au Delegate
						input.isPressed = true;
						input.mousePos = Vec2(sf::Mouse::getPosition(*data->render).x, sf::Mouse::getPosition(*data->render).y);
						input.delegatePressed->Broadcast(input);
					}
				}
			}
			else // Gestion des touches du clavier
			{
				if (input.isPressed)
				{
					if (sf::Keyboard::isKeyPressed(input.key) == false)
					{
						// Appelle toutes les fonctions attaché au Delegate
						input.isPressed = false;
						input.delegateReleased->Broadcast(input);
					}
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(input.key) == true)
					{
						// Appelle toutes les fonctions attaché au Delegate
						input.isPressed = true;
						input.delegatePressed->Broadcast(input);
					}
				}
			}
		}
		else // Gestion des inputs de type axis (Envoyé donc toutes les frames)
		{
			float value = input.axisValue;
			value = 0.f;
			if (sf::Keyboard::isKeyPressed(input.key))
			{
				value = input.axisValue;
			}
			Input* newInput = new Input(input);
			newInput->axisValue = value;
			input.axisDelegate->Broadcast(*newInput);
			delete newInput;
		}
	}
}

const Input Inputs::GetInputByName(std::string _name)
{
	return inputs[_name];
}

void Inputs::ModifyInputByName(std::string _name, Input _input)
{
	inputs[_name] = _input;
}

InputDelegate* Inputs::GetPressedDelegate(std::string _name)
{
	return inputs[_name].delegatePressed;
}

InputDelegate* Inputs::GetReleasedDelegate(std::string _name)
{
	return inputs[_name].delegateReleased;
}

InputDelegate* Inputs::GetAxisDelegate(std::string _name)
{
	return inputs[_name].axisDelegate;
}
