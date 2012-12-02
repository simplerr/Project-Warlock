#include "States.h"

void InitShoppingState(ArenaState& state)
{
	state.elapsed = 0.0f;
	state.length = SHOPPING_TIME;
	state.state = SHOPPING_STATE;
}

void InitPlayingState(ArenaState& state)
{
	state.elapsed = 0.0f;
	state.length = PLAYING_TIME;
	state.state = PLAYING_STATE;
}