#include "FireProjectile.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "Sound.h"

FireProjectile::FireProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir)
	: Projectile(owner, position, dir, "FireParticle.lua")
{
	
}

FireProjectile::~FireProjectile()
{

}

void FireProjectile::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = GetDirection();
	dir.y = 0.0f;
	pPlayer->SetVelocity(dir * GetImpactImpulse());

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	// Damage the player.
	pPlayer->TakeDamage(item->GetAttributes().damage);
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));

	// Dead? [NOTE] A bit of a hack.
	if(pPlayer->GetCurrentHealth() <= 0) 
		pArena->PlayerEliminated(pPlayer, (Player*)GetWorld()->GetObjectById(GetOwner()));
}