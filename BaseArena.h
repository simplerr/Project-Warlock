#pragma once
#include <vector>
using namespace std;

namespace GLib {
	class World;
	class Object3D;
	class Input;
	class Graphics;
}

class Player;

class BaseArena
{
public:
	BaseArena();
	virtual ~BaseArena();

	virtual void PlayerEliminated(Player* pPlayer, Player* pEliminator) {};
protected:
	GLib::World*	mWorld;
	vector<Player*>	mPlayerList;
private:
};

