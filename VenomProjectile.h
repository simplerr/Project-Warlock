#pragma once
#include "Projectile.h"

class Player;
class BaseArena;
class ItemLoaderXML;

class VenomProjectile : public Projectile
{
public:
	VenomProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir);
	~VenomProjectile();

	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);
	StatusEffect* GetStatusEffect(ItemLoaderXML* pItemLoader);
private:
};