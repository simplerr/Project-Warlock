#include "FrostProjectile.h"
#include "Particle.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "FreezeEffect.h"

FrostProjectile::FrostProjectile(int owner, XMFLOAT3 position)
	: Projectile(owner, position, XMFLOAT3(0, 0, 0), "F:\\Users\\Axel\\Documents\\Visual Studio 11\\Projects\\Project Warlock Server\\Project Warlock Server\\FrostParticle.lua")
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
	pPlayer->SetVelocity(dir * GetImpactImpulse());

	// Get item data.
	Item* item = pItemLoader->GetItem(ItemKey(GetSkillType(), GetSkillLevel()));

	// Damage the player.
	pPlayer->SetHealth(pPlayer->GetHealth() - item->GetAttributes().damage);
	pPlayer->SetLastHitter((Player*)GetWorld()->GetObjectById(GetOwner()));

	// Dead? [NOTE] A bit of a hack.
	if(pPlayer->GetHealth() <= 0) 
		pArena->PlayerEliminated(pPlayer, (Player*)GetWorld()->GetObjectById(GetOwner()));
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

StatusEffect* FrostProjectile::GetStatusEffect()
{
	return new FreezeEffect();
}