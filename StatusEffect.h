#pragma once
#include "Items.h"

class Player;

namespace GLib {
	class Graphics;
}

enum StatusEffectType {
	FREEZE_EFFECT
};

class StatusEffect
{
public:
	StatusEffect(ItemName type, float duration = 3) {
		mTimer = 0.0f;
		mType = type;
		mDuration = duration;
	}

	virtual ~StatusEffect() {};

	virtual void Update(float dt) {
		mTimer += dt;
	}
	virtual void Draw(GLib::Graphics* pGraphics) = 0;
	virtual void Apply() = 0;
	virtual void Remove() = 0;

	void SetPlayer(Player* pPlayer) {
		mPlayer = pPlayer;
	}

	void SetDuration(float duration) {
		mDuration = duration;
	}

	float GetDuration() {
		return mDuration;
	}

	float GetTimer() {
		return mTimer;
	}

	ItemName GetType() {
		return mType;
	}

	Player* GetPlayer() {
		return mPlayer;
	}
private:
	Player* mPlayer;
	ItemName mType;
	float mDuration;
	float mTimer;
};