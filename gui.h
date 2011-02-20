#ifndef __FRAME_H__
#define __FRAME_H__
#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/spinctrl.h>
#undef Frame_STYLE
#define Frame_STYLE wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX

enum{
	BUTTON_ADD_TO_TRAY = 100,
	BUTTON_CHANGE_IP = 101,
	MENU_O_M = 102,
	MENU_INFO = 103,
	MENU_CHANGE_IP = 104,
	MENU_CLEAR_HISTORY = 105
};

class Frame : public wxFrame
{	
	public:
		Frame(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ip-changer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = Frame_STYLE);
		~Frame();
        void FrameClickEvents(wxCommandEvent& event);
		wxStatusBar *sb;
		wxSpinCtrl *host;
		wxComboBox *clients;
		wxComboBox *ip;
		
	private:
		wxPanel *WxPanel1;
        wxTaskBarIcon *trayIcon;
		void OnClose(wxCloseEvent& event);
DECLARE_EVENT_TABLE()
};

class TrayIcon: public wxTaskBarIcon
{
public:
    TrayIcon(Frame* frame);
	Frame* frame;
	virtual wxMenu* CreatePopupMenu();
private:
	wxMenu *menuTray;
	void TrayLeftClick(wxTaskBarIconEvent& event);
	void TrayClickEvents(wxCommandEvent& event);
DECLARE_EVENT_TABLE()
};
#endif
