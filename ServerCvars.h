#pragma once
#include <map>
#include <string>
#include <fstream>
using namespace std;

namespace Cvars {
	const string GIVE_GOLD = "-gold";
	const string CVAR_LIST_CMD = "-cvarlist";	// [NOTE] No Cvar!!
	const string RESTART_ROUND = "-restart";	// [NOTE] No Cvar!!
	const string START_GOLD = "-startgold";
	const string SHOP_TIME = "-shoptime";
	const string ROUND_TIME = "-roundtime";
	const string NUM_ROUNDS = "-rounds";
	const string GOLD_PER_KILL = "-goldperkill";
	const string GOLD_PER_WIN = "-goldperwin";
	const string LAVA_DMG = "-lavadmg";
	const string LAVA_SLOW = "-lavaslow";
	const string PROJECTILE_IMPULSE = "-impulse";
	const string ARENA_RADIUS = "-radius";
	const string FLOOD_INTERVAL = "-flood_interval";
	const string FLOOD_SIZE = "-flood_size";
	const string CHEATS = "-cheats";
}

struct ServerCvars
{
	ServerCvars();

	void LoadFromFile(string filename);

	void SetCvarValue(string cvar, float value) {
		if(CvarMap.find(cvar) != CvarMap.end())
			CvarMap[cvar] = value;
	}

	float GetCvarValue(string cvar) {
		if(CvarMap.find(cvar) != CvarMap.end())
			return CvarMap[cvar];
		else
			return 0;
	}

	map<string, float> CvarMap;
};

extern ServerCvars* gCvars;