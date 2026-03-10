#include "SpikeTrap.hpp"
#include "Tools/Physics/Physics.hpp"

#include "../Player/Player.hpp"

SpikeTrap::SpikeTrap(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/SpikeTrapOn.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { _pos, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });

	sprite.SetPosition(Physics::GetBodyPosition(body));
	
	isSpikeDown = false;
	spikeTimer = 0.f;
}

void SpikeTrap::Update(float _dt)
{

	if (isSpikeDesactivatedByPlayer)
	{
		if (!isSpikeDown)
		{
			SpikeDown();
			spikeTimer = 0.f;
		}
	}
	else
	{
		if (!isSpikeDown)
		{
			spikeTimer += _dt;
			if (spikeTimer >= SPIKE_DURATION)
			{
				SpikeDown();
				spikeTimer = 0.f;
			}
		}
		else
		{
			spikeTimer += _dt;
			if (spikeTimer >= SPIKE_COOLDOWN)
			{
				SpikeUp();
				spikeTimer = 0.f;
			}
		}
	}
}

void SpikeTrap::Draw(sf::RenderTarget* _render)
{
	sprite.Draw(_render);
}

void SpikeTrap::OnCollisionEnter(ColEvent _col)
{
	Player* player = dynamic_cast<Player*>(_col.other);

	if (player)
	{
		player->SetHealth(player->GetHealth() - 10.f);
	}
}

void SpikeTrap::OnCollisionExit(ColEvent _col)
{
}

void SpikeTrap::OnInteract(Actor* _interactingActor)
{
	//si bouton appuyé -> désactiver le piège
	CloseSpikeTrap();
}

void SpikeTrap::CloseSpikeTrap()
{
	isSpikeDesactivatedByPlayer = !isSpikeDesactivatedByPlayer;
}

void SpikeTrap::SpikeDown()
{
	if (b2Body_IsValid(body))
	{
		b2DestroyBody(body);
	}

	isSpikeDown = true;

	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/SpikeTrapOff.png"));
}

void SpikeTrap::SpikeUp()
{
	CreateCollider();

	isSpikeDown = false;

	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/SpikeTrapOn.png"));

}

void SpikeTrap::CreateCollider()
{
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, Vec2(64, 64) }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, Vec2(64, 64) });
}

void SpikeTrap::OnTriggerEnter(ColEvent _col)
{

}
