
#include "gui.h"
#include "animator.h"

Animator *AnimCube;
MainDlg  *Window;
QApplication *Application;
 
int main(int argc, char *argv[])
{ 
	Application = new QApplication(argc, argv);
	AnimCube = new Animator;
	Window = new MainDlg;
	
	Window->resize(800, 590);
	
	Application->setMainWidget(Window);
	Window->show();
	
	return Application->exec();
}
