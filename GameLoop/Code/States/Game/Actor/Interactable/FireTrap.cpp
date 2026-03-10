#include "FireTrap.hpp"
#include "Tools/Physics/Physics.hpp"

#include "../Player/Player.hpp"

FireTrap::FireTrap(GameData* _data) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/FireTrap.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(600, 200), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });
}

void FireTrap::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void FireTrap::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void FireTrap::OnCollisionEnter(ColEvent _col)
{
	Player* player = dynamic_cast<Player*>(_col.other);

	if (player)
	{
		player->ApplyFire(3.f, 5.f);
	}
}

void FireTrap::OnCollisionExit(ColEvent _col)
{
}

void FireTrap::OnInteract(Actor* _interactingActor)
{
	//si bouton appuyé -> désactiver le piège
}
