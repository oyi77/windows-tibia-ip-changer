#include "gui.h"
#include "clients.h"
#include "change.h"
#include "Images/Self_Frame_XPM.xpm"

BEGIN_EVENT_TABLE(Frame,wxFrame)
	EVT_CLOSE(Frame::OnClose)
	EVT_BUTTON(wxID_EXIT, Frame::FrameClickEvents)
	EVT_BUTTON(BUTTON_ADD_TO_TRAY, Frame::FrameClickEvents)
	EVT_MENU(MENU_INFO, Frame::FrameClickEvents)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK(TrayIcon::TrayLeftClick)
    EVT_MENU(wxID_EXIT, TrayIcon::TrayClickEvents)
    EVT_MENU(MENU_O_M, TrayIcon::TrayClickEvents)
END_EVENT_TABLE()

TrayIcon::TrayIcon(Frame* frame) 
: wxTaskBarIcon() 
, frame(frame) 
{ 
} 

Frame::Frame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	WxPanel1 = new wxPanel(this, -1, wxPoint(-5, 9), wxSize(360, 83));
	(void) new wxStaticText(WxPanel1, -1, wxT("IP Adress:"), wxPoint(9, 0), wxDefaultSize);
	(void) new wxStaticText(WxPanel1, -1, wxT("Port:"), wxPoint(237, 0), wxDefaultSize);
	(void) new wxStaticText(WxPanel1, -1, wxT("Tibia Version:"), wxPoint(288, 0));
	//wxArrayString arrayStringFor_ip;
	//ip = new wxComboBox(WxPanel1, -1, wxT(""), wxPoint(11, 17), wxSize(214, 21), arrayStringFor_ip, 0, wxDefaultValidator, wxT("ip"));
	ip = new wxTextCtrl(WxPanel1, -1, wxT(""), wxPoint(11, 17), wxSize(214, 21));
	wxArrayString arrayStringFor_clients;
	arrayStringFor_clients.Add(wxT("Auto"));
	for(int i = 0; i < int(sizeof(adresses)/sizeof(adresses[0])); i++){
        wxString wxs(adresses[i][0].insert(1, ".").c_str(), wxConvUTF8);
        arrayStringFor_clients.Add(wxs);
    }
	host = new wxTextCtrl(WxPanel1, -1, wxT("7171"), wxPoint(238, 17), wxSize(37, 21), wxTE_CENTRE);
	host->SetMaxLength(4);
    clients = new wxComboBox(WxPanel1, -1, wxT("Auto"), wxPoint(289, 17), wxSize(54, 21), arrayStringFor_clients, wxCB_READONLY);
	(void) new wxButton(WxPanel1, BUTTON_CHANGE_IP, wxT("Change IP"), wxPoint(81, 40), wxSize(87, 31), 0, wxDefaultValidator, wxT("change"));
    Connect(BUTTON_CHANGE_IP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Frame::FrameClickEvents));
	(void) new wxButton(WxPanel1, BUTTON_ADD_TO_TRAY, wxT("Add to Tray"), wxPoint(170, 40), wxSize(87, 31));
	(void) new wxButton(WxPanel1, wxID_EXIT, wxT("Exit"), wxPoint(259, 40), wxSize(87, 31));
	sb = new wxStatusBar(this, -1);

	wxMenuBar *menubar = new wxMenuBar();
	wxMenu *file = new wxMenu;
	//file->AppendSeparator(); 
	file->Append(wxID_EXIT, wxT("Exit"));
	menubar->Append(file, wxT("File"));
	wxMenu *help = new wxMenu;
	help->Append(MENU_INFO, wxT("Info"));
	menubar->Append(help, wxT("Help"));
	SetMenuBar(menubar);
    trayIcon = new TrayIcon(this);
    wxIcon icon(Self_Frame_XPM);
	if(!trayIcon->SetIcon(icon, wxT("IP Chabger")))
        wxMessageBox(wxT("Could not set icon."));
	SetStatusBar(sb);
	SetTitle(wxT("IP Changer"));
	SetIcon(Self_Frame_XPM);
	SetSize(8,8,362,147);
	Center();
}

Frame::~Frame(){ 
    delete trayIcon; 
} 

void sendDialog(wxString str, long styles){
	wxMessageDialog *dial = new wxMessageDialog(NULL, str, wxT("Informations"), styles);
	dial->ShowModal();
}

void guiChangeIP(Frame *frame, int eventId){
    Change c;
    HANDLE tibia = c.getTibiaProcess();
    if(!tibia){
        frame->sb->SetStatusText(wxT("Tibia process not found!"),0);
        if(eventId == MENU_CHANGE_IP)
            sendDialog(wxT("Tibia process not found!"), wxOK | wxICON_ERROR);
        CloseHandle(tibia);
        return;
    }
    std::string sip = std::string(frame->ip->GetValue().mb_str());
    std::string sport = std::string(frame->host->GetValue().mb_str());
    std::string client = std::string(frame->clients->GetValue().mb_str());
    if(sip == ""){
        frame->sb->SetStatusText(wxT("IP adress not found!"),0);
        if(eventId == MENU_CHANGE_IP)
        	sendDialog(wxT("IP adress not found!"), wxOK | wxICON_ERROR);
        return;
    }
        
    if(client != "Auto"){
        std::string s ="";
        s += client.at(1);
        if(s == ".")
        	client = client.erase(1,1);
    }
        	
    bool ret = c.changeIP(sip, atoi(sport.c_str()), client);
        
    if(ret){
        frame->sb->SetStatusText(wxT("IP Changed!"),0);
        if(eventId == MENU_CHANGE_IP)
        	sendDialog(wxT("IP Changed!"), wxOK);
    }else{
        frame->sb->SetStatusText(wxT("IP not Changed!"),0);
        if(eventId == MENU_CHANGE_IP)
        	sendDialog(wxT("IP not Changed!"), wxOK | wxICON_ERROR);
        return;
    }
}
void Frame::OnClose(wxCloseEvent& event){
	Iconize(true);
	Show(false);
}

void Frame::FrameClickEvents(wxCommandEvent& event)
{
    switch(event.GetId()){
        case BUTTON_CHANGE_IP:{
			guiChangeIP(this, event.GetId());
			break;
        }
        case wxID_EXIT:{
            trayIcon->RemoveIcon();
            Destroy();
            break;
        }
        case BUTTON_ADD_TO_TRAY:{
			Iconize(true);
			Show(false);
			break;
        }
        case MENU_INFO:{
			sendDialog(wxT("This program change IP tibia client in order to connect to Open Tibia Server.\nLicense: GNU GPL\nProgrammer: Miziak.\nAdress Finder: Virtelio.\nWrote in wxWidgets library."), wxOK);
			break;
        }
    }
}

void TrayIcon::TrayClickEvents(wxCommandEvent& event){ 
	switch(event.GetId()){
		case wxID_EXIT:{
			RemoveIcon();
			frame->Destroy();
			break;
		}
		case MENU_O_M:{
			if(frame->IsIconized()){
				frame->Iconize(false);
				frame->Show(true);
			}else{
				frame->Iconize(true);
				frame->Show(false);	
			}
			break;
		}
		case MENU_CHANGE_IP:{
			guiChangeIP(frame, event.GetId());
			break;
		}
	}
}

void TrayIcon::TrayLeftClick(wxTaskBarIconEvent& event){
	if(frame->IsIconized()){
		frame->Iconize(false);
		frame->Show(true);
	}else{
		frame->Iconize(true);
		frame->Show(false);	
	}
}

wxMenu* TrayIcon::CreatePopupMenu() 
{ 
    menuTray = new wxMenu();
    menuTray->Append(MENU_CHANGE_IP, wxT("Change IP"));
    Connect(MENU_CHANGE_IP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(TrayIcon::TrayClickEvents));
	if(frame->IsIconized())
		menuTray->Append(MENU_O_M, wxT("Open"));
	else
		menuTray->Append(MENU_O_M, wxT("Minimize"));
    menuTray->AppendSeparator(); 
    menuTray->Append(wxID_EXIT, wxT("Exit")); 

    return menuTray; 
}
