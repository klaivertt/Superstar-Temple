#include "Box.hpp"
#include "Tools/Physics/Physics.hpp"

Box::Box(GameData* _data) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Interactable/Box.png");
	sprite.SetTexture(&texture);
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::DYNAMIC, { Vec2(600, 100), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });

	triggerRange = 100.f;

	SetTriggerRange(triggerRange);
}

void Box::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void Box::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Box::OnCollisionEnter(ColEvent _col)
{
}

void Box::OnCollisionExit(ColEvent _col)
{
}

void Box::OnInteract(Actor* _interactingActor)
{	
	// Delete the key when interact with it
	Actor::~Actor();
}