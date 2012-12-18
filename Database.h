#pragma once
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Must include winsock2.h before mysql++.h
#include <winsock2.h>
#include "mysql++.h"

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

class Database
{
public:
	Database();
	~Database();

	void AddServer(string host, string name, string publicIp, string localIp);
	void RemoveServer(string host);
	void IncrementPlayerCounter(string host, int num);

	vector<ServerData> GetServers();
	string GetPublicIp();
	string GetLocalIp();
	ServerData GetServerData(string host);
private:
	mysqlpp::Connection mConnection;
};