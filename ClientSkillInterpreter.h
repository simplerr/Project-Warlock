#pragma once
#include "BitStream.h"
#include "NetworkMessages.h"

class Server;
class Client;

class ClientSkillInterpreter
{
public:
	ClientSkillInterpreter();
	~ClientSkillInterpreter();

	void Interpret(Client* pClient, MessageId id, RakNet::BitStream& bitstream);
private:
};