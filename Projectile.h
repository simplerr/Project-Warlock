#pragma  once
#include "ParticleSystem.h"
#include "Skills.h"
#include "Items.h"

namespace GLib
{
	class Graphics;
}

class Projectile : public GLib::ParticleSystem
{
public:
	Projectile(int owner, XMFLOAT3 pos, XMFLOAT3 dir, string luaScript);
	~Projectile();

	void Update(float dt);
	void Draw(GLib::Graphics* pGraphics);

	void SetSkillType(ItemName type);
	void SetSkillLevel(int level);

	int		 GetOwner();
	XMFLOAT3 GetDirection();
	ItemName GetSkillType();
	int		 GetSkillLevel();
	float	 GetDamage();
private:
	XMFLOAT3	mDirection;
	int			mOwner;
	float		mSpeed;
	float		mMaxDistance;
	float		mTravelled;
	float		mDamage;

	// Skill data.
	ItemName	mSkillType;
	int			mSkillLevel;
};