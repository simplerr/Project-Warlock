#pragma once
#include "Items.h"
#include "ParticleSystem.h"

class Player;

namespace GLib {
	class Graphics;
	class Texture2D;
	class ParticleSystem;
}

enum StatusEffectType {
	FREEZE_EFFECT
};

class StatusEffect
{
public:
	StatusEffect(ItemName type, float duration, string particleTexture, string luaScript);

	virtual ~StatusEffect();
	void DeleteParticleSystem();

	virtual void Update(float dt);
	virtual void Draw(GLib::Graphics* pGraphics);
	virtual void Apply() = 0;
	virtual void Remove();

	void SetPlayer(Player* pPlayer);
	void SetDuration(float duration);

	float GetDuration();
	float GetTimer();
	ItemName GetType();
	Player* GetPlayer();
private:
	GLib::ParticleSystem* mParticleSystem;
	Player* mPlayer;
	ItemName mType;
	float mDuration;
	float mTimer;
};