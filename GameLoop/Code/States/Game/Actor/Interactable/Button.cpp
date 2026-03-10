#include "Button.hpp"

Button::Button(GameData* _data) : Interactable(_data)
{

}

void Button::Update(float _dt)
{

}

void Button::Draw(sf::RenderTarget* _render)
{

}

void Button::OnCollisionEnter(ColEvent _col)
{
}

void Button::OnCollisionExit(ColEvent _col)
{
}
<<<<<<< Updated upstream
=======

void Button::OnInteract(Actor* _interactingActor)
{
	ActivateTarget(_interactingActor);

	//Interactable::~Interactable();
}

bool Button::ReturnState(void)
{
	return isPressed;
}
>>>>>>> Stashed changes
