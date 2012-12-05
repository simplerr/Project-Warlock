#pragma once
#include "d3dUtil.h"
#include "Items.h"

class Client;

class Skill : public BaseItem
{
public:
	Skill(string icon);
	virtual ~Skill();

	void Update(float dt);
	void DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size);
	virtual void Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end) = 0;

	void ResetCooldown();
	void SetOwner(int owner);

	int GetOwner();
	bool IsReady();
protected:
	int			mOwner;
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