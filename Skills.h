#pragma once
#include "d3dUtil.h"
#include "Items.h"

class Client;

class Skill : public BaseItem
{
public:
	Skill(string icon) : BaseItem(icon), mLevel(1), mCooldown(3.0f), mCooldownCounter(0.0f) {
		SetName(SKILL_FIREBALL);
	};
	virtual ~Skill() {};

	void Update(float dt) {
		mCooldownCounter -= dt;
	}

	virtual void Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end) = 0;
	void ResetCooldown() {
		mCooldownCounter = mCooldown;
	}

	void Draw(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);

	void SetOwner(int owner) {
		mOwner = owner;
	}

	int GetLevel() {return mLevel;}
	int GetOwner() {return mOwner;}
protected:
	int			mOwner;
	int			mLevel;
	float		mCooldown;
	float		mCooldownCounter;
};

class FireBall : public Skill
{
public:
	FireBall(string icon);
	~FireBall();

	void Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end);
private:
};