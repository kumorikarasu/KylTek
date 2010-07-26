#include "winsock2.h"

#include "Main.h"
#include "GUI.h"
#include "CConsole.h"
#include "CLevel.h"
#include "CGlobal.h"
#include "CollisionManager.h"

SOCKET Client;
sockaddr_in CLIENT;
string hostIP;
WSADATA w;    // used to store information about WinSock version

void OnGUIEvent(UINT Event, CGUIBase* Src, void* Dest){

  UINT GUIType=Src->GetType();

  switch(GUIType){
  case GUITYPE_WINDOW:
    break;
  case GUITYPE_INPUTBOX:
    switch(Event){
  case EVENT_TOGGLEFULLSCREEN:
  case EVENT_NEWGAME:
  case EVENT_SAVEGAME:
  case EVENT_EXITGAME:
    break;
  case EVENT_CONSOLE:
    ((CConsole*)Dest)->Command(((CGUIInputBox*)Src)->GetText());
    ((CGUIInputBox*)Src)->SetText("");
    break;
  case EVENT_SETGLOBAL:
    break;
    }
    break;
  case GUITYPE_RADIO:
    switch(Event){
  case EVENT_SETGLOBAL:
    break;
    }
    break;
  case GUITYPE_CHECKBOX:
    switch(Event){
  case EVENT_TOGGLEFULLSCREEN:
    //fullscreen=((CGUICheckBox*)Src)->GetChecked();
    break;
  case EVENT_SETGLOBAL:
    break;
    }
    break;
  case GUITYPE_BUTTON:
    switch(Event){
  case EVENT_TOGGLEFULLSCREEN:
  case EVENT_NEWGAME:
    Src->GetParent()->OnFocusOut();
    Src->GetParent()->SetEnabled(false);
    Src->GetParent()->SetVisible(false);
    ((CLevel*)Dest)->DumpLevel();
    ((CLevel*)Dest)->LoadLevel("../Resources/Levels/level1.mel");
    Global->inGame=true;
    break;
    /*case EVENT_LOADGAME:
    ((CLevel*)Dest)->DumpLevel();
    ((CLevel*)Dest)->LoadLevel("../Resources/Levels/level1.mel");
    Global->inGame=true;
    break;
    */
  case EVENT_NETWORK:
    //Open Netplay Window
    //DEBUG

    //INI WINSOCK


    WSAStartup (0x0202, &w);   // Fill in w

    //CONNECT TO MASTER SERVER

    hostIP = "99.236.225.103";

    Client =  socket (AF_INET, SOCK_DGRAM, 0);; // Create socket
    CLIENT.sin_family = AF_INET;           // address family Internet
    CLIENT.sin_port = htons (7500);        // set server’s port number
    CLIENT.sin_addr.s_addr = inet_addr (hostIP.c_str());  // set server’s IP

    /*if (connect(Client, (struct sockaddr *) &CLIENT, sizeof(CLIENT)) == SOCKET_ERROR)
    { // an error connecting has occurred!
    cout << "Error!\n";
    if (WSAGetLastError() == 10060)
    cout << "Connection Timed out";
    else
    cout << WSAGetLastError();
    WSACleanup ();
    }
    */
    //END DEBUG

    break;
  case EVENT_SAVEGAME:
    break;
  case EVENT_EXITGAME:
    Global->Exit=true;
    break;
  case EVENT_OPENWINDOW:
    ((CGUIWindow*)Dest)->SetEnabled(true);
    ((CGUIWindow*)Dest)->RequestFocus();
    break;
  case EVENT_CLOSEWINDOW:
    ((CGUIWindow*)Dest)->OnFocusOut();
    ((CGUIWindow*)Dest)->SetEnabled(false);
    ((CGUIWindow*)Dest)->SetVisible(false);
    break;
  case EVENT_SWITCHTABS:
    ((CGUITabGroup*)Dest)->ActivateTab(((CGUIButton*)Src)->GetText());
    break;
  case EVENT_SCROLLLEFT:
    {
      CGUIHorizontalScrollBar* dest;
      dest=((CGUIHorizontalScrollBar*)Dest);
      dest->Scroll(-(dest->GetMaxValue()/(dest->GetMaxBarWidth()-dest->GetBarWidth())));
    }
    break;
  case EVENT_SCROLLRIGHT:
    {
      CGUIHorizontalScrollBar* dest;
      dest=((CGUIHorizontalScrollBar*)Dest);
      dest->Scroll(dest->GetMaxValue()/(dest->GetMaxBarWidth()-dest->GetBarWidth()));
    }
    break;
  case EVENT_SCROLLUP:
    {
      CGUIVerticalScrollBar* dest;
      dest=((CGUIVerticalScrollBar*)Dest);
      dest->Scroll(-(dest->GetMaxValue()/(dest->GetMaxBarHeight()-dest->GetBarHeight())));
    }
    break;
  case EVENT_SCROLLDOWN:
    {
      CGUIVerticalScrollBar* dest;
      dest=((CGUIVerticalScrollBar*)Dest);
      dest->Scroll(dest->GetMaxValue()/(dest->GetMaxBarHeight()-dest->GetBarHeight()));
    }
    break;
    }
    break;
  case GUITYPE_TABGROUP:
    break;
  }
}