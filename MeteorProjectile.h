#pragma once
#include "Projectile.h"

class MeteorProjectile : public Projectile
{
public:
	MeteorProjectile(int owner, XMFLOAT3 target);
	~MeteorProjectile();

	void Draw(GLib::Graphics* pGraphics);
	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);

	XNA::AxisAlignedBox GetBoundingBox();
	StatusEffect* GetStatusEffect();
private:

};