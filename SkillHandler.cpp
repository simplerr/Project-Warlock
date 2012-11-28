#include "SkillHandler.h"
#include "Input.h"
#include "Skills.h"

SkillHandler::SkillHandler()
{
	mSkillMap['Q'] = new FireBall();
	mActiveSkill = -1;
}

SkillHandler::~SkillHandler()
{
	for(auto iter = mSkillMap.begin(); iter != mSkillMap.end(); iter++)
		delete iter->second;
}

void SkillHandler::PollAction(Client* pClient, GLib::Input* pInput, XMFLOAT3 start, XMFLOAT3 end)
{
	if(pInput->KeyPressed('Q')) {
		mActiveSkill = 'Q';
	}

	// LBUTTON pressed, skill selected and inside the terrain.
	if(pInput->KeyPressed(VK_LBUTTON) && mActiveSkill != -1 && end.x != numeric_limits<float>::infinity())
	{
		// Use the skill.
		mSkillMap[mActiveSkill]->Cast(pClient, start, end);
		mActiveSkill = -1;
	}
}

bool SkillHandler::IsCastingSkill()
{
	return mActiveSkill == -1 ? false : true;
}