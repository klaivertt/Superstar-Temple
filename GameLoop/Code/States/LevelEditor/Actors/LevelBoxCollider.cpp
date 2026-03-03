#include "LevelBoxCollider.hpp"

#include "Tools/Physics/Physics.hpp"

LevelBoxCollider::LevelBoxCollider(GameData* _data, Vec2 _pos) : Actor(_data)
{
	position = _pos;

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(_pos.x,_pos.y), 0.f, Vec2(100.f,100.f) }, nullptr);
	Physics::CreateBoxCollider(body, { Vec2(0.f,0.f), 0.f, size });

	Property prop;
	prop.name = "Size";
	prop.type = P_VECTOR2;
	prop.value = &size;
	prop.callback.Add(this, &LevelBoxCollider::ResetBox);
	properties.Add(prop);
	prop.name = "Rotation";
	prop.type = P_FLOAT;
	prop.value = &rotation;
	prop.callback.Add(this, &LevelBoxCollider::ResetBox);
	properties.Add(prop);
	data->log->Info("Created Box Collider at position " + position.ToString());
}

LevelBoxCollider::LevelBoxCollider(GameData* _data, std::string _name) : Actor(_data, _name)
{
	b2DestroyBody(body);
}

void LevelBoxCollider::ResetBox(void)
{
	b2DestroyBody(body);
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, Vec2(100.f,100.f) }, nullptr);
	Physics::CreateBoxCollider(body, { Vec2(0.f,0.f), rotation, size });
}

void LevelBoxCollider::Update(float _dt)
{

}

void LevelBoxCollider::Draw(sf::RenderTarget* _render)
{

}
