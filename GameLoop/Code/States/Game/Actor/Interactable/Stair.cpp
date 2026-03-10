#include "Stair.hpp"

#include "Tools/Debug/Logger.hpp"
#include "Tools/Physics/Physics.hpp"

Vec2 ComputeExitOffset(const Vec2& _from, const Vec2& _to)
{
	Vec2 direction = _to - _from;
	if (direction.GetLenght() <= 0.001f)
	{
		return Vec2(64.f, 64.f);
	}

	direction.Normalize();
	return direction * 64.f;
}

Stair::Stair(GameData* _data, Vec2 _pos, int _stairId) : Interactable(_data)
{
	stairId = _stairId;
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/Stares.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));

	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	position = _pos;
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, textureSize }, this, true);
	Physics::CreateBoxTrigger(body, { Vec2(0.f, 0.f), 0.f, textureSize });

	sprite.SetPosition(_pos);
}

void Stair::SetColor(const sf::Color& _color)
{
	sprite.SetColor(_color);
}

void Stair::Update(float _dt)
{
	sprite.SetPosition(Physics::GetBodyPosition(body));
}

void Stair::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void Stair::OnCollisionEnter(ColEvent _col)
{
	
}

void Stair::OnCollisionExit(ColEvent _col)
{
}

void Stair::OnInteract(Actor* _interactingActor)
{
}

void Stair::OnTriggerEnter(ColEvent _col)
{
	if (_col.other != nullptr)
	{
		if (Actor* actor = dynamic_cast<Actor*>(_col.other))
		{
			Stair* targetStair = dynamic_cast<Stair*>(target);
			if (targetStair == nullptr || !b2Body_IsValid(targetStair->body))
			{
				Logger::Debug("Stair " + std::to_string(stairId) + " has no valid target.");
				return;
			}

			Vec2 sourcePosition = Physics::GetBodyPosition(body);
			Vec2 destinationPosition = Physics::GetBodyPosition(targetStair->body);
			Vec2 exitOffset = ComputeExitOffset(sourcePosition, destinationPosition);

			b2Body_SetLinearVelocity(actor->body, { 0.f, 0.f });
			Physics::SetBodyPosition(actor->body, destinationPosition + exitOffset);
			Logger::Debug("Stair " + std::to_string(stairId) + " used.");
		}
	}
}

void Stair::OnTriggerExit(ColEvent _col)
{
}
