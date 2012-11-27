#include "Skills.h"
#include "Client.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "NetworkMessages.h"

FireBall::FireBall()
{
	mType = SKILL_FIREBALL;
}

FireBall::~FireBall()
{

}

void FireBall::Cast(Client* pClient, XMFLOAT3 start, XMFLOAT3 end)
{
	// Tell the server to add a fireball.
	RakNet::BitStream bitstream;

	bitstream.Write((unsigned char)NMSG_SKILL_CAST);
	bitstream.Write((unsigned char)NMSG_ADD_FIREBALL);
	bitstream.Write(pClient->GetPlayerId());
	bitstream.Write(GetType());
	bitstream.Write(GetLevel());
	bitstream.Write(start);
	bitstream.Write(end);

	pClient->SendServerMessage(bitstream);
}