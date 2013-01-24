#include "FreezeEffect.h"
#include "Player.h"
#include "Graphics.h"

FreezeEffect::FreezeEffect()
	: StatusEffect(FREEZE_STATUS)
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
	box.Extents = XMFLOAT3(3, 3, 3);
	pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::Blue));
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
}