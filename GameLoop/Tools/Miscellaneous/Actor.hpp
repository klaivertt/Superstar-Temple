#pragma once

#include "../../Common.hpp"

class GameData;
class Vec2;


struct ColEvent;

class Actor
{
public:
	// 0.f is the front so move with the camera, increase this value
	// to make the actor being behind the camera
	float z = 0.f; 
	// Position of the actor
	Vec2 position = { 0.f,0.f };

	// The body of the actor, is null per default you have to had one if you want one
	b2BodyId body = { 0 };

	// Pointer to data, to maje thinks easier
	GameData* data = nullptr;
	// The name of the actor
	std::string name = ".";
public:
	// Properties of the actor
	Array<Property> properties;

	Actor(GameData* _data);
	Actor(GameData* _data, Vec2 _pos);
	Actor(GameData* _data, std::string _name);
	~Actor(void);
	virtual std::string GetClassName(void) { return "Actor"; }

	virtual void Update(float _dt);
	virtual void Draw(sf::RenderTarget* _render);


	virtual void OnCollisionEnter(ColEvent _col);
	virtual void OnCollisionExit(ColEvent _col);

	virtual void OnCollisionHit(ColEvent _col);

	virtual void OnTriggerEnter(ColEvent _col);
	virtual void OnTriggerExit(ColEvent _col);
	virtual sf::FloatRect GetBounds() const;
	virtual Physics::Transform GetTransform(void) const;
	

	Array<Property>* GetProperties(void);
};