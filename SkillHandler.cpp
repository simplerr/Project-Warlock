#include "SkillHandler.h"
#include "Input.h"
#include "Skills.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "NetworkMessages.h"
#include "Client.h"
#include "Player.h"

SkillHandler::SkillHandler()
{
	mActiveSkill = -1;
}

SkillHandler::~SkillHandler()
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++)
		delete iter->second;
}

void SkillHandler::Update(float dt)
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++)
		(*iter).second->Update(dt);
}

void SkillHandler::PollAction(Client* pClient, GLib::Input* pInput, XMFLOAT3 start, XMFLOAT3 end)
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++) {
		Skill* skill = (*iter).second;
		char key = (*iter).first;
		if(pInput->KeyPressed(key) && skill->IsReady())
		{
			// Send a skill casted event to the server.
			RakNet::BitStream bitstream;

			bitstream.Write((unsigned char)NMSG_SKILL_CAST);
			bitstream.Write((unsigned char)skill->GetName());
			bitstream.Write(pClient->GetLocalPlayer()->GetId());
			bitstream.Write(skill->GetName());
			bitstream.Write(skill->GetLevel());
			bitstream.Write(start);
			bitstream.Write(end);

			pClient->SendServerMessage(bitstream);

			skill->ResetCooldown();
		}
	}
}

bool SkillHandler::IsCastingSkill()
{
	return mActiveSkill == -1 ? false : true;
}

Skill* SkillHandler::AddSkill(ItemName skillName)
{
	Skill* skill = 0;
	char key;

	// Create the specific skill at the right key.
	if(skillName == SKILL_FIREBALL) {
		skill = new FireBall("textures/icons/fireball.png");
		key = 'Q';
	}
	else if(skillName == SKILL_FROSTNOVA) {
		skill = new FrostNova("textures/icons/frost_nova.png");
		key = 'W';
	}
	else if(skillName == SKILL_TELEPORT) {
		skill = new Teleport("textures/icons/leap.png");
		key = 'E';
	}
	else if(skillName == SKILL_METEOR) {
		skill = new Meteor("textures/icons/meteor.png");
		key = 'R';
	}
	else if(skillName == SKILL_HOOK) {
		skill = new Hook("textures/icons/hook.png");
		key = 'A';
	}
	else
		return nullptr;

	mSkillMap[key] = skill;
	return skill;
}

void SkillHandler::RemoveSkill(ItemName name)
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++)
	{
		if((*iter).second->GetName() == name) {
			mSkillMap.erase(iter);
			break;
		}
	}
}

std::map<int, Skill*> SkillHandler::GetSkillMap()
{
	return mSkillMap;
}