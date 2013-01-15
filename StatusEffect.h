#pragma once

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
	StatusEffect(StatusEffectType type) {
		mTimer = 0.0f;
		mType = type;
		mDuration = 3.0f;
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

	StatusEffectType GetType() {
		return mType;
	}

	Player* GetPlayer() {
		return mPlayer;
	}
private:
	Player* mPlayer;
	StatusEffectType mType;
	float mDuration;
	float mTimer;
};