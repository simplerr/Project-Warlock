#pragma once
#include <string>
using namespace std;

//! Name and IP of a server.
struct ServerData
{
	string host;
	string name;
	string publicIp;
	string localIp;
	int numPlayers;
};