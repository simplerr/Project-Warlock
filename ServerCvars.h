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
	const string PROJECTILE_IMPULSE = "-impulse";
}

struct ServerCvars
{
	ServerCvars() {
		CvarMap[Cvars::START_GOLD] = 13;
		CvarMap[Cvars::SHOP_TIME] = 3;
		CvarMap[Cvars::ROUND_TIME] = 120;
		CvarMap[Cvars::NUM_ROUNDS] = 5;
		CvarMap[Cvars::GOLD_PER_KILL] = 1;
		CvarMap[Cvars::GOLD_PER_WIN] = 3;
		CvarMap[Cvars::LAVA_DMG] = 1;
		CvarMap[Cvars::PROJECTILE_IMPULSE] =  3; // Divided by 10 later.
	}

	void LoadFromFile(string filename) {
		//ifstream fin(filename);
		//string trash;
		//int num;


		//fin >> trash >> num;
		//CvarMap[Cvars::START_GOLD] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::SHOP_TIME] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::ROUND_TIME] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::NUM_ROUNDS] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::GOLD_PER_KILL] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::GOLD_PER_WIN] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::LAVA_DMG] = num;
		//fin >> trash >> num;
		//CvarMap[Cvars::PROJECTILE_IMPULSE] = num; // Divided by 10 later.


		//fin.close();
	}

	void SetCvarValue(string cvar, int value) {
		if(CvarMap.find(cvar) != CvarMap.end())
			CvarMap[cvar] = value;
	}

	int GetCvarValue(string cvar) {
		if(CvarMap.find(cvar) != CvarMap.end())
			return CvarMap[cvar];
		else
			return 0;
	}

	map<string, int> CvarMap;
};

extern ServerCvars* gCvars;