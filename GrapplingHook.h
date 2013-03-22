#pragma once
#include "Projectile.h"

class Player;
class BaseArena;
class ItemLoaderXML;

class GrapplingHook: public Projectile
{
public:
	GrapplingHook(int owner, XMFLOAT3 position, XMFLOAT3 dir);
	~GrapplingHook();

	void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader);
private:
};