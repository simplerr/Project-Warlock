#include "NetworkMessages.h"
#include "d3dUtil.h"
#include "BitStream.h"
#include "Chat.h"
#include <Windowsx.h>
#include <Richedit.h>
#include <Commctrl.h>
#include "Client.h"
#include "World.h"
#include "Object3D.h"

COLORREF colors[7] = {
	RGB(255, 0, 0),
	RGB(0, 255, 0),
	RGB(0, 0, 255),
	RGB(255, 255, 0),
	RGB(255, 0, 255),
	RGB(0, 0, 255),
	RGB(0, 255, 255),
};

void setFont(HWND hwnd, int height, int weight = FW_DONTCARE, string family="Arial")
{
	// Change to font in the input box.
	HFONT hFont = CreateFont (height, 0, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, family.c_str());

	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), true);
}

// Message IDs.
#define ID_SEND_MESSAGE 101

// The default edit control procedure.
WNDPROC DefEditProc;

// Ugly hack... NOTE: HACKCKKC
bool first = true;

//! Adds text to a richedit control.
/**
@param hwnd Handle to the control.
@param szTextIn The text to add.
@param crNewColor The text color.
@note Works on any window control, without the color effect.
*/
void Chat::AddText(char *szTextIn, COLORREF crNewColor)
{
	char *Text = (char *)malloc(lstrlen(szTextIn) + 5);
	CHARFORMAT cf;
	int iTotalTextLength = GetWindowTextLength(mhChatBox);
	int iStartPos = iTotalTextLength;
	int iEndPos;

	strcpy(Text, szTextIn);

	SendMessage(mhChatBox, EM_SETSEL, (WPARAM)(int)iTotalTextLength, (LPARAM)(int)iTotalTextLength);
	SendMessage(mhChatBox, EM_REPLACESEL, (WPARAM)(BOOL)FALSE, (LPARAM)(LPCSTR)Text);

	free(Text);

	cf.cbSize      = sizeof(CHARFORMAT);
	cf.dwMask      = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects   = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = crNewColor;

	iEndPos = GetWindowTextLength(mhChatBox);

	SendMessage(mhChatBox, EM_SETSEL, (WPARAM)(int)iStartPos, (LPARAM)(int)iEndPos);
	SendMessage(mhChatBox, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
	SendMessage(mhChatBox, EM_HIDESELECTION, (WPARAM)(BOOL)TRUE, (LPARAM)(BOOL)FALSE);

	SendMessage(mhChatBox, EM_LINESCROLL, (WPARAM)(int)0, (LPARAM)(int)1);
}

//! Subclassed msg proc for the input box.
LRESULT InputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	bool enter = false;
	switch (uMsg)
	{
	case WM_CHAR:
		// Enter was pressed.
		if(wParam == VK_RETURN)
			enter = true;
	default:
		// Handles all default actions.
		result =  CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
	}

	// Has to be here since case WM_CHAR is before the default procedure. 
	// Otherwise the caret positions gets set but then changed when the ENTER msg is proccessed.
	if(enter)
	{
		// Send the message so Chat can catch it.
		SendMessage(GLib::GetWindowHandler(), ID_SEND_MESSAGE, 0, 0);
	}

	return result;
}

//! Constructor.
Chat::Chat(int x, int y, int width, int height)
{
	// Needed
	InitCommonControls(); 

	SetClient(nullptr);

	// Create all the windows controls needed for the chat
	LoadLibrary("riched32.dll");
	mhChatBox = CreateWindowEx(WS_EX_CLIENTEDGE, "RichEdit", NULL, 
		WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
		x, y, width, height, GLib::GetWindowHandler(), (HMENU)IDC_CHAT_BOX, GLib::GetAppInstance(), NULL);

	mhSendButton = CreateWindow("BUTTON", "Send", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED,
		x+width-70, y+height+10, 70, 40, GLib::GetWindowHandler(), (HMENU)IDC_SEND_BUTTON, GLib::GetAppInstance(), NULL);

	mhInputBox = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | ES_MULTILINE | ES_AUTOVSCROLL,
		x, y+height+10, width-80, 40, GLib::GetWindowHandler(), (HMENU)IDC_INPUT_BOX, GLib::GetAppInstance(), NULL);

	// Set the char format for the chat box
	CHARFORMAT cf;
	cf.cbSize = sizeof (CHARFORMAT);  
	cf.dwMask = CFM_FACE; 
	strcpy(cf.szFaceName, "Arial"); 
	SendMessage(mhChatBox, EM_SETCHARFORMAT, (WPARAM)(UINT)0, (LPARAM)&cf);

	// Change the font in the input box
	setFont(mhInputBox, 16);

	// Set the default edit control proc
	DefEditProc = (WNDPROC)SetWindowLong(mhInputBox, GWL_WNDPROC, (DWORD)InputProc);

	// Add info text to the chat
	AddText("Welcome!\n", RGB(175, 77, 146));
}

//! Destructor.
Chat::~Chat()
{
	// Destroy all windows
	DestroyWindow(mhChatBox);
	DestroyWindow(mhInputBox);
	DestroyWindow(mhSendButton);
	first = true;
}

//! Adds a message to the chat.
void Chat::AddMessage(string from, string message)
{
	string name;
	string msg = message;
	if(first)
		name = "\n<";
	else
		name = "<";

	int playerId = mClient->GetWorld()->GetObjectByName(from)->GetId();

	COLORREF nameColor = colors[playerId % 7];	// [NOTE] size (7) is hardcoded!

	name += from;
	name += ">: ";
	AddText((char*)name.c_str(), nameColor);
	AddText((char*)message.c_str(), RGB(0, 0, 0));

	if(msg.find("\n") == string::npos)
		AddText("\n", RGB(0, 0, 0));

	first = false;
}

//! Send message to opponent and add it to your own chat.
void Chat::SendChatMessage()
{
	// Add the message to your own chat.
	int inputLen = GetWindowTextLength(mhInputBox) + 1;
	char* inputBuffer = new char[inputLen];
	GetWindowText(mhInputBox, inputBuffer, inputLen);

	// No empty messages.
	if(string(inputBuffer) == "\r\n")
		return;

	SetWindowText(mhInputBox, "");
	SetFocus(GLib::GetWindowHandler());

	// Send the message to the server.
	RakNet::BitStream bitstream;
	bitstream.Write((unsigned char)NMSG_CHAT_MESSAGE_SENT);
	bitstream.Write(mClient->GetName().c_str());
	bitstream.Write(inputBuffer);
	mClient->SendServerMessage(bitstream);

	// Call the callback function, letting it handle the sent message any way it wants to (admin commands etc...).
	if(!OnMessageSent.empty())
		OnMessageSent(string(inputBuffer));

	first = false;
	delete inputBuffer;
}

//! The message proc for the chat.
LRESULT Chat::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case ID_SEND_MESSAGE:
		SendChatMessage();
		break;
	case WM_COMMAND:
		// LOWORD(wParam) is the ID on the control window the message comes from.
		// lParam is a handle to the control window.
		if(LOWORD(wParam) == IDC_SEND_BUTTON)
			SendChatMessage();
		break;
	case WM_CHAR:
		if(wParam == VK_RETURN)
			SetFocus(mhInputBox);
		break;
	default:
		return 0;
	}
}

void Chat::HandleMessage(RakNet::BitStream& bitstream)
{
	// Handle the message and add it to the chat.
	char from[32];
	char message[256];
	bitstream.Read(from);
	bitstream.Read(message);
	AddMessage(string(from), string(message));
}

void Chat::SetClient(Client* pClient)
{
	mClient = pClient;
}

void Chat::SetDimensions(int x, int y, int width, int height)
{
	SetWindowPos(mhChatBox, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
	SetWindowPos(mhInputBox, HWND_TOP, x, y+height+10, width-80, 40, SWP_SHOWWINDOW);
	SetWindowPos(mhSendButton, HWND_TOP, x+width-70, y+height+10, 70, 40, SWP_SHOWWINDOW);
}

bool Chat::HasFocus()
{
	if(GetFocus() == mhInputBox)
		return true;
	else
		return false;
}