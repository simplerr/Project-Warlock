#include "HookProjectile.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"

HookProjectile::HookProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir)
	: Projectile(owner, position, XMFLOAT3(dir.x, 0, dir.z), "HookParticle.lua")
{
	int a = 1;
}

HookProjectile::~HookProjectile()
{

}

void HookProjectile::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = GetDirection();
	dir.y = 0.0f;
	pPlayer->SetVelocity(dir * GetImpactImpulse() * (1.0f-pPlayer->GetKnockBackResistance()));
	pPlayer->ClearTargetQueue();

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	Player* caster = (Player*)GetWorld()->GetObjectById(GetOwner());

	// Damage the player.
	pPlayer->TakeDamage(item->GetAttributes().damage + caster->GetBonusDamage());
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));

	// Dead? [NOTE] A bit of a hack.
	//if(pPlayer->GetCurrentHealth() <= 0) 
	//	pArena->PlayerEliminated(pPlayer, (Player*)GetWorld()->GetObjectById(GetOwner()));
}