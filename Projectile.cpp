#include "Projectile.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Skills.h"

Projectile::Projectile(int owner, XMFLOAT3 pos, XMFLOAT3 dir, string luaScript)
	: ParticleSystem(pos, luaScript)
{
	SetMaterials(GLib::Material(GLib::Colors::Red));
	SetType(GLib::PROJECTILE);
	SetSkillType(SKILL_FIREBALL);
	SetSkillLevel(1);
	mDirection = dir;
	mMaxDistance = 40.0f;
	mOwner = owner;
	mSpeed = 0.2f;
	mTravelled = 0.0f;
}

Projectile::~Projectile()
{

}

void Projectile::Update(float dt)
{
	ParticleSystem::Update(dt);

	XMFLOAT3 velocity = mDirection * mSpeed;
	SetPosition(GetPosition() + velocity);
	mTravelled += sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

	if(mTravelled > mMaxDistance)
		Kill();
}

void Projectile::Draw(GLib::Graphics* pGraphics)
{
	ParticleSystem::Draw(pGraphics);
}

int Projectile::GetOwner()
{
	return mOwner;
}

XMFLOAT3 Projectile::GetDirection()
{
	return mDirection;
}

void Projectile::SetSkillType(ItemName type)
{
	mSkillType = type;
}

void Projectile::SetSkillLevel(int level)
{
	mSkillLevel = level;
}

ItemName Projectile::GetSkillType()
{
	return mSkillType;
}

int Projectile::GetSkillLevel()
{
	return mSkillLevel;
}