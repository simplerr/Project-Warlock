#include "States.h"
#include "Graphics.h"
#include "Camera.h"
#include "ServerCvars.h"

void InitShoppingState(ArenaState& state, bool server)
{
	if(!server) {
		XMFLOAT3 pos = GLib::GetGraphics()->GetCamera()->GetPosition();
		GLib::GetGraphics()->GetCamera()->SetPosition(XMFLOAT3(-30, 150, 0));
		GLib::GetGraphics()->GetCamera()->SetTarget(XMFLOAT3(0, 0, 0));
	}
	
	state.elapsed = 0.0f;
	state.length = gCvars->GetCvarValue(Cvars::SHOP_TIME);
	state.state = SHOPPING_STATE;
}

void InitPlayingState(ArenaState& state, bool server, XMFLOAT3 cameraTarget)
{
	if(!server) {
		XMFLOAT3 pos = GLib::GetGraphics()->GetCamera()->GetPosition();
		GLib::GetGraphics()->GetCamera()->SetPosition(cameraTarget + XMFLOAT3(-30, 60, 0));
		GLib::GetGraphics()->GetCamera()->SetTarget(cameraTarget);
	}

	state.elapsed = 0.0f;
	state.length = PLAYING_TIME;
	state.state = PLAYING_STATE;
}