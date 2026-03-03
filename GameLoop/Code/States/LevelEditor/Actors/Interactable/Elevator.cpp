#include "Elevator.hpp"
#include "Tools/Physics/Physics.hpp"

Elevator::Elevator(GameData* _data, Vec2 _pos) : Actor(_data)
{
	position = _pos;
}

Elevator::~Elevator()
{
}

void Elevator::ResetBox(void)
{
}

void Elevator::Activate(void)
{

}

void Elevator::Deactivate(void)
{
}

void Elevator::Update(float _dt)
{
}

void Elevator::Draw(sf::RenderTarget* _render)
{
}

void Elevator::OnTriggerEnter(ColEvent _col)
{
}

void Elevator::OnTriggerExit(ColEvent _col)
{
}