#include "Key.hpp"
#include "Tools/Physics/Physics.hpp"
#include "Tools/AssetsManager.hpp"

Key::Key(GameData* _data) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Interactable/Key.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(400, 100), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });

	triggerRange = 100.f;

	SetTriggerRange(triggerRange);
}

void Key::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));

}

void Key::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Key::OnCollisionEnter(ColEvent _col)
{
}

void Key::OnCollisionExit(ColEvent _col)
{
}

void Key::OnInteract(Actor* _interactingActor)
{
	// Delete the key when interact with it
	Actor::~Actor();

}
