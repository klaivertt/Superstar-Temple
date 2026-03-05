#include "SpikeTrap.hpp"
#include "Tools/Physics/Physics.hpp"

SpikeTrap::SpikeTrap(GameData* _data) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Map/SpikeTrap.png");
	sprite.SetTexture(&texture);
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(300, 200), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });
}

void SpikeTrap::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void SpikeTrap::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void SpikeTrap::OnCollisionEnter(ColEvent _col)
{
	//perte de vie du player
}

void SpikeTrap::OnCollisionExit(ColEvent _col)
{
}

void SpikeTrap::OnInteract(Actor* _interactingActor)
{
	//si bouton appuyé -> désactiver le piège
}
