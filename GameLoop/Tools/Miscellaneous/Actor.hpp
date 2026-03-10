#pragma once

#include "../../Common.hpp"
#include "../Physics/Physics.hpp"
#include "Delegate.hpp"

class GameData;
class Vec2;
class Actor;

struct ColEvent;

// Declare once here so every Actor subclass can use it
DECLARE_DELEGATE_OneParam(OnActorDestroyedDelegate, Actor*)

class Actor
{
public:
	std::string editorObjectId;
	std::string editorTypeId;

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

	// Ce delegate permet de notifier les autres acteurs que cet acteur est détruit, pour éviter les pointeurs invalides
	OnActorDestroyedDelegate onDestroyed;

	virtual void Update(float _dt);
	virtual void Draw(sf::RenderTarget* _render);


	virtual void OnCollisionEnter(ColEvent _col);
	virtual void OnCollisionExit(ColEvent _col);

	virtual void OnCollisionHit(ColEvent _col);

	virtual void OnTriggerEnter(ColEvent _col);
	virtual void OnTriggerExit(ColEvent _col);
	virtual sf::FloatRect GetBounds() const;
	virtual Physics::Transform GetTransform();

	// Editor gizmo support
	virtual float GetEditorRotation() const { return 0.f; }
	virtual void  SetEditorRotation(float) {}
	virtual Vec2  GetEditorSize() const { return Vec2(32.f, 32.f); }
	virtual void  SetEditorSize(Vec2) {}
	virtual void  OnEditorTransformChanged() {}

	const std::string& GetEditorObjectId() const;
	const std::string& GetEditorTypeId() const;
	void SetEditorObjectId(const std::string& _objectId);
	void SetEditorTypeId(const std::string& _typeId);

	Array<Property>* GetProperties(void);
};