#pragma once
#include "StatusEffect.h"

class TeleportEffect : public StatusEffect
{
public:
	TeleportEffect();
	~TeleportEffect();

	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);
	void Apply();
	void Remove();
};