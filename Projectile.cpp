#include "Projectile.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Skills.h"
#include "LuaWrapper.h"

Projectile::Projectile(int owner, XMFLOAT3 pos, XMFLOAT3 dir, string luaScript)
	: ParticleSystem(pos, luaScript)
{
	SetMaterials(GLib::Material(GLib::Colors::Red));
	SetType(GLib::PROJECTILE);
	SetSkillType(SKILL_FIREBALL);
	SetSkillLevel(1);
	mDirection = dir;
	mTravelled = 0.0f;
	mOwner = owner;

	mSpeed = GetLuaWrapper()->GetTableNumber("Projectile", "speed");
	mMaxDistance = GetLuaWrapper()->GetTableNumber("Projectile", "max_distance");
	mImpactImpulse = GetLuaWrapper()->GetTableNumber("Projectile", "impact_impulse");
	mCastSound = GetLuaWrapper()->GetTableString("Projectile", "cast_sound");
	mImpactSound = GetLuaWrapper()->GetTableString("Projectile", "impact_sound");
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

void Projectile::SetPosition(XMFLOAT3 position)
{
	Object3D::SetPosition(position);
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

void Projectile::SetSpeed(float speed)
{
	mSpeed = speed;
}

void Projectile::SetMaxDistance(float distance)
{
	mMaxDistance = distance;
}

float Projectile::GetImpactImpulse()
{
	return mImpactImpulse;
}

string Projectile::GetCastSound()
{
	return mCastSound;
}

string Projectile::GetImpactSound()
{
	return mImpactSound;
}
