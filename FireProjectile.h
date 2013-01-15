#pragma once
#include "Projectile.h"

class Player;
class BaseArena;
class ItemLoaderXML;

class FireProjectile : public Projectile
{
public:
	FireProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir);
	~FireProjectile();

	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);
private:
};