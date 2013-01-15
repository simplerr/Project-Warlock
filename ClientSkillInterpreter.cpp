#include "ClientSkillInterpreter.h"
#include "Client.h"
#include "World.h"
#include "FireProjectile.h"
#include "Skills.h"
#include "FrostProjectile.h"

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
	bitstream.Read(projectileId);
	bitstream.Read(skillType);
	bitstream.Read(skillLevel);
	bitstream.Read(start);
	bitstream.Read(end);

	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&start) - XMLoadFloat3(&start)));

	Projectile* projectile = nullptr;
	if(id == NMSG_ADD_FIREBALL)
		projectile = new FireProjectile(owner, start, dir);
	else if(id == NMSG_ADD_FROSTNOVA)
		projectile = new FrostProjectile(owner, start);

	projectile->SetSkillLevel(skillLevel);
	projectile->SetSkillLevel(skillType);
	projectile->SetOriginObject(world->GetObjectById(owner));
	world->AddObject(projectile);

	// Use the servers Id!
	projectile->SetId(projectileId);
}