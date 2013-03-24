#include "VenomProjectile.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "Sound.h"
#include "PoisonEffect.h"

VenomProjectile::VenomProjectile(int owner, XMFLOAT3 position, XMFLOAT3 dir)
	: Projectile(owner, position, XMFLOAT3(dir.x, 0, dir.z), "F:/Users/Axel/Documents/Visual Studio 11/Projects/Project Warlock/Project Warlock/data/lua/VenomParticle.lua")
{

}

VenomProjectile::~VenomProjectile()
{

}

void VenomProjectile::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = GetDirection();
	dir.y = 0.0f;
	//pPlayer->SetVelocity(dir * GetImpactImpulse() * (1-pPlayer->GetKnockBackResistance()));

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	Player* caster = (Player*)GetWorld()->GetObjectById(GetOwner());

	// Damage the player. 
	pPlayer->TakeDamage(item->GetAttributes().damage + caster->GetBonusDamage());
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));
}

StatusEffect* VenomProjectile::GetStatusEffect(ItemLoaderXML* pItemLoader)
{
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));
	Attributes attributes = item->GetAttributes();
	return new PoisonEffect(attributes.duration, attributes.slow, attributes.dot);
}