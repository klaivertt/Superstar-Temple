#include "DuelTrigger.hpp"

#include "Code/States/Game/Game.hpp"
#include "Tools/Physics/Physics.hpp"

DuelTrigger::DuelTrigger(GameData* _data, Vec2 _pos, Vec2 _size, Game* _game) : Actor(_data)
{
	game = _game;
	position = _pos;
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, _size }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0.f, 0.f), 0.f, _size });
	z = 10.f;
}

void DuelTrigger::OnTriggerEnter(ColEvent _col)
{
	if (isTriggered || game == nullptr || _col.other == nullptr)
	{
		return;
	}

	Actor* actor = dynamic_cast<Actor*>(_col.other);
	if (actor == nullptr || actor->GetClassName() != "Player")
	{
		return;
	}

	isTriggered = true;
	game->StartDuel();
}