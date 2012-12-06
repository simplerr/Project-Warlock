#pragma once
#include <Windows.h>
#include <string>
#include "RakPeerInterface.h"
using namespace std;

#define IDC_SEND_BUTTON 101
#define IDC_CHAT_BOX	102
#define IDC_INPUT_BOX	103

class Client;

//! Contains the chat components.
class Chat
{
public:
	Chat(int x, int y, int width, int height);
	~Chat();

	void AddMessage(string from, string message);
	void SendChatMessage();

	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void	AddText(HWND hwnd, char *szTextIn, COLORREF crNewColor);
	void	HandleMessage(RakNet::BitStream& bitstream);

	void	SetClient(Client* pClient);
private:
	// Window controls.
	HWND mhChatBox;
	HWND mhInputBox;
	HWND mhSendButton;

	Client* mClient;
};