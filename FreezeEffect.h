#pragma once
#include "StatusEffect.h"

class FreezeEffect : public StatusEffect
{
public:
	FreezeEffect();
	~FreezeEffect();

	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	void Apply();
	void Remove();
private:

};