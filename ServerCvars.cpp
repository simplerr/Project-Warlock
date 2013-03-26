#include "ServerCvars.h"

ServerCvars::ServerCvars() 
{
	CvarMap[Cvars::START_GOLD] = 100;
	CvarMap[Cvars::SHOP_TIME] = 3;
	CvarMap[Cvars::ROUND_TIME] = 120;
	CvarMap[Cvars::NUM_ROUNDS] = 1;
	CvarMap[Cvars::GOLD_PER_KILL] = 1;
	CvarMap[Cvars::GOLD_PER_WIN] = 3;
	CvarMap[Cvars::GOLD_PER_ROUND] = 4;
	CvarMap[Cvars::LAVA_DMG] = 1;
	CvarMap[Cvars::LAVA_SLOW] = 0.4;
	CvarMap[Cvars::PROJECTILE_IMPULSE] =  3; // Divided by 10 later.
	CvarMap[Cvars::ARENA_RADIUS] = 60;
	CvarMap[Cvars::FLOOD_INTERVAL] = 10;
	CvarMap[Cvars::FLOOD_SIZE] = 10;
	CvarMap[Cvars::CHEATS] = 1;
}

void ServerCvars::LoadFromFile(string filename) 
{
	ifstream fin(filename);
	string trash;
	float num;

	fin >> trash >> num;
	CvarMap[Cvars::START_GOLD] = num;
	fin >> trash >> num;
	CvarMap[Cvars::SHOP_TIME] = num;
	fin >> trash >> num;
	CvarMap[Cvars::ROUND_TIME] = num;
	fin >> trash >> num;
	CvarMap[Cvars::NUM_ROUNDS] = num;
	fin >> trash >> num;
	CvarMap[Cvars::GOLD_PER_KILL] = num;
	fin >> trash >> num;
	CvarMap[Cvars::GOLD_PER_WIN] = num;
	fin >> trash >> num;
	CvarMap[Cvars::GOLD_PER_ROUND] = num;
	fin >> trash >> num;
	CvarMap[Cvars::LAVA_DMG] = num;
	fin >> trash >> num;
	CvarMap[Cvars::LAVA_SLOW] = num;
	fin >> trash >> num;
	CvarMap[Cvars::PROJECTILE_IMPULSE] = num; // Divided by 10 later.
	fin >> trash >> num;
	CvarMap[Cvars::ARENA_RADIUS] = num;
	fin >> trash >> num;
	CvarMap[Cvars::FLOOD_INTERVAL] = num;
	fin >> trash >> num;
	CvarMap[Cvars::FLOOD_SIZE] = num;
	fin >> trash >> num;
	CvarMap[Cvars::CHEATS] = num;

	fin.close();
}