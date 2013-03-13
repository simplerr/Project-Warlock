#include "PlayerModule.h"
#include "Player.h"
#include "Input.h"
#include "World.h"
#include "Client.h"
#include "Graphics.h"
#include "Client.h"
#include "SkillHandler.h"
#include "StaticObject.h"
#include "ModelImporter.h"
#include "NetworkMessages.h"
#include "UserInterface.h"
#include "Sound.h"
#include <time.h>

PlayerModule::PlayerModule()
{
	mSkillHandler = new SkillHandler();
	mPlayer = nullptr;
	srand(time(0));
	mSoundDelta = -1;
}

PlayerModule::~PlayerModule()
{
	delete mSkillHandler;
}

void PlayerModule::Init()
{

}

void PlayerModule::Update(float dt)
{
	mSkillHandler->Update(dt);
	mSoundDelta -= dt;
}

void PlayerModule::PollAction(Client* pClient, GLib::Input* pInput)
{
	if(!mPlayer->GetEliminated() && !mPlayer->GetStunned())
	{
		// [TODO] Add mSkillHandler->PollAction().
		if(mSkillHandler->PollAction(pClient, pInput, mPlayer->GetPosition(), mPlayer->GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay())))
			mUserInterface->SetSelectedPlayer(this);

		// Add movement target for the selected object.
		if(pInput->KeyPressed(VK_RBUTTON))
		{
			// Don't move if the shop area was pressed.
			if(pClient->GetArenaState() == SHOPPING_STATE && pInput->MousePosition().x <= 320)
				return;

			XMFLOAT3 pos = mPlayer->GetWorld()->GetTerrainIntersectPoint(pInput->GetWorldPickingRay());

			// Inform the server about what happened.
			// The server then informs all the clients, including the callee.
			if(pos.x != numeric_limits<float>::infinity())
			{
				SendAddTarget(pClient, mPlayer->GetId(), pos, pInput->KeyDown(VK_SHIFT) ? false : true);

				if(mSoundDelta < 0)
				{
					// Play walking sound.
					int soundIndex = rand() % 12 + 1;
					char buffer[128];
					sprintf(buffer, "sounds/talk/%i.wav", soundIndex);
					gSound->PlayEffect(buffer);

					mSoundDelta = 3.5f;
				}
			}

			// Inform the UI to set the local player as selected. [TODO]
			mUserInterface->SetSelectedPlayer(this);
		}
	}
}

// Sent from client when a target is added.
void PlayerModule::SendAddTarget(Client* pClient, int id, XMFLOAT3 pos, bool clear)
{
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_TARGET_ADDED);
	bitstream.Write(pClient->GetName().c_str());
	bitstream.Write((unsigned char)id);
	bitstream.Write(pos.x);
	bitstream.Write(pos.y);
	bitstream.Write(pos.z);
	bitstream.Write(clear);
	pClient->SendServerMessage(bitstream);
}

bool PlayerModule::IsCastingSkill()
{
	return mSkillHandler->IsCastingSkill();
}

Skill* PlayerModule::AddSkill(ItemName skillName)
{
	return mSkillHandler->AddSkill(skillName);
}

void PlayerModule::RemoveSkill(ItemName name)
{
	mSkillHandler->RemoveSkill(name);
}

std::map<int, Skill*> PlayerModule::GetSkillMap()
{
	return mSkillHandler->GetSkillMap();
}

void PlayerModule::SetPlayer(Player* pPlayer)
{
	// Update the skill map here.
	mPlayer = pPlayer;
}

Player* PlayerModule::GetPlayer()
{
	return mPlayer;
}

void PlayerModule::SetUserInterface(UserInterface* pInterface)
{
	mUserInterface = pInterface;
}