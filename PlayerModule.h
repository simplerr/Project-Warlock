#pragma once
#include "ItemLoaderXML.h"
#include <set>

namespace GLib {
	class StaticObject;
	class Graphics;
}

class Player;
class SkillHandler;
class Skill;
class Client;
class UserInterface;

class PlayerModule
{
public:
	PlayerModule();
	~PlayerModule();

	void Init();
	void Update(float dt);
	void PollAction(Client* pClient, GLib::Input* pInput);
	void SendAddTarget(Client* pClient, int id, XMFLOAT3 pos, bool clear);

	Skill* AddSkill(ItemName skillName);
	void RemoveSkill(ItemName name);
	std::map<int, Skill*> GetSkillMap();

	bool IsCastingSkill();
	void SetPlayer(Player* pPlayer);
	void SetUserInterface(UserInterface* pInterface);
	Player* GetPlayer();
private:
	Player*				  mPlayer;
	SkillHandler*		  mSkillHandler;	// Should it really be here?
	UserInterface*		  mUserInterface;
	float				  mSoundDelta;
};