#include "Player.h"
#include "Input.h"
#include "World.h"
#include "Client.h"
#include "Graphics.h"
#include "Client.h"
#include "SkillHandler.h"

Player::Player()
	: Actor(GLib::GetGraphics()->GetModelImporter(), "models/smith/smith.x")
{
	SetType(GLib::PLAYER);
	SetHealth(100.0f);
	mSkillHandler = new SkillHandler();
}

Player::~Player()
{
	delete mSkillHandler;
}

void Player::Init()
{

}

void Player::Update(float dt)
{
	Actor::Update(dt);
}

void Player::Draw(GLib::Graphics* pGraphics)
{
	Actor::Draw(pGraphics);
}

void Player::PollAction(Client* pClient, GLib::Input* pInput)
{
	// [TODO] Add mSkillHandler->PollAction().
	mSkillHandler->PollAction(pClient, pInput, GetPosition(), GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay()));

	if(pInput->KeyPressed('W'))
		SetVelocity(XMFLOAT3(0, 0, 0.1f));

	// Add movement target for the selected object.
	if(pInput->KeyPressed(VK_RBUTTON))
	{
		XMFLOAT3 pos = GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

		// Inform the server about what happened.
		// The server then informs all the clients, including the callee.
		if(pos.x != numeric_limits<float>::infinity())
			pClient->SendAddTarget(GetId(), pos, pInput->KeyDown(VK_SHIFT) ? false : true);
	}
}

void Player::SetSystemAdress(RakNet::SystemAddress adress)
{
	mSystemAdress = adress;
}

RakNet::SystemAddress Player::GetSystemAdress()
{
	return mSystemAdress;
}

void Player::SetHealth(float health)
{
	mHealth = health;
}

float Player::GetHealth()
{
	return mHealth;
}

bool Player::IsCastingSkill()
{
	return mSkillHandler->IsCastingSkill();
}