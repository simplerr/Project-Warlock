#pragma once
#include "Projectile.h"

class StatusEffect;

class FrostProjectile : public Projectile
{
public:
	FrostProjectile(int owner, XMFLOAT3 position);
	~FrostProjectile();

	void Draw(GLib::Graphics* pGraphics);
	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);

	XNA::AxisAlignedBox GetBoundingBox();
	StatusEffect* GetStatusEffect();
private:

};