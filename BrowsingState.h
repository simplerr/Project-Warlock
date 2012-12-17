#include "GameState.h"
#include "Database.h"

class ControlManager;
class Label;

class BrowsingState : public GameState
{
public:
	void Init(Game* pGame);
	void Cleanup(void);

	void Pause();
	void Resume();

	void MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(GLib::Input* pInput, double dt);
	void Draw(GLib::Graphics* pGraphics);

	static BrowsingState* Instance() {
		return &mBrowsingState;
	}

	void BuildUi();
	void OnServerPressed(Label* pLabel);
protected:
	BrowsingState() {};
private:
	static BrowsingState mBrowsingState;
	Database*			 mDatabase;
	ControlManager*		 mControlManager;
	map<string, ServerData>  mServerMap;
};