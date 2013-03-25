#include "Projectile.h"
#include "d3dUtil.h"
#include "Graphics.h"
#include "Skills.h"
#include "LuaWrapper.h"
#include "LightObject.h"
#include "World.h"

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

	mSpeed = GetLuaWrapper()->Get<float>("Projectile.speed");
	mMaxDistance = GetLuaWrapper()->Get<float>("Projectile.max_distance");
	mImpactImpulse = GetLuaWrapper()->Get<float>("Projectile.impact_impulse");
	mCastSound = GetLuaWrapper()->Get<string>("Projectile.cast_sound");
	mImpactSound = GetLuaWrapper()->Get<string>("Projectile.impact_sound");
}

Projectile::~Projectile()
{
	mLight->Kill();
}

void Projectile::Init()
{
	string color = GetLuaWrapper()->Get<string>("LightData.color");
	string intensity =GetLuaWrapper()->Get<string>("LightData.intensity");
	string att = GetLuaWrapper()->Get<string>("LightData.att");
	float spot = GetLuaWrapper()->Get<float>("LightData.spot");

	auto colorsSplit = GLib::SplitString(color, ' ');
	auto intensitySplit = GLib::SplitString(intensity, ' ');
	auto attSplit = GLib::SplitString(att, ' ');

	mLight = new GLib::LightObject();
	XMFLOAT4 colors = XMFLOAT4(atoi(colorsSplit[0].c_str()) / 255.0f, atoi(colorsSplit[1].c_str()) / 255.0f, atoi(colorsSplit[2].c_str()) / 255.0f, atoi(colorsSplit[3].c_str()) / 255.0f);
	mLight->SetMaterials(GLib::Material(colors));
	mLight->SetLightType((GLib::LightType)GLib::POINT_LIGHT);
	mLight->SetIntensity(atof(intensitySplit[0].c_str()), atof(intensitySplit[1].c_str()), atof(intensitySplit[2].c_str()));
	mLight->SetAtt(atof(attSplit[0].c_str()), atof(attSplit[1].c_str()), atof(attSplit[2].c_str()));
	mLight->SetSpot(spot);
	mLight->SetRange(1000);
	mLight->SetRotation(XMFLOAT3(0, -1, 0));
	GetWorld()->AddObject(mLight);
	mLight->SetId(mLight->GetId() + 900); // [HACK]
}

void Projectile::Update(float dt)
{
	ParticleSystem::Update(dt);

	XMFLOAT3 velocity = mDirection * mSpeed;
	SetPosition(GetPosition() + velocity);
	mTravelled += sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

	mLight->SetPosition(GetPosition());

	if(mTravelled > mMaxDistance)
		Kill();
}

void Projectile::Draw(GLib::Graphics* pGraphics)
{
	ParticleSystem::Draw(pGraphics);

	/*XNA::AxisAlignedBox box;
	box.Center = GetPosition();
	box.Extents = XMFLOAT3(0.4, 0.4, 0.4);
	pGraphics->DrawBoundingBox(&box, GetWorldMatrix(), GLib::Material(XMFLOAT4(1, 1, 1, 1)), 1.0f);*/
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
