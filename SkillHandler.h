#pragma once
#include "d3dUtil.h"
#include <map>

namespace GLib {
	class Input;
}

class Skill;
class Client;

class SkillHandler
{
public:
	SkillHandler();
	~SkillHandler();

	void PollAction(Client* pClient, GLib::Input* pInput, XMFLOAT3 position, XMFLOAT3 end);
private:
	std::map<int, Skill*> mSkillMap;
	int mActiveSkill;
};