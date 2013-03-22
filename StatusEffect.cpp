#include "StatusEffect.h"
#include "Player.h"
#include "World.h"

StatusEffect::StatusEffect(ItemName type, float duration, string particleTexture, string luaScript)
{
	// duration+1 because the particle system shouldn't be deleted before the status effect [HACK][NOTE]
	mParticleSystem = new GLib::ParticleSystem(XMFLOAT3(0, 0, 0), luaScript, particleTexture, duration+3);	
	mTimer = 0.0f;
	mType = type;
	mDuration = duration;
}

StatusEffect::~StatusEffect() 
{
	delete mParticleSystem;
}

void StatusEffect::DeleteParticleSystem()
{
	delete mParticleSystem;
}

void StatusEffect::Remove()
{
	mParticleSystem->Kill();
}

void StatusEffect::Update(float dt)
{
	mTimer += dt;
	mParticleSystem->Update(dt);
	mParticleSystem->SetPosition(mPlayer->GetPosition() + XMFLOAT3(0, 3, 0));
}

void StatusEffect::Draw(GLib::Graphics* pGraphics)
{
	mParticleSystem->Draw(pGraphics);
}

void StatusEffect::SetPlayer(Player* pPlayer) 
{
	mPlayer = pPlayer;
}

void StatusEffect::SetDuration(float duration) 
{
	mDuration = duration;
}

float StatusEffect::GetDuration() 
{
	return mDuration;
}

float StatusEffect::GetTimer() 
{
	return mTimer;
}

ItemName StatusEffect::GetType() 
{
	return mType;
}

Player* StatusEffect::GetPlayer() 
{
	return mPlayer;
}