#pragma once
#include "d3dUtil.h"
#include "Items.h"

class Client;
class Player;

namespace GLib {
	class World;
}

class Skill : public HudItem
{
public:
	Skill(string icon);
	virtual ~Skill();

	void Update(float dt);
	void DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);
	virtual void Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end) = 0;

	void ResetCooldown();
	void SetOwner(int owner);
	void SetCastDelay(float delay);
	float GetCastDelay();

	int GetOwner();
	bool IsReady();
protected:
	int			mOwner;
	float		mCooldownCounter;
	float		mCastDelay;
};

// Remake this, this isnt the actual skill....
class FireBall : public Skill
{
public:
	FireBall(string icon);
	~FireBall();

	void Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end);
};

class FrostNova : public Skill
{
public:
	FrostNova(string icon);
	~FrostNova();

	void Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end);
};

class Teleport : public Skill
{
public:
	Teleport(string icon);
	Teleport();
	~Teleport();

	void Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end);
};

class Meteor : public Skill
{
public:
	Meteor(string icon);
	Meteor();
	~Meteor();

	void Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end);
};