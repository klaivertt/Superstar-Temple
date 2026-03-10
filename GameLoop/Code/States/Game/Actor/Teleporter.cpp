#include "Teleporter.hpp"

Teleporter::Teleporter(GameData* _data, Vec2 _pos, int _teleporterId) : Actor(_data)
{
	teleporterId = _teleporterId;
	portal.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/Teleporter.png"));
	portal.SetOrigin(Vec2(0.5f, 0.5f));
	portal.SetPosition(_pos);

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, Vec2(64.f, 64.f) }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0, 0), 0.f, Vec2(64.f, 64.f) }, TP_TRIGGER);
	position = _pos;
	z = 0.05f;
}

bool Teleporter::CanTeleportActor(Actor* _actor)
{
	if (_actor == nullptr)
	{
		return false;
	}

	if (_actor->GetClassName() == "Player")
	{
		return false;
	}
	const std::unordered_map<Actor*, float>::iterator cooldownIt = recentlyTeleportedActors.find(_actor);
	return cooldownIt == recentlyTeleportedActors.end() || cooldownIt->second <= 0.f;
}

void Teleporter::MarkActorAsTeleported(Actor* _actor)
{
	if (_actor != nullptr)
	{
		recentlyTeleportedActors[_actor] = TELEPORTER_COOLDOWN;
	}
}

void Teleporter::SetTargetTeleporter(Teleporter* _teleporter)
{
	linkedTeleporter = _teleporter;
}

void Teleporter::SetColor(const sf::Color& _color)
{
	portal.SetColor(_color);
}

void Teleporter::Update(float _dt)
{
	Actor::Update(_dt);
	visualTimer += _dt;
	portal.SetPosition(Physics::GetBodyPosition(body));
	portal.SetRotation(portal.GetRotation() + 55.f * _dt);

	const float pulse = 0.92f + 0.08f * std::sin(visualTimer * 4.5f);
	portal.SetScale(Vec2(pulse, pulse));

	for (std::unordered_map<Actor*, float>::iterator it = recentlyTeleportedActors.begin(); it != recentlyTeleportedActors.end();)
	{
		it->second -= _dt;
		if (it->second <= 0.f)
		{
			it = recentlyTeleportedActors.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Teleporter::Draw(sf::RenderTarget* _render)
{
	sf::CircleShape glow(38.f);
	glow.setOrigin(38.f, 38.f);
	glow.setPosition(portal.GetPosition().x, portal.GetPosition().y);
	const sf::Color baseColor = portal.GetColor();
	glow.setFillColor(sf::Color(baseColor.r, baseColor.g, baseColor.b, 70));
	_render->draw(glow);

	portal.Draw(_render);
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
	Actor* actor = dynamic_cast<Actor*>(_col.other);
	if (linkedTeleporter == nullptr || actor == nullptr || actor == this || !b2Body_IsValid(actor->body))
	{
		return;
	}

	if (!CanTeleportActor(actor) || !linkedTeleporter->CanTeleportActor(actor))
	{
		return;
	}

	Vec2 entryPosition = Physics::GetBodyPosition(body);
	Vec2 exitPosition = Physics::GetBodyPosition(linkedTeleporter->body);
	Vec2 actorPosition = Physics::GetBodyPosition(actor->body);
	Vec2 localOffset = actorPosition - entryPosition;

	Physics::SetBodyPosition(actor->body, exitPosition + localOffset);
	MarkActorAsTeleported(actor);
	linkedTeleporter->MarkActorAsTeleported(actor);

	if (actor->GetClassName() == "Box")
	{
		Physics::SetLinearVelocity(actor->body, Vec2(0.f, 0.f));
	}
}

void Teleporter::OnTriggerExit(ColEvent _col)
{
}
