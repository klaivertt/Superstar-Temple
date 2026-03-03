#pragma once

#include "../../GameData.hpp"

// This class is like a sf::Sprite but with more features and easier to use, it will be used for all the sprites in the game
class Sprite
{
private:
	GameData* data = nullptr;
	// Peut être utile plus tard
	sf::Shader shader;
	sf::RenderStates renderStates;
	bool isUsingShader = false;
	sf::Sprite sprite;
	sf::Texture* texture = nullptr;

	Vec2 position = { 0.f,0.f };
	float rotation = 0.f;
	Vec2 scale = { 0.f,0.f };
	// L'origine est un pourcentage de 0.f à 1.f, en fonction de la taille du sprite
	Vec2 origin = { 0.f,0.f };
public:
	// L'origin est un pourcentage de 0.f à 1.f
	Sprite(sf::Texture* _texture, Vec2 _origin = { 0.f,0.f });
	// L'origin est un pourcentage de 0.f à 1.f
	Sprite(std::string _path, Vec2 _origin = { 0.f,0.f });
	Sprite(void);

	void SetPosition(Vec2 _position);
	void SetOrigin(Vec2 _origin);
	void SetScale(Vec2 _scale);
	// Send the rotation in degrees, it will be converted to radians automatically
	void SetRotation(float _rotation);

	// The origin will be updated to the new texture
	void SetTexture(sf::Texture* _texture);
	// CAUTION if the path is incorrect, the texture set will be the missing texture
	void SetTexture(std::string _path);

	Vec2 GetPosition(void);
	Vec2 GetOrigin(void);
	Vec2 GetScale(void);
	float GetRotation(void);

	sf::Texture* GetTexture(void);

	sf::FloatRect GetGlobalBounds(void);
	sf::FloatRect GetLocalBounds(void);

	sf::IntRect GetTextureRect(void);
	void SetTextureRect(sf::IntRect _rect);

	sf::Color GetColor(void);
	void SetColor(sf::Color _color);

	void SetShader(std::string _path);
	void SetUseShader(bool _use);

	void Draw(sf::RenderTarget* _render);
};