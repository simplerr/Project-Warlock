#include "MeteorProjectile.h"
#include "Particle.h"
#include "BaseArena.h"
#include "ServerCvars.h"
#include "ItemLoaderXML.h"
#include "Player.h"
#include "World.h"
#include "FreezeEffect.h"

MeteorProjectile::MeteorProjectile(int owner, XMFLOAT3 target)
	: Projectile(owner, target + XMFLOAT3(0, 40, 0), XMFLOAT3(0, -1, 0), "MeteorParticle.lua")
{

}

MeteorProjectile::~MeteorProjectile()
{

}

void MeteorProjectile::Draw(GLib::Graphics* pGraphics)
{
	Projectile::Draw(pGraphics);

	//pGraphics->DrawBoundingBox(&GetBoundingBox(), XMMatrixIdentity(), GLib::Material(GLib::Colors::Red));
}

void MeteorProjectile::HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader)
{
	// Add a "impulse" to the player.
	XMFLOAT3 dir = pPlayer->GetPosition() - GetPosition();
	dir.y = 0.0f;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
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

AxisAlignedBox MeteorProjectile::GetBoundingBox()
{
	return ParticleSystem::GetBoundingBox();

	// Uses one random particles distance from the center.
	GLib::Particle* particle = GetParticle(1);
	if(particle == nullptr)
		return XNA::AxisAlignedBox();

	XMFLOAT3 pos = particle->GetPosition();
	XMFLOAT3 diff = pos - GetPosition();
	float dist = sqrt(diff.x*diff.x + diff.y * diff.y + diff.z*diff.z);

	XNA::AxisAlignedBox box;
	box.Center = GetPosition();
	box.Extents = XMFLOAT3(dist, dist, dist);

	char buffer[244];
	sprintf(buffer, "DIST MFER: %f\n", dist);
	OutputDebugString(buffer);

	return box;
}

StatusEffect* MeteorProjectile::GetStatusEffect()
{
	return nullptr;
}