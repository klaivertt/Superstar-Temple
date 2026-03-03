#include "Door.hpp"
#include "Tools/Physics/Physics.hpp"

LevelDoor::LevelDoor(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	position = _pos;

	// Création du body statique
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC,
		{ Vec2(_pos.x, _pos.y), 0.f, size }, nullptr);

	// Box collider pour la porte (bloque le joueur quand fermée)
	Physics::CreateBoxCollider(body, { Vec2(0.f, 0.f), 0.f, size });

	// Création du sprite de la porte avec spritesheet
	doorSprite = new Sprite();
	doorSprite->SetTexture(_data->assets->GetTexture("Assets/Sprites/Level/Door.png"));
	doorSprite->SetOrigin(Vec2(0.5f, 0.5f));

	sf::Vector2u textureSize = doorSprite->GetTexture()->getSize();
	size = Vec2(textureSize.x, textureSize.y);
	doorSprite->SetTextureRect(sf::IntRect(0, 0, textureSize.x / 2, textureSize.y));

	// Propriétés éditables
	Property prop;

	prop.name = "Size";
	prop.type = P_VECTOR2;
	prop.value = &size;
	prop.range.x = 1.f;
	prop.range.y = 500.f;
	prop.callback.Add(this, &LevelDoor::ResetBox);
	properties.Add(prop);

	prop.name = "Rotation";
	prop.type = P_FLOAT;
	prop.value = &rotation;
	prop.range.x = 0.f;
	prop.range.y = 360.f;
	prop.callback.Add(this, &LevelDoor::ResetBox);
	properties.Add(prop);

	prop.name = "Move Speed";
	prop.type = P_FLOAT;
	prop.range.x = 1.f;
	prop.range.y = 300.f;
	prop.value = &moveSpeed;
	properties.Add(prop);

	prop.name = "Is Activated";
	prop.type = P_BOOL;
	prop.value = &isOpen;
	properties.Add(prop);

	prop.name = "Has Collider";
	prop.type = P_BOOL;
	prop.value = &hasCollider;
	prop.callback.Add(this, &LevelDoor::ResetBox);
	properties.Add(prop);

	prop.name = "Sprite Size";
	prop.type = P_CONNECTED_VECTOR2;
	prop.value = &scale;
	prop.range.x = 0.1f;
	prop.range.y = 10.f;
	properties.Add(prop);

	// Initialisation
	currentFrame = 0;

	Logger::Info("Created Door at position " + position.ToString());
}

LevelDoor::~LevelDoor()
{
	// Les sprites seront gérés par le système
}

void LevelDoor::ResetBox(void)
{
	b2DestroyBody(body);

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC,
		{ position, rotation, size }, nullptr);

	// Ne créer le collider que si la porte doit bloquer
	if (hasCollider && !isOpen)
	{
		Physics::CreateBoxCollider(body, { Vec2(0.f, 0.f), 0.f, size });
	}
}

void LevelDoor::Open(void)
{
	if (!isOpen)
	{
		isOpen = true;

		// Désactiver le collider quand la porte s'ouvre
		if (hasCollider)
		{
			ResetBox();
		}

		Logger::Info("Door opening");
	}
}

void LevelDoor::Close(void)
{
	if (isOpen)
	{
		isOpen = false;
		Logger::Info("Door closing");
	}
}

void LevelDoor::Toggle(void)
{
	if (isOpen)
	{
		Close();
	}
	else
	{
		Open();
	}
}

void LevelDoor::SetDoorState(bool _open)
{
	if (_open)
	{
		Open();
	}
	else
	{
		Close();
	}
}

bool LevelDoor::IsFullyOpen(void)
{
	return 0.f < 1.f;
}

bool LevelDoor::IsFullyClosed(void)
{
	return 0.f < 1.f;
}

void LevelDoor::Update(float _dt)
{

	int targetFrame = isOpen ? 1 : 0;

    //data->log->Info(data->log->Bool("Is Open: ", isOpen), false);

	if (currentFrame != targetFrame)
	{
		currentFrame = targetFrame;

		sf::Vector2u textureSize = doorSprite->GetTexture()->getSize();
		doorSprite->SetTextureRect(sf::IntRect((textureSize.x / 2 * targetFrame)		, 0, textureSize.x / 2, textureSize.y));
	}

	if (!isOpen && IsFullyClosed() && hasCollider)
	{
		if (!wasFullyClosed)
		{
			ResetBox();
			wasFullyClosed = true;
		}
	}
	else
	{
		wasFullyClosed = false;
	}

	// Update du sprite
	if (doorSprite != nullptr)
	{
		doorSprite->SetPosition(position);
		doorSprite->SetRotation(rotation);
	}
}

void LevelDoor::Draw(sf::RenderTarget* _render)
{
	if (doorSprite != nullptr)
	{
		sf::Color currentColor = isOpen ? colorOpen : colorClosed;
		doorSprite->SetColor(currentColor);
		doorSprite->SetScale(scale);
		doorSprite->Draw(_render);
	}

	sf::RectangleShape debugBox(sf::Vector2f(size.x, size.y));
	debugBox.setOrigin(size.x / 2.f, size.y / 2.f);
	debugBox.setPosition(position.x, position.y);
	debugBox.setRotation(rotation);

	if (isOpen)
	{
		debugBox.setFillColor(sf::Color(0, 255, 0, 50));
		debugBox.setOutlineColor(sf::Color::Green);
	}
	else
	{
		debugBox.setFillColor(sf::Color(255, 0, 0, 50));
		debugBox.setOutlineColor(sf::Color::Red);
	}

	debugBox.setOutlineThickness(2.f);
	_render->draw(debugBox);
}

void LevelDoor::OnTriggerEnter(ColEvent _col)
{

}

void LevelDoor::OnTriggerExit(ColEvent _col)
{

}

sf::FloatRect LevelDoor::GetBounds()
{
	return sf::FloatRect(position.x - size.x / 2.f, position.y - size.y / 2.f, size.x, size.y);
}