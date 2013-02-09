#pragma once
#include "d3dUtil.h"
#include <map>
#include "Items.h"
#include "Skills.h"

namespace GLib {
	class Input;
}

class Skill;
class Client;
class UserInterface;

class SkillHandler
{
public:
	SkillHandler();
	~SkillHandler();

	void Update(float dt);
	bool PollAction(Client* pClient, GLib::Input* pInput, XMFLOAT3 position, XMFLOAT3 end);
	bool IsCastingSkill();
	Skill* AddSkill(ItemName skillName);
	void RemoveSkill(ItemName name);
	std::map<int, Skill*> GetSkillMap();
private:
	std::map<int, Skill*> mSkillMap;
	int mActiveSkill;
};