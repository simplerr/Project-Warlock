#pragma once
#include "StatusEffect.h"

class PoisonEffect : public StatusEffect
{
public:
	PoisonEffect(float duration, float slow, float dot);
	~PoisonEffect();

	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	void Apply();
	void Remove();
private:
	float mDamagePerSecond;
	float mSlow;
	float mDelta;
};