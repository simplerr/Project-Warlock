#include "TeleportEffect.h"
#include "Player.h"
#include "Graphics.h"

TeleportEffect::TeleportEffect()
	: StatusEffect(TELEPORT_STATUS)
{
	SetDuration(0.5f);
}

TeleportEffect::~TeleportEffect()
{

}

void TeleportEffect::Update(float dt)
{
	StatusEffect::Update(dt);
}

void TeleportEffect::Draw(GLib::Graphics* pGraphics)
{
	XNA::AxisAlignedBox box;
	box.Center = GetPlayer()->GetPosition() + XMFLOAT3(0, 5, 0);
	box.Extents = XMFLOAT3(1, 1, 1);
	pGraphics->DrawBoundingBox(&box, XMMatrixIdentity(), GLib::Material(GLib::Colors::Red));
}

void TeleportEffect::Apply()
{
	
}

void TeleportEffect::Remove()
{
	
}