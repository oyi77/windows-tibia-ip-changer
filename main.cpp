#include "main.h"
#include "gui.h"

int main(int argc, char **argv){
	wxApp::SetInstance(new IPChanger());
	return wxEntry(argc, argv);
}
 
bool IPChanger::OnInit()
{
    Frame *frame = new Frame(NULL);
    SetTopWindow(frame);
    frame->Show(true);
    return true;
}
