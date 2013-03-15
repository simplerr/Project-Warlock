#include "FireProjectile.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "Sound.h"

FireProjectile::FireProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir)
	: Projectile(owner, position, XMFLOAT3(dir.x, 0, dir.z), "FireParticle.lua")
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
	pPlayer->SetVelocity(dir * GetImpactImpulse() * (1-pPlayer->GetKnockBackResistance()));

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	Player* caster = (Player*)GetWorld()->GetObjectById(GetOwner());

	// Damage the player.
	pPlayer->TakeDamage(item->GetAttributes().damage + caster->GetBonusDamage());
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));
}