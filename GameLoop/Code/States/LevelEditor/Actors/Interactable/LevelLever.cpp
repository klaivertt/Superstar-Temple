#include "LevelLever.hpp"
#include "Tools/Physics/Physics.hpp"

LevelLever::LevelLever(GameData* _data, Vec2 _pos) : Interactable(_data)
{
	position = _pos;

	// Création du body statique
	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC,
		{ Vec2(_pos.x, _pos.y), 0.f, Vec2(100.f, 100.f) }, nullptr);

	// Box collider pour le levier
	//Physics::CreateBoxCollider(body, { Vec2(0.f, 0.f), 0.f, size });

	// Zone de trigger pour l'interaction
	Physics::CreateCircleCollider(body, { Vec2(0.f, 0.f), interactionRadius }, true);

	// Création du sprite
	leverSprite = new Sprite();
	leverSprite->SetTexture(_data->assets->GetTexture("Assets/Sprites/Level/Lever.png"));
	leverSprite->SetOrigin(Vec2(0.5f, 1.f));

	// Création du sprite
	leverBaseSprite = new Sprite();
	leverBaseSprite->SetTexture(_data->assets->GetTexture("Assets/Sprites/Level/Lever_Base.png"));
	leverBaseSprite->SetOrigin(Vec2(0.5f, 0.5f));

	sf::Vector2u textureSize = leverBaseSprite->GetTexture()->getSize();
	size = Vec2(textureSize.x, textureSize.y);

	// Propriétés éditables
	Property prop;

	prop.name = "Size";
	prop.type = P_VECTOR2;
	prop.value = &size;
	prop.range.x = 0.1f;
	prop.range.y = 20.f;
	prop.callback.Add(this, &LevelLever::ResetBox);
	properties.Add(prop);

	prop.name = "Rotation";
	prop.type = P_FLOAT;
	prop.value = &rotation;
	prop.range.x = 0.f;
	prop.range.y = 360.f;
	prop.callback.Add(this, &LevelLever::ResetBox);
	properties.Add(prop);

	prop.name = "Rotation Speed";
	prop.type = P_FLOAT;
	prop.value = &rotationSpeed;
	prop.range.x = 0.f;
	prop.range.y = 720.f;
	properties.Add(prop);

	prop.name = "Interaction Radius";
	prop.type = P_FLOAT;
	prop.value = &interactionRadius;
	prop.range.x = 20.f;
	prop.range.y = 500.f;
	prop.callback.Add(this, &LevelLever::ResetBox);
	properties.Add(prop);

	prop.name = "Is Activated";
	prop.type = P_BOOL;
	prop.value = &isActivated;
	prop.callback.Add(this, &LevelLever::Toggle);
	properties.Add(prop);

	prop.name = "Sprite Size";
	prop.type = P_CONNECTED_VECTOR2;
	prop.value = &scale;
	prop.range.x = 0.1f;
	prop.range.y = 10.f;
	prop.callback.RemoveAll();
	properties.Add(prop);

	prop.name = "Linked object";
	prop.type = P_OBJECT;
	prop.value = &linkedObject;
	prop.objectType = OT_INTERACTABLE;
	properties.Add(prop);
	// Initialisation

	currentAngle = minAngle;
	targetAngle = minAngle;

	Logger::Info("Created Lever at position " + position.ToString());
}

LevelLever::~LevelLever()
{
	//if (leverSprite != nullptr)
	//{
	//	delete leverSprite;
	//}
}

void LevelLever::ResetBox(void)
{
	//if (body != nullptr)
	b2DestroyBody(body);

	body = Physics::CreateBody(data->physicsWorld, Physics::BodyType::STATIC,
		{ position, 0.f, Vec2(100.f, 100.f) }, nullptr);

	Physics::CreateCircleCollider(body, { Vec2(0.f, 0.f), 0.f, Vec2(interactionRadius, interactionRadius) }, true);
}

void LevelLever::Activate(void)
{
	if (!isActivated)
	{
		isActivated = true;
		targetAngle = maxAngle;
		//onStateChanged.Invoke(true);
		//data->log->Info("Lever activated");
	}
}

void LevelLever::Deactivate(void)
{
	if (isActivated)
	{
		isActivated = false;
		targetAngle = minAngle;
		//onStateChanged.Invoke(false);
		//data->log->Info("Lever deactivated");
	}
}

void LevelLever::Toggle(void)
{
	if (linkedObject != nullptr)
	{
		linkedObject->Toggle();
	}
}

bool LevelLever::IsPlayerNearby(void)
{
	// Cette fonction sera appelée via les triggers
	return canInteract;
}

void LevelLever::Update(float _dt)
{
	if (isActivated)
	{
		currentAngle = Lerp(currentAngle, -45.f, rotationSpeed, _dt);
	}
	else
	{
		currentAngle = Lerp(currentAngle, 45.f, rotationSpeed, _dt);
	}

	// Update du sprite
	if (leverSprite != nullptr)
	{
		position = Physics::GetBodyTransform(body).position;
		Vec2 newPos = GetRotationByPosition(Vec2(0.f, 75.f), rotation, scale);
		leverSprite->SetPosition(position + newPos);
		leverSprite->SetRotation(rotation + currentAngle);
		leverBaseSprite->SetRotation(rotation);
		//data->log->Info("Lever position: " + position.ToString() + " angle: " + std::to_string(rotation + currentAngle));
		//data->log->Warning("sprite position: " + leverSprite->GetPosition().ToString() + " rotation: " + std::to_string(leverSprite->GetRotation()));
	}
}

void LevelLever::Draw(sf::RenderTarget* _render)
{
	if (linkedObject != nullptr)
	{
		std::array<sf::Vertex, 2> line =
		{
			sf::Vertex{sf::Vector2f(position.x, position.y), sf::Color::Red},
			sf::Vertex{sf::Vector2f(linkedObject->position.x, linkedObject->position.y), sf::Color::Red}
		};

		_render->draw(line.data(), line.size(), sf::Lines);
	}

	// Dessiner le sprite
	if (leverSprite != nullptr)
	{
		// Changer la couleur selon l'état
		sf::Color currentColor = isActivated ? colorOn : colorOff;
		leverSprite->SetColor(currentColor);
		leverSprite->SetScale(scale);
		leverSprite->Draw(_render);

		leverBaseSprite->SetPosition(position);
		leverBaseSprite->SetScale(scale);
		leverBaseSprite->Draw(_render);
	}

	// Debug: afficher la zone d'interaction

}

void LevelLever::OnTriggerEnter(ColEvent _col)
{
	// Si le joueur entre dans la zone d'interaction
	/*if (_col.other->GetName().find("Player") != std::string::npos)
	{
		canInteract = true;
		data->log->Info("Player can interact with lever");
	}*/
}

void LevelLever::OnTriggerExit(ColEvent _col)
{
	// Si le joueur sort de la zone d'interaction
	/*if (_col.other->GetName().find("Player") != std::string::npos)
	{
		canInteract = false;
		data->log->Info("Player left lever interaction zone");
	}*/
}

sf::FloatRect LevelLever::GetBounds()
{
	return sf::FloatRect(position.x - size.x / 2.f, position.y - size.y / 2.f, size.x, size.y);
}