#include "SkillHandler.h"
#include "Input.h"
#include "Skills.h"

SkillHandler::SkillHandler()
{
	mActiveSkill = -1;
}

SkillHandler::~SkillHandler()
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++)
		delete iter->second;
}

void SkillHandler::PollAction(Client* pClient, GLib::Input* pInput, XMFLOAT3 start, XMFLOAT3 end)
{
	if(pInput->KeyPressed('Q') && mSkillMap.find('Q') != mSkillMap.end()) 
		mActiveSkill = 'Q';

	// LBUTTON pressed, skill selected and inside the terrain.
	if(pInput->KeyPressed(VK_LBUTTON) && mActiveSkill != -1 && end.x != numeric_limits<float>::infinity())
	{
		// Use the skill.
		if(mSkillMap.find(mActiveSkill) != mSkillMap.end())
		{
			mSkillMap[mActiveSkill]->Cast(pClient, start, end);
			mActiveSkill = -1;
		}
	}
}

bool SkillHandler::IsCastingSkill()
{
	return mActiveSkill == -1 ? false : true;
}

Skill* SkillHandler::AddSkill(ItemName skillName)
{
	if(skillName == SKILL_FIREBALL)
	{
		mSkillMap['Q'] = new FireBall("textures/icons/fireball.png");
		return mSkillMap['Q'];
	}

	return nullptr;
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