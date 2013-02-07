#include "Skills.h"
#include "Client.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "NetworkMessages.h"
#include "Graphics.h"
#include "Player.h"

//
//	Base skill
//

Skill::Skill(string icon) 
	: HudItem(icon), mCooldownCounter(0.0f)
{
	SetName(SKILL_FIREBALL);
	SetCastDelay(0.0f);
}

Skill::~Skill()
{

}

void Skill::Update(float dt) 
{
	mCooldownCounter -= dt;
}

void Skill::DrawIcon(GLib::Graphics* pGraphics, XMFLOAT2 pos, float size)
{
	pGraphics->DrawScreenQuad(GetIconTexture(), pos.x, pos.y, size, size);

	if(mCooldownCounter > 0.0f) {
		char buffer[244];
		sprintf(buffer, "%.2f", mCooldownCounter);
		pGraphics->DrawText(buffer, pos.x-10, pos.y-10, 14);
	}
}

void Skill::ResetCooldown() 
{
	mCooldownCounter = GetAttributes().cooldown;
}

void Skill::SetOwner(int owner) 
{
	mOwner = owner;
}

int Skill::GetOwner()
{
	return mOwner;
}

bool Skill::IsReady()
{
	return (mCooldownCounter <= 0.0f);
}

void Skill::SetCastDelay(float delay)
{
	mCastDelay = delay;
}

float Skill::GetCastDelay()
{
	return mCastDelay;
}

//
// Fireball
//

FireBall::FireBall(string icon) : Skill(icon)
{
	SetName(SKILL_FIREBALL);
}

FireBall::~FireBall()
{

}

void FireBall::Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end)
{
	
}

//
// Frost nova.
//

FrostNova::FrostNova(string icon) : Skill(icon)
{
	SetName(SKILL_FROSTNOVA);
}

FrostNova::~FrostNova()
{

}

void FrostNova::Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end)
{
	
}

//
// Teleport.
//

Teleport::Teleport(string icon) : Skill(icon)
{
	SetName(SKILL_TELEPORT);
	SetCastDelay(0.5f);
}

Teleport::Teleport()
	: Skill("#none")
{

}

Teleport::~Teleport()
{

}

void Teleport::Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end)
{
	XMFLOAT3 diff = end - pCaster->GetPosition();
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&diff)));
	float dist = sqrt(diff.x * diff.x + diff.z * diff.z);
	float range = 20.0f;

	if(dist < range)
		pCaster->SetPosition(end);
	else
		pCaster->SetPosition(start + dir * range);

	pCaster->ClearTargetQueue();
}

//
// Meteor.
//

Meteor::Meteor(string icon) : Skill(icon)
{
	SetName(SKILL_METEOR);
	SetCastDelay(0.5f);
}

Meteor::Meteor()
	: Skill("#none")
{

}

Meteor::~Meteor()
{

}

void Meteor::Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end)
{
	XMFLOAT3 diff = end - pCaster->GetPosition();
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&diff)));
	float dist = sqrt(diff.x * diff.x + diff.z * diff.z);
	float range = 20.0f;

	if(dist < range)
		pCaster->SetPosition(end);
	else
		pCaster->SetPosition(start + dir * range);

	pCaster->ClearTargetQueue();
}

Hook::Hook(string icon) : Skill(icon)
{
	SetName(SKILL_HOOK);
}

Hook::~Hook()
{

}

void Hook::Cast(GLib::World* pWorld, Player* pCaster, XMFLOAT3 start, XMFLOAT3 end)
{

}