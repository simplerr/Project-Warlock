#pragma  once
#include "ParticleSystem.h"
#include "Skills.h"
#include "Items.h"

namespace GLib {
	class Graphics;
	class LightObject;
}

class Player;
class BaseArena;
class ItemLoaderXML;
class StatusEffect;

class Projectile : public GLib::ParticleSystem
{
public:
	Projectile(int owner, XMFLOAT3 pos, XMFLOAT3 dir, string luaScript);
	virtual ~Projectile();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(GLib::Graphics* pGraphics);

	virtual void HandlePlayerCollision(Player* pPlayer, BaseArena* pArena, ItemLoaderXML* pItemLoader) = 0;

	void SetSkillType(ItemName type);
	void SetSkillLevel(int level);
	void SetPosition(XMFLOAT3 position);
	void SetSpeed(float speed);
	void SetMaxDistance(float distance);

	int		 GetOwner();
	XMFLOAT3 GetDirection();
	ItemName GetSkillType();
	int		 GetSkillLevel();
	float	 GetDamage();
	float	 GetImpactImpulse();
	virtual StatusEffect* GetStatusEffect(ItemLoaderXML* pItemLoader) {return nullptr;}
	string	 GetCastSound();
	string	 GetImpactSound();
private:
	XMFLOAT3	mDirection;
	int			mOwner;
	float		mSpeed;
	float		mMaxDistance;
	float		mTravelled;
	float		mDamage;
	float		mImpactImpulse;
	string		mCastSound;
	string		mImpactSound;
	int			mLightId;

	// Skill data.
	ItemName	mSkillType;
	int			mSkillLevel;
};