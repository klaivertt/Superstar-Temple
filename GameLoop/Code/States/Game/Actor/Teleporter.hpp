#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"

#define TP_TRIGGER "tp trigger"
#define TELEPORTER_COOLDOWN 0.35f

class Teleporter : public Actor
{
private:
	Sprite portal;
	Teleporter* linkedTeleporter = nullptr;
	int teleporterId = -1;
	float visualTimer = 0.f;
	std::unordered_map<Actor*, float> recentlyTeleportedActors;

	bool CanTeleportActor(Actor* _actor);
	void MarkActorAsTeleported(Actor* _actor);
public:
	Teleporter(GameData* _data, Vec2 _pos, int _teleporterId = -1);
	virtual std::string GetClassName(void) override { return "Teleporter"; }
	void SetTargetTeleporter(Teleporter* _teleporter);
	void SetColor(const sf::Color& _color);
	int GetTeleporterId() const { return teleporterId; }

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;


// Uncomment the function if you want to use them
	virtual void OnCollisionEnter(ColEvent _col) override;
	virtual void OnCollisionExit(ColEvent _col) override;
// virtual void OnCollisionHit(ColEvent _col) override;
	virtual void OnTriggerEnter(ColEvent _col) override;
	virtual void OnTriggerExit(ColEvent _col) override;
};
