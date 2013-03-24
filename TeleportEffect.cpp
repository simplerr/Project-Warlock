#include "TeleportEffect.h"
#include "Player.h"
#include "Graphics.h"

TeleportEffect::TeleportEffect()
	: StatusEffect(TELEPORT_STATUS, 3, "data/textures/smoke.dds", "data/lua/StatusEffectParticle.lua")
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

	StatusEffect::Draw(pGraphics);
}

void TeleportEffect::Apply()
{
	
}

void TeleportEffect::Remove()
{
	StatusEffect::Remove();
}