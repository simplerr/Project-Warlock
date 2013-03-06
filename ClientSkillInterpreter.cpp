#include "ClientSkillInterpreter.h"
#include "Client.h"
#include "World.h"
#include "FireProjectile.h"
#include "Skills.h"
#include "FrostProjectile.h"
#include "Player.h"
#include "TeleportEffect.h"
#include "MeteorProjectile.h"
#include "HookProjectile.h"
#include "Sound.h"

ClientSkillInterpreter::ClientSkillInterpreter()
{

}

ClientSkillInterpreter::~ClientSkillInterpreter()
{

}

void ClientSkillInterpreter::Interpret(Client* pClient, MessageId id, RakNet::BitStream& bitstream)
{
	GLib::World* world = pClient->GetWorld();

	XMFLOAT3 start, end, dir;
	int projectileId, owner, skillLevel;
	ItemName skillType;

	bitstream.Read(owner);
	bitstream.Read(skillType);
	bitstream.Read(skillLevel);
	bitstream.Read(start);
	bitstream.Read(end);

	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&start) - XMLoadFloat3(&start)));

	Projectile* projectile = nullptr;
	if(id == SKILL_FIREBALL) {
		projectile = new FireProjectile(owner, start, dir);
		gSound->PlayEffect("sounds/fireball.wav");
	}
	else if(id == SKILL_HOOK) {
		projectile = new HookProjectile(owner, start, dir);
		gSound->PlayEffect("sounds/blinkarrival.wav");
	}
	else if(id == SKILL_FROSTNOVA) {
		projectile = new FrostProjectile(owner, start);
		gSound->PlayEffect("sounds/frostnova.wav");
	}
	else if(id == SKILL_METEOR) {
		projectile = new MeteorProjectile(owner, end);
		gSound->PlayEffect("sounds/blinkarrival.wav");
	}
	else if(id == SKILL_TELEPORT) 
	{
		Player* player = ((Player*)world->GetObjectById(owner));
		player->ClearTargetQueue();

		// Add a teleport status effect (just for visuals).
		player->AddStatusEffect(new TeleportEffect());

		gSound->PlayEffect("sounds/blinkarrival.wav");
	}

	if(id == SKILL_FIREBALL || id == SKILL_FROSTNOVA || id == SKILL_METEOR || id == SKILL_HOOK) 
	{
		bitstream.Read(projectileId);
		projectile->SetSkillLevel(skillLevel);
		projectile->SetSkillLevel(skillType);
		projectile->SetOriginObject(world->GetObjectById(owner));
		world->AddObject(projectile);

		// Use the servers Id!
		projectile->SetId(projectileId);
	}
}