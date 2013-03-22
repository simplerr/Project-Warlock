#include "GrapplingHook.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"

GrapplingHook::GrapplingHook(int owner, XMFLOAT3 position, XMFLOAT3 dir)
	: Projectile(owner, position, XMFLOAT3(dir.x, 0, dir.z), "GrapplingHookParticle.lua")
{
	int a = 1;
}

GrapplingHook::~GrapplingHook()
{

}

void GrapplingHook::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = GetDirection();
	dir.y = 0.0f;

	Player* caster = (Player*)GetWorld()->GetObjectById(GetOwner());

	caster->SetVelocity(dir * GetImpactImpulse());
	caster->ClearTargetQueue();

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	// Damage the player.
	pPlayer->TakeDamage(item->GetAttributes().damage + caster->GetBonusDamage());
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));
}