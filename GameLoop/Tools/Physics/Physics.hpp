#pragma once

#include "../../GameData.hpp"
#include <cmath>

#define METERS_TO_PIXELS 64.f

class Actor;

namespace Physics
{
	// The transform of a body, used to easily set the position, rotation and size of a body when creating it
	struct Transform
	{
		Vec2 position = { 0.f,0.f };
		float rotation = 0.f;
		Vec2 size = { 1.f,1.f };
	};

	// The type of the body
	enum BodyType
	{
		STATIC, // While not move at all
		DYNAMIC, // While move with the gravity and forces applied to it
		KINEMATIC // While move but not with the gravity and forces applied to it, you have to set the velocity of the body yourself
	};

	// The collision response of the shape, used to easily set the collision response of a shape with a preset
	enum CollisionPreset
	{
		NONE_MOVABLE = 0x00000001,
		MOVABLE = 0x00000002,
		PLAYER = 0x00000003,
		ENEMIES = 0x00000004,
	};

	b2WorldDef CreateDefaultWorldDef(void);
	b2WorldId CreateWorld(b2WorldDef* _def);
	Vec2 GetWorldGravity(b2WorldId _id);

	void Update(b2WorldId _id, float _dt, int _step);
	void DrawDebug(b2WorldId _world, b2DebugDraw* _debugDraw);
	void Destroy(b2WorldId _id);

	// Actor needed to set the user data of the body, so we can easily get the actor from the body in the collision events
	// @param Actor* _parent: Important, si vous voulez que les collisions de ce body soient associées à un acteur, mettez un pointeur vers cet acteur, sinon mettez nullptr
	b2BodyId CreateBody(b2WorldId _world, BodyType _type, Transform _transform, Actor* _parent, bool _fixedRotation = false);
	// Set the preset of the shape, so we can easily set the collision response of the shape in the collision events
	void SetShapePreset(b2ShapeId _shape, CollisionPreset _preset);
	// Set the collision response of the shape with a custom preset, so you can use it in the collision events
	void SetShapeResponses(b2ShapeId _shape, uint32_t _presets);

	void SetLinearVelocity(b2BodyId _body, Vec2 _velocity);
	void ApplyForce(b2BodyId _body, Vec2 _force);
	void SetBodyFixedRotation(b2BodyId _body, bool _fixed);
	Transform GetBodyTransform(b2BodyId _body);
	void SetBodyType(b2BodyId _body, BodyType _type);
	void SetBodyTransform(b2BodyId _body, Vec2 _position, float _rotation);
	void SetBodyTransform(b2BodyId _body, Transform _transform);

	// Shapes
	b2ShapeId CreateBoxCollider(b2BodyId _body, Transform _transform);
	b2ShapeId CreateConvexCollider(b2BodyId _body, Transform _transform, b2Hull _hull);
	b2ShapeId CreateCircleCollider(b2BodyId _body, Transform _transform, float _radius);

	b2ShapeId CreateBoxTrigger(b2BodyId _body, Transform _transform);
	b2ShapeId CreateConvexTrigger(b2BodyId _body, Transform _transform, b2Hull _hull);
	b2ShapeId CreateCircleTrigger(b2BodyId _body, Transform _transform, float _radius);

	// Modify the shape with new values for friction, restitution and density
	void ModifyShape(b2ShapeId _shape, float _friction, float _restitution, float _density);
	// -- DEBUGDRAW --

	///  Draw point
	void DebugDrawPoint(b2Vec2 p1, float size, b2HexColor color, void* _context);

	/// Draw a closed polygon provided in CCW order.
	void DebugDrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* _context);

	/// Draw a solid closed polygon provided in CCW order.
	void DebugDrawSolidPolygon(b2Transform _transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* _context);

	/// Draw a circle.
	void DebugDrawCircle(b2Vec2 center, float radius, b2HexColor color, void* _context);

	/// Draw a solid circle.
	void DebugDrawSolidCircle(b2Transform _transform, float radius, b2HexColor color, void* _context);

	/// Draw a line segment.
	void DebugDrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* _context);

	/// Draw a transform. Choose your own length scale.
	void DebugDrawTransform(b2Transform _transform, void* _context);

	//Draw a string
	void DebugDrawString(b2Vec2 p1, const char* string, b2HexColor color, void* _context);

	sf::Vector2f WorldToScreen(b2Vec2 _world);

	Vec2 GetBodyPosition(b2BodyId _body);
	void SetBodyPosition(b2BodyId _body, Vec2 _position);

	sf::FloatRect GetBodyBound(b2BodyId _body);

	// Not destroy the world immediately, just clear it by destroying all the bodies and shapes in it, so you can reuse it later without having to create a new one
}