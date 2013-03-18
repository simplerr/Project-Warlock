#include "FreezeEffect.h"
#include "Player.h"
#include "Graphics.h"
#include "ParticleSystem.h"

FreezeEffect::FreezeEffect()
	: StatusEffect(FREEZE_STATUS, 3, "textures/smoke.dds", "StatusEffectParticle.lua")
{

}

FreezeEffect::~FreezeEffect()
{

}

void FreezeEffect::Update(float dt)
{
	StatusEffect::Update(dt);
}

void FreezeEffect::Draw(GLib::Graphics* pGraphics)
{
	XNA::AxisAlignedBox box;
	box.Center = GetPlayer()->GetPosition() + XMFLOAT3(0, 10, 0);
	box.Extents = XMFLOAT3(0.5f, 0.5f, 0.5f);
	pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::Blue));

	StatusEffect::Draw(pGraphics);
}

void FreezeEffect::Apply()
{
	mOriginalMovementSpeed = GetPlayer()->GetMovementSpeed();
	//GetPlayer()->SetStunned(true);
	GetPlayer()->SetMovementSpeed(mOriginalMovementSpeed / 2.0f);
}

void FreezeEffect::Remove()
{
	//GetPlayer()->SetStunned(false);
	GetPlayer()->SetMovementSpeed(mOriginalMovementSpeed);
	StatusEffect::Remove();
}