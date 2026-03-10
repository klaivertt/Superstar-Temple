#include "Physics.hpp"

#include "../SceneManager.hpp"

namespace
{
	ShapeData* CreateShapeDataForBody(b2BodyId _body, const std::string& _sensorId)
	{
		ShapeData* shapeData = new ShapeData();
		shapeData->actor = static_cast<Actor*>(b2Body_GetUserData(_body));
		shapeData->sensorId = _sensorId;
		return shapeData;
	}
}

#pragma region DebugDraw

b2WorldDef Physics::CreateDefaultWorldDef(void)
{
	b2WorldDef def = b2DefaultWorldDef();
	def.gravity = { 0.f, -GRAVITY };
	return def;
}

b2WorldId Physics::CreateWorld(b2WorldDef* _def)
{
	b2WorldId world = b2CreateWorld(_def);
	b2World_EnableSleeping(world, false);

	Logger::Success("Physics World created");
	return world;
}

Vec2 Physics::GetWorldGravity(b2WorldId _id)
{
	b2Vec2 gravity = b2World_GetGravity(_id);

	return Vec2(gravity.x, gravity.y);
}

void Physics::Update(b2WorldId _id, float _dt, int _step)
{
	b2World_Step(_id, _dt, _step);
	//b2World_Step(_id, 1.f/60.f, _step);
}

void Physics::DrawDebug(b2WorldId _world, b2DebugDraw* _debugDraw)
{
	b2World_Draw(_world, _debugDraw);
}

void Physics::Destroy(b2WorldId _id)
{
	b2DestroyWorld(_id);
}

b2BodyId Physics::CreateBody(b2WorldId _world, BodyType _type, Transform _transform, Actor* _parent, bool _fixedRotation)
{
	b2BodyDef def = b2DefaultBodyDef();
	def.fixedRotation = _fixedRotation;

	switch (_type)
	{
	case Physics::STATIC:
		def.type = b2_staticBody;
		break;
	case Physics::DYNAMIC:
		def.type = b2_dynamicBody;
		break;
	case Physics::KINEMATIC:
		def.type = b2_kinematicBody;
		break;
	default:
		def.type = b2_staticBody;
		break;
	}

	b2BodyId body = b2CreateBody(_world, &def);

	b2Vec2 pos = { _transform.position.x / METERS_TO_PIXELS, -_transform.position.y / METERS_TO_PIXELS };
	b2Rot rot = b2MakeRot(DegreesToRadian(-_transform.rotation));

	b2Body_SetTransform(body, pos, rot);

	b2Body_SetUserData(body, (void*)_parent);

	return body;
}

void Physics::SetShapePreset(b2ShapeId _shape, CollisionPreset _preset)
{
	b2Filter filter = b2Shape_GetFilter(_shape);
	filter.categoryBits = _preset;

	b2Shape_SetFilter(_shape, filter);
}

void Physics::SetShapeResponses(b2ShapeId _shape, uint32_t _presets)
{
	b2Filter filter = b2Shape_GetFilter(_shape);
	filter.maskBits = _presets;

	b2Shape_SetFilter(_shape, filter);
}

void Physics::SetLinearVelocity(b2BodyId _body, Vec2 _velocity)
{
	b2Vec2 vel = b2Body_GetLinearVelocity(_body);
	float desiredSpeed = _velocity.x / METERS_TO_PIXELS;
	float change = desiredSpeed - vel.x;

	float impulse = b2Body_GetMass(_body) * change;

	//b2Body_ApplyLinearImpulseToCenter(_body, b2Vec2(impulse, 0.f ), true);
	b2Body_SetLinearVelocity(_body, b2Vec2(_velocity.x, vel.y));
}

void Physics::SetVelocity(b2BodyId _body, Vec2 _velocity)
{
	b2Vec2 vel = b2Body_GetLinearVelocity(_body);

	// set the velocity to 0 and apply new velocity
	b2Body_SetLinearVelocity(_body, b2Vec2(0.f, 0.f));

	b2Body_ApplyLinearImpulseToCenter(_body, b2Vec2(_velocity.x * b2Body_GetMass(_body), _velocity.y * b2Body_GetMass(_body)), true);
}

void Physics::ApplyForce(b2BodyId _body, Vec2 _force)
{
	b2Body_ApplyForceToCenter(_body, { _force.x, _force.y }, true);
}

void Physics::SetBodyFixedRotation(b2BodyId _body, bool _fixed)
{
	b2Body_SetFixedRotation(_body, _fixed);
}

b2ShapeId Physics::CreateBoxCollider(b2BodyId _body, Transform _transform)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;


	b2Hull hull = { 0 };

	// Points locaux du rectangle (en m�tres)
	hull.count = 4;
	hull.points[0] = { -(_transform.size.x / 2) / METERS_TO_PIXELS, -(_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[1] = { (_transform.size.x / 2) / METERS_TO_PIXELS, -(_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[2] = { (_transform.size.x / 2) / METERS_TO_PIXELS, (_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[3] = { -(_transform.size.x / 2) / METERS_TO_PIXELS, (_transform.size.y / 2) / METERS_TO_PIXELS };

	// Appliquer la rotation autour de l'origine (coh�rente avec CreateBody qui utilise -rotation)
	float angle = DegreesToRadian(-_transform.rotation);
	float c = cosf(angle);
	float s = sinf(angle);

	for (int i = 0; i < hull.count; i++)
	{
		// Rotation
		float x = hull.points[i].x;
		float y = hull.points[i].y;
		float rx = c * x - s * y;
		float ry = s * x + c * y;
		hull.points[i].x = rx;
		hull.points[i].y = ry;

		// Translation en coordonn�es monde (conversion m�tres + inversion Y)
		hull.points[i].x += _transform.position.x / METERS_TO_PIXELS;
		hull.points[i].y -= _transform.position.y / METERS_TO_PIXELS;
	}

	b2Polygon poly = b2MakePolygon(&hull, 0.f);

	b2ShapeId shape = b2CreatePolygonShape(_body, &def, &poly);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, ""));
	b2Shape_EnableContactEvents(shape, true);
	b2Shape_EnableSensorEvents(shape, true);

	b2Shape_SetRestitution(shape, 0.f);

	SetShapePreset(shape, CollisionPreset::NONE_MOVABLE);

	return shape;
}

b2ShapeId Physics::CreateConvexCollider(b2BodyId _body, Transform _transform, b2Hull _hull)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;

	for (int i = 0; i < _hull.count; i++)
	{
		_hull.points[i].x *= (_transform.size.x / METERS_TO_PIXELS);
		_hull.points[i].y *= -(_transform.size.y / METERS_TO_PIXELS);
	}

	for (int i = 0; i < _hull.count; i++)
	{
		_hull.points[i].x += _transform.position.x / METERS_TO_PIXELS;
		_hull.points[i].y -= _transform.position.y / METERS_TO_PIXELS;
	}

	b2Polygon poly = b2MakePolygon(&_hull, 0.f);

	b2ShapeId shape = b2CreatePolygonShape(_body, &def, &poly);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, ""));
	b2Shape_EnableContactEvents(shape, true);
	b2Shape_EnableSensorEvents(shape, true);

	b2Shape_SetRestitution(shape, 0.f);

	SetShapePreset(shape, CollisionPreset::NONE_MOVABLE);

	return shape;
}

b2ShapeId Physics::CreateCircleCollider(b2BodyId _body, Transform _transform, float _radius)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;

	b2Circle circle = { 0 };
	circle.center = (b2Vec2)(_transform.position.x / METERS_TO_PIXELS, -_transform.position.y / METERS_TO_PIXELS);
	//circle.center = (b2Vec2)(0.f, 0.f);
	circle.radius = _radius / METERS_TO_PIXELS;

	b2ShapeId shape = b2CreateCircleShape(_body, &def, &circle);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, ""));
	b2Shape_EnableContactEvents(shape, true);
	b2Shape_EnableSensorEvents(shape, true);

	b2Shape_SetRestitution(shape, 0.f);

	SetShapePreset(shape, CollisionPreset::NONE_MOVABLE);

	return shape;
}

b2ShapeId Physics::CreateBoxTrigger(b2BodyId _body, Transform _transform)
{
	return Physics::CreateBoxTrigger(_body, _transform, "");
}

b2ShapeId Physics::CreateBoxTrigger(b2BodyId _body, Transform _transform, const std::string& _sensorId)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;
	def.isSensor = true;

	b2Hull hull = { 0 };

	hull.count = 4;
	hull.points[0] = { -(_transform.size.x / 2) / METERS_TO_PIXELS, -(_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[1] = { (_transform.size.x / 2) / METERS_TO_PIXELS, -(_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[2] = { (_transform.size.x / 2) / METERS_TO_PIXELS, (_transform.size.y / 2) / METERS_TO_PIXELS };
	hull.points[3] = { -(_transform.size.x / 2) / METERS_TO_PIXELS, (_transform.size.y / 2) / METERS_TO_PIXELS };

	for (int i = 0; i < hull.count; i++)
	{
		hull.points[i].x += _transform.position.x / METERS_TO_PIXELS;
		hull.points[i].y -= _transform.position.y / METERS_TO_PIXELS;
	}

	b2Polygon poly = b2MakePolygon(&hull, 0.f);

	b2ShapeId shape = b2CreatePolygonShape(_body, &def, &poly);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, _sensorId));
	b2Shape_EnableSensorEvents(shape, true);

	return shape;
}

b2ShapeId Physics::CreateConvexTrigger(b2BodyId _body, Transform _transform, b2Hull _hull)
{
	return Physics::CreateConvexTrigger(_body, _transform, _hull, "");
}

b2ShapeId Physics::CreateConvexTrigger(b2BodyId _body, Transform _transform, b2Hull _hull, const std::string& _sensorId)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;
	def.isSensor = true;

	for (int i = 0; i < _hull.count; i++)
	{
		_hull.points[i].x *= (_transform.size.x / METERS_TO_PIXELS);
		_hull.points[i].y *= -(_transform.size.y / METERS_TO_PIXELS);
	}

	for (int i = 0; i < _hull.count; i++)
	{
		_hull.points[i].x += _transform.position.x / METERS_TO_PIXELS;
		_hull.points[i].y -= _transform.position.y / METERS_TO_PIXELS;
	}

	b2Polygon poly = b2MakePolygon(&_hull, 0.f);

	b2ShapeId shape = b2CreatePolygonShape(_body, &def, &poly);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, _sensorId));
	b2Shape_EnableSensorEvents(shape, true);

	return shape;
}

b2ShapeId Physics::CreateCircleTrigger(b2BodyId _body, Transform _transform, float _radius)
{
	return Physics::CreateCircleTrigger(_body, _transform, _radius, "");
}

b2ShapeId Physics::CreateCircleTrigger(b2BodyId _body, Transform _transform, float _radius, const std::string& _sensorId)
{
	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.f;
	def.isSensor = true;

	b2Circle circle = { 0 };
	circle.center = (b2Vec2)(_transform.position.x / METERS_TO_PIXELS, -_transform.position.y / METERS_TO_PIXELS);
	circle.radius = _radius / METERS_TO_PIXELS;

	b2ShapeId shape = b2CreateCircleShape(_body, &def, &circle);
	b2Shape_SetUserData(shape, CreateShapeDataForBody(_body, _sensorId));
	b2Shape_EnableSensorEvents(shape, true);

	return shape;
}

void Physics::ModifyShape(b2ShapeId _shape, float _friction, float _restitution, float _density)
{
	b2Shape_SetFriction(_shape, _friction);
	b2Shape_SetRestitution(_shape, _restitution);
	b2Shape_SetDensity(_shape, _density, true);
}

Physics::Transform Physics::GetBodyTransform(b2BodyId _body)
{
	Physics::Transform trans;

	b2Vec2 pos = b2Body_GetPosition(_body);
	b2Rot rot = b2Body_GetRotation(_body);

	trans.position = Vec2(pos.x * METERS_TO_PIXELS, -pos.y * METERS_TO_PIXELS);
	trans.rotation = -RadianToDegrees(b2Rot_GetAngle(rot));

	return trans;
}

void Physics::SetBodyType(b2BodyId _body, BodyType _type)
{
	switch (_type)
	{
	case Physics::STATIC:
		b2Body_SetType(_body, b2_staticBody);
		break;
	case Physics::DYNAMIC:
		b2Body_SetType(_body, b2_dynamicBody);
		break;
	case Physics::KINEMATIC:
		b2Body_SetType(_body, b2_kinematicBody);
		break;
	default:
		break;
	}
}

void Physics::SetBodyTransform(b2BodyId _body, Vec2 _position, float _rotation)
{
	b2Vec2 pos = { _position.x / METERS_TO_PIXELS, -_position.y / METERS_TO_PIXELS };
	b2Rot rot = b2MakeRot(DegreesToRadian(-_rotation));
	b2Body_SetTransform(_body, pos, rot);
}

void Physics::SetBodyTransform(b2BodyId _body, Transform _transform)
{
	b2Vec2 pos = { _transform.position.x / METERS_TO_PIXELS, -_transform.position.y / METERS_TO_PIXELS };
	b2Rot rot = b2MakeRot(DegreesToRadian(-_transform.rotation));
	b2Body_SetTransform(_body, pos, rot);
}

void Physics::DebugDrawPoint(b2Vec2 p1, float size, b2HexColor color, void* _context)
{
	sf::Vertex point = sf::Vertex(WorldToScreen(p1), sf::Color((color << 8) | 255));

	sf::RectangleShape rectangle({ size,size });
	rectangle.setPosition(WorldToScreen(p1) - sf::Vector2f(size / 2, size / 2));
	rectangle.setFillColor(sf::Color((color << 8) | 255));
	((GameData*)_context)->render->draw(rectangle);
}

void Physics::DebugDrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* _context)
{
	sf::ConvexShape polygon(vertexCount);
	sf::Vector2f center;
	for (int i = 0; i < vertexCount; i++)
	{
		sf::Vector2f transformedVec = WorldToScreen(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(sf::Color((color << 8) | 255));

	((GameData*)_context)->render->draw(polygon);
}

void Physics::DebugDrawSolidPolygon(b2Transform _transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* _context)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		sf::Vector2f transformedVec = WorldToScreen(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color((color << 8) | 60));
	polygon.setOutlineColor(sf::Color((color << 8) | 255));
	polygon.setPosition(WorldToScreen(_transform.p));
	polygon.setRotation(-b2Rot_GetAngle(_transform.q) * 180 / 3.1415926535f);


	((GameData*)_context)->render->draw(polygon);
}

void Physics::DebugDrawCircle(b2Vec2 center, float radius, b2HexColor color, void* _context)
{
	sf::CircleShape circle(radius * METERS_TO_PIXELS);
	circle.setOrigin(radius * METERS_TO_PIXELS, radius * METERS_TO_PIXELS);
	circle.setPosition(WorldToScreen(center));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(sf::Color((color << 8) | 255));

	((GameData*)_context)->render->draw(circle);
}

void Physics::DebugDrawSolidCircle(b2Transform _transform, float radius, b2HexColor color, void* _context)
{
	sf::CircleShape circle(radius * METERS_TO_PIXELS);
	circle.setOrigin(radius * METERS_TO_PIXELS, radius * METERS_TO_PIXELS);
	circle.setPosition(WorldToScreen(_transform.p));
	circle.setFillColor(sf::Color((color << 8) | 60));
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(sf::Color((color << 8) | 255));

	b2Vec2 endPoint = _transform.p + b2Vec2{ radius, 0 };// +radius * b2Tr _transform.q;
	sf::Vertex line[2] =
	{
		sf::Vertex(WorldToScreen(_transform.p), sf::Color((color << 8) | 255)),
		sf::Vertex(WorldToScreen(endPoint), sf::Color((color << 8) | 255)),
	};

	((GameData*)_context)->render->draw(circle);
	((GameData*)_context)->render->draw(line, 2, sf::Lines);
}

void Physics::DebugDrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* _context)
{
	sf::Vertex line[] =
	{
		sf::Vertex(WorldToScreen(p1), sf::Color((color << 8) | 255)),
		sf::Vertex(WorldToScreen(p2), sf::Color((color << 8) | 255))
	};

	((GameData*)_context)->render->draw(line, 2, sf::Lines);
}

void Physics::DebugDrawTransform(b2Transform _transform, void* _context)
{
	float lineLength = 0.4f;

	b2Vec2 xAxis = _transform.p + lineLength * b2Rot_GetXAxis(_transform.q);
	sf::Vertex redLine[] =
	{
		sf::Vertex(WorldToScreen(_transform.p), sf::Color::Red),
		sf::Vertex(WorldToScreen(xAxis), sf::Color::Red)
	};


	// You might notice that the ordinate(Y axis) points downward unlike the one in Box2D testbed
	// That's because the ordinate in SFML coordinate system points downward while the OpenGL(testbed) points upward
	b2Vec2 yAxis = _transform.p + lineLength * b2Rot_GetYAxis(_transform.q);
	sf::Vertex greenLine[] =
	{
		sf::Vertex(WorldToScreen(_transform.p), sf::Color::Green),
		sf::Vertex(WorldToScreen(yAxis), sf::Color::Green)
	};

	((GameData*)_context)->render->draw(redLine, 2, sf::Lines);
	((GameData*)_context)->render->draw(greenLine, 2, sf::Lines);
}


#pragma endregion

void Physics::DebugDrawString(b2Vec2 p1, const char* string, b2HexColor color, void* _context)
{
}

sf::Vector2f Physics::WorldToScreen(b2Vec2 _world)
{
	_world.y = -_world.y;
	_world.x *= METERS_TO_PIXELS;
	_world.y *= METERS_TO_PIXELS;

	return sf::Vector2f(_world.x, _world.y);
}

Vec2 Physics::GetBodyPosition(b2BodyId _body)
{
	if (b2Body_IsValid(_body))
	{
		b2Vec2 pos = b2Body_GetPosition(_body);
		pos.y = -pos.y;
		return pos * METERS_TO_PIXELS;
	}

	return Vec2(0, 0);
}

void Physics::SetBodyPosition(b2BodyId _body, Vec2 _position)
{
	b2Vec2 pos = { _position.x / METERS_TO_PIXELS, -_position.y / METERS_TO_PIXELS };
	b2Body_SetTransform(_body, pos, b2Body_GetRotation(_body));
}

float Physics::GetBodyRotation(b2BodyId _body)
{
	if (b2Body_IsValid(_body))
	{
		b2Rot rot = b2Body_GetRotation(_body);
		return -RadianToDegrees(b2Rot_GetAngle(rot));
	}
	return 0.f;
}

void Physics::SetBodyRotation(b2BodyId _body, float _rotation)
{
	b2Rot rot = b2MakeRot(DegreesToRadian(-_rotation));
	b2Body_SetTransform(_body, b2Body_GetPosition(_body), rot);
}

sf::FloatRect Physics::GetBodyBound(b2BodyId _body)
{
	if (b2Body_IsValid(_body))
	{
		b2AABB aabb = b2Body_ComputeAABB(_body);

		float left = aabb.lowerBound.x * METERS_TO_PIXELS;
		float top = -aabb.upperBound.y * METERS_TO_PIXELS;
		float width = (aabb.upperBound.x - aabb.lowerBound.x) * METERS_TO_PIXELS;
		float height = (aabb.upperBound.y - aabb.lowerBound.y) * METERS_TO_PIXELS;

		return sf::FloatRect(left, top, width, height);
	}

	return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
}
