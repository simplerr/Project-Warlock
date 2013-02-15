#include "States.h"
#include "Graphics.h"
#include "Camera.h"

void InitShoppingState(ArenaState& state, bool server)
{
	if(!server) {
		XMFLOAT3 pos = GLib::GetGraphics()->GetCamera()->GetPosition();
		GLib::GetGraphics()->GetCamera()->SetPosition(XMFLOAT3(0, 150, 30));
	}
	
	state.elapsed = 0.0f;
	state.length = SHOPPING_TIME;
	state.state = SHOPPING_STATE;
}

void InitPlayingState(ArenaState& state, bool server)
{
	if(!server) {
		XMFLOAT3 pos = GLib::GetGraphics()->GetCamera()->GetPosition();
		GLib::GetGraphics()->GetCamera()->SetPosition(XMFLOAT3(0, 60, 30));
	}

	state.elapsed = 0.0f;
	state.length = PLAYING_TIME;
	state.state = PLAYING_STATE;
}