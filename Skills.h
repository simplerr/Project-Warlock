#pragma once
#include "d3dUtil.h"

class Client;

enum SkillType 
{
	SKILL_FIREBALL,
	TEST_SKILL
};

class Skill
{
public:
	Skill() : mLevel(1), mType(SKILL_FIREBALL) {};
	virtual ~Skill() {};

	virtual void Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end) = 0;

	void SetOwner(int owner) {
		mOwner = owner;
	}

	int GetLevel() {return mLevel;}
	int GetOwner() {return mOwner;}
	SkillType GetType() {return mType;}
protected:
	SkillType	mType;
	int			mOwner;
	int			mLevel;
};

class FireBall : public Skill
{
public:
	FireBall();
	~FireBall();

	void Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end);
private:
};