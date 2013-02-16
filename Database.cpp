#include "Database.h"
#include <Windows.h>
#include <Windows.h>
#include <wininet.h>
#include <sys/types.h>

#pragma comment(lib, "wininet")

#define USERNAME "104714-wn93145"
#define PASSWORD "b311m4ns1"
#define DATABASE "104714-banzai"
#define SERVER "mysql01.citynetwork.se"

Database::Database()
	: mConnection(false)
{
	mConnection.connect(DATABASE, SERVER, USERNAME, PASSWORD);

	// Check if connected TODO:
	if(!mConnection.connected())	{
		MessageBox(0, "Could not connect to database.", "Connection error!", MB_ICONEXCLAMATION);
	}
}

Database::~Database()
{

}

void Database::AddServer(string host, string name, string publicIp, string localIp)
{
	string q = "INSERT INTO Warlock_Servers VALUES ('"+host+"', '"+name+"', '"+publicIp+"', '"+localIp+"', '0')";
	//mConnection.query(q.c_str()).store();
	mysqlpp::Query query = mConnection.query(q);
	query.store();

	// [NOTE] For some reason this code makes it crash (smells like heap corruption):
	// The same thing happens in RemoveServer()
	//	mysqlpp::Query query = mConnection.query(q);
	//	mysqlpp::StoreQueryResult res = query.store();
}

void Database::RemoveServer(string host)
{
	string q = "DELETE FROM Warlock_Servers WHERE Host='"+host+"'";
	mConnection.query(q.c_str()).store();
}

void Database::IncrementPlayerCounter(string host, int num)
{
	// Get current wins.
	char buffer[256];
	int players = GetServerData(host).numPlayers;
	string q = "UPDATE Warlock_Servers SET Players="+string(itoa(players+num, buffer, 10))+" WHERE Host='"+host+"'";
	mysqlpp::Query query(&mConnection, true, q.c_str());
	query.exec();
}

ServerData Database::GetServerData(string host)
{
	string q = "SELECT * FROM Warlock_Servers";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();
	ServerData server;
	server.host = res[0]["Host"];
	server.name = res[0]["Name"];
	server.publicIp = res[0]["Public_Ip"];
	server.localIp = res[0]["Local_ip"];
	server.numPlayers = res[0]["Players"];

	return server;
}

vector<ServerData> Database::GetServers()
{
	vector<ServerData> servers;
	string q = "SELECT * FROM Warlock_Servers";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();
	if(res)
	{
		for(int i = 0; i < res.num_rows(); i++)
		{
			ServerData server;
			server.host = res[i]["Host"];
			server.name = res[i]["Name"];
			server.publicIp = res[i]["Public_Ip"];
			server.localIp = res[i]["Local_ip"];
			server.numPlayers = res[i]["Players"];
			servers.push_back(server);
		}
	}

	return servers;
}

string Database::GetPublicIp()
{
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char buffer[32];

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFile = InternetOpenUrl(hInternet, "http://auto.whatismyip.com/ip.php?user=simpler&password=automation_password", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
	buffer[rSize] = '\0';

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	return buffer;
}

string Database::GetLocalIp()
{
	char buffer[256];
	gethostname(buffer, 255);

	hostent* record = gethostbyname(buffer);
	in_addr* address=(in_addr*)record->h_addr;

	string ip = inet_ntoa(*address);
	return ip;
}