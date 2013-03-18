#include "PoisonEffect.h"
#include "Player.h"
#include "Graphics.h"

PoisonEffect::PoisonEffect(float duration, float slow, float dot)
	: StatusEffect(POISON_STATUS, 3, "textures/venom.png", "StatusEffectParticle.lua")
{
	mSlow = slow;
	mDamagePerSecond = dot;
	mDelta = 0.0f;
}

PoisonEffect::~PoisonEffect()
{

}

void PoisonEffect::Update(float dt)
{
	mDelta += dt;
	StatusEffect::Update(dt);

	GetPlayer()->SetSlow(mSlow);

	if(mDelta >= 1.0f) {
		GetPlayer()->TakeDamage(mDamagePerSecond);
		mDelta = 0.0f;
	}
}

void PoisonEffect::Draw(GLib::Graphics* pGraphics)
{
	XNA::AxisAlignedBox box;
	box.Center = GetPlayer()->GetPosition() + XMFLOAT3(0, 10, 0);
	box.Extents = XMFLOAT3(3, 3, 3);
	pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::Green));

	StatusEffect::Draw(pGraphics);
}

void PoisonEffect::Apply()
{
	GetPlayer()->SetSlow(mSlow);
}

void PoisonEffect::Remove()
{
	GetPlayer()->SetSlow(0.0f);
	StatusEffect::Remove();
}