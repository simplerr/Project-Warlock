#pragma once
#include <Windows.h>
#include <string>
#include <boost\function.hpp>
#include <boost\bind.hpp>
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
	void	AddText(char *szTextIn, COLORREF crNewColor);
	void	HandleMessage(RakNet::BitStream& bitstream);

	void	SetClient(Client* pClient);
	void	SetDimensions(int x, int y, int width, int height);

	bool	HasFocus();

	template <class T>
	void AddOnMessageSentListener(void(T::*_callback)(string), T* _object)	{
		OnMessageSent = boost::bind(_callback, _object, _1);
	}
private:
	// Callback.
	boost::function<void(string)> OnMessageSent;

	// Window controls.
	HWND mhChatBox;
	HWND mhInputBox;
	HWND mhSendButton;

	Client* mClient;
};