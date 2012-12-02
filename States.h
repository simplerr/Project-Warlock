#pragma once

const float SHOPPING_TIME = 20.0f;
const float PLAYING_TIME = 120.0f;

enum GameState
{
	PLAYING_STATE,
	SHOPPING_STATE
};

struct ArenaState
{
	GameState state;
	float length;
	float elapsed;
};

void InitShoppingState(ArenaState& state);
void InitPlayingState(ArenaState& state);