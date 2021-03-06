#include "FrostProjectile.h"
#include "Particle.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "FreezeEffect.h"
#include "Sound.h"

FrostProjectile::FrostProjectile(int owner, XMFLOAT3 position)
	: Projectile(owner, position, XMFLOAT3(0, 0, 0), "data/lua/FrostParticle.lua")
{
	
}

FrostProjectile::~FrostProjectile()
{

}

void FrostProjectile::Draw(GLib::Graphics* pGraphics)
{
	Projectile::Draw(pGraphics);

	GLib::Particle* particle = GetParticle(0);
	if(particle == nullptr)
		return;

	XMFLOAT3 pos = particle->GetPosition();
	XMFLOAT3 diff = pos - GetPosition();
	float dist = sqrt(diff.x*diff.x + diff.z*diff.z);

	/*XNA::AxisAlignedBox box;
	box.Center = GetPosition();
	box.Extents = XMFLOAT3(dist, 10, dist);
	pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::Red));*/
}

void FrostProjectile::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = GetDirection();
	pPlayer->SetVelocity(dir * GetImpactImpulse() * (1.0f-pPlayer->GetKnockBackResistance()));
	pPlayer->ClearTargetQueue();

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	Player* caster = (Player*)GetWorld()->GetObjectById(GetOwner());

	// Damage the player.
	pPlayer->TakeDamage(item->GetAttributes().damage + caster->GetBonusDamage());
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));
}

AxisAlignedBox FrostProjectile::GetBoundingBox()
{
	GLib::Particle* particle = GetParticle(0);
	if(particle == nullptr)
		return XNA::AxisAlignedBox();

	XMFLOAT3 pos = particle->GetPosition();
	XMFLOAT3 diff = pos - GetPosition();
	float dist = sqrt(diff.x*diff.x + diff.z*diff.z);

	XNA::AxisAlignedBox box;
	box.Center = GetPosition();
	box.Extents = XMFLOAT3(dist, 10, dist);

	char buffer[244];
	sprintf(buffer, "DIST MFER: %f\n", dist);
	OutputDebugString(buffer);

	return box;
}

StatusEffect* FrostProjectile::GetStatusEffect(ItemLoaderXML* pItemLoader)
{
	return new FreezeEffect();
}