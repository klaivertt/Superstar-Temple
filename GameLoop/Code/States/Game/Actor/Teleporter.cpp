#include "Teleporter.hpp"

#include "Interactable/Interactable.hpp"

Teleporter::Teleporter(GameData* _data, std::string _name) : Actor(_data, _name)
{
	sf::Texture* texture = data->assets->GetTexture("Assets/Sprites/Game/Map/Teleporter.png");

	first = new Sprite(texture, Vec2(0.5f, 0.5f));
	second = new Sprite(texture, Vec2(0.5f, 0.5f));

	secondTeleporterLocal.x = 200.f;
	secondTeleporterLocal.y = 50.f;

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { Vec2(500, 200), 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0,0), 0.f, Vec2(64, 64) }, TP_LEFT);
	Physics::CreateBoxTrigger(body, { secondTeleporterLocal, 0.f, Vec2(64, 64) }, TP_RIGHT);

	first->SetPosition(Physics::GetBodyPosition(body));
	first->SetColor(sf::Color::Red);
	second->SetPosition(Physics::GetBodyPosition(body) + secondTeleporterLocal);
	second->SetColor(sf::Color::Green);
}

void Teleporter::Update(float _dt)
{
	Actor::Update(_dt);
	first->SetRotation(first->GetRotation() + 60.f * _dt);
	second->SetRotation(first->GetRotation() + 60.f * _dt);
}

void Teleporter::Draw(sf::RenderTarget* _render)
{
	first->Draw(_render);
	second->Draw(_render);
	Actor::Draw(_render);
}

void Teleporter::OnCollisionEnter(ColEvent _col)
{
}

void Teleporter::OnCollisionExit(ColEvent _col)
{
}

void Teleporter::OnTriggerEnter(ColEvent _col)
{
	Interactable* obj = dynamic_cast<Interactable*>(_col.other);

	if (obj)
	{
		if (_col.sensorId == TP_LEFT)
		{
			Vec2 basePos = Physics::GetBodyPosition(body);

			Vec2 pos = Physics::GetBodyPosition(obj->body);
			Vec2 selfPos = Physics::GetBodyPosition(body);
			Vec2 decal = Vec2(selfPos - pos);
			Physics::SetBodyPosition(obj->body, basePos + secondTeleporterLocal + decal);
		}
		else
		{
			Vec2 basePos = Physics::GetBodyPosition(body);

			Vec2 pos = Physics::GetBodyPosition(obj->body);
			Vec2 selfPos = Physics::GetBodyPosition(body) + secondTeleporterLocal;
			Vec2 decal = Vec2(selfPos - pos);
			Physics::SetBodyPosition(obj->body, basePos + decal);
		}
	}
}

void Teleporter::OnTriggerExit(ColEvent _col)
{
}
