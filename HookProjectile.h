#pragma once
#include "Projectile.h"

class Player;
class BaseArena;
class ItemLoaderXML;

class HookProjectile : public Projectile
{
public:
	HookProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir);
	~HookProjectile();

	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);
private:
};