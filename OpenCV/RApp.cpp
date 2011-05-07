/* 
 * File:   RApp.cpp
 * Author: jenicek
 * 
 * Created on July 26, 2010, 8:43 PM
 */

#include "RApp.h"

IMPLEMENT_APP(RApp);

bool RApp::OnInit()
{
    // Create an instance of main window
    RReconstWindow *reconstWnd = new RReconstWindow(NULL, _("Reconstruction"), wxPoint(50, 50), wxSize(800, 630));
    reconstWnd->Show(true);
    reconstWnd->Initialize((char*)"capture", 35.0f, -26.5f, -100.0f, 740.0f, 7.0f);
    SetTopWindow(reconstWnd);

    return true;
}