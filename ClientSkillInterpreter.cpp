#include "ClientSkillInterpreter.h"
#include "Client.h"
#include "World.h"
#include "Projectile.h"
#include "Skills.h"

ClientSkillInterpreter::ClientSkillInterpreter()
{

}

ClientSkillInterpreter::~ClientSkillInterpreter()
{

}

void ClientSkillInterpreter::Interpret(Client* pClient, MessageId id, RakNet::BitStream& bitstream)
{
	GLib::World* world = pClient->GetWorld();

	if(id == NMSG_ADD_FIREBALL)
	{
		XMFLOAT3 start, end, dir;
		int id, owner, skillLevel;
		ItemName skillType;

		bitstream.Read(owner);
		bitstream.Read(id);
		bitstream.Read(skillType);
		bitstream.Read(skillLevel);
		bitstream.Read(start);
		bitstream.Read(end);

		XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&start) - XMLoadFloat3(&start)));
		Projectile* projectile = new Projectile(owner, start, dir, "FireParticle.lua");
		projectile->SetSkillLevel(skillLevel);
		projectile->SetSkillLevel(skillType);
		world->AddObject(projectile);
		projectile->SetId(id);
	}
}