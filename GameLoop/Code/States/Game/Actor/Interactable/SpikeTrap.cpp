#include "SpikeTrap.hpp"
#include "Tools/Physics/Physics.hpp"

#include "../Player/Player.hpp"

SpikeTrap::SpikeTrap(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/SpikeTrapOn.png"));
	sprite.SetOrigin(Vec2(0.5f, 0.5f));
	position = _pos;
	CreateCollider();
	sprite.SetPosition(Physics::GetBodyPosition(body));
	
	isSpikeDown = false;
}

void SpikeTrap::Update(float _dt)
{
	visualTimer += _dt;
	sprite.SetPosition(position);

	if (isSpikeDesactivatedByPlayer)
	{
		if (!isSpikeDown)
		{
			SpikeDown();
		}
	}
	else if (isSpikeDown)
	{
		SpikeUp();
	}

	if (isSpikeDown)
	{
		const sf::Uint8 alpha = static_cast<sf::Uint8>(95.f + 35.f * (0.5f + 0.5f * std::sin(visualTimer * 5.f)));
		sprite.SetColor(sf::Color(255, 255, 255, alpha));
	}
	else
	{
		sprite.SetColor(sf::Color::White);
	}
}

bool SpikeTrap::IsDangerous() const
{
	return !isSpikeDown;
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
		player->TakeDamage(10.f);
	}
}

void SpikeTrap::OnCollisionExit(ColEvent _col)
{
}

void SpikeTrap::OnInteract(Actor* _interactingActor)
{
	//si bouton appuy� -> d�sactiver le pi�ge
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
	sprite.SetColor(sf::Color(255, 255, 255, 110));
}

void SpikeTrap::SpikeUp()
{
	sprite.SetTexture(data->assets->GetTexture("Assets/Sprites/Game/Map/SpikeTrapOn.png"));
	sprite.SetColor(sf::Color::White);
	CreateCollider();

	isSpikeDown = false;

}

void SpikeTrap::CreateCollider()
{
	sf::Vector2u textureSize = sprite.GetTexture()->getSize();
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC, { position, 0.f, textureSize }, this, true);
	Physics::CreateBoxCollider(body, { Vec2(0,0), 0.f, textureSize });
}

void SpikeTrap::OnTriggerEnter(ColEvent _col)
{

}
