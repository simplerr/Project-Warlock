#include "Skills.h"
#include "Client.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "NetworkMessages.h"
#include "Graphics.h"

//
//	Base skill
//

Skill::Skill(string icon) 
	: BaseItem(icon), mCooldown(1.0f), mCooldownCounter(0.0f)
{
	SetName(SKILL_FIREBALL);
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
	mCooldownCounter = mCooldown;
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

void FireBall::Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end)
{
	// Tell the server to add a fireball.
	RakNet::BitStream bitstream;

	bitstream.Write((unsigned char)NMSG_SKILL_CAST);
	bitstream.Write((unsigned char)NMSG_ADD_FIREBALL);
	bitstream.Write(pClient->GetPlayerId());
	bitstream.Write(GetName());
	bitstream.Write(GetLevel());
	bitstream.Write(start);
	bitstream.Write(end);

	pClient->SendServerMessage(bitstream);
}