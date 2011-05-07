/* 
 * File:   RCaptWindow.cpp
 * Author: jenicek
 * 
 * Created on July 28, 2010, 7:14 AM
 */

#include "RCaptWindow.h"

RCaptWindow::RCaptWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    this->Connect(-1, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&RCaptWindow::OnCloseWindow);
    this->Connect(-1, wxEVT_PAINT, (wxObjectEventFunction)&RCaptWindow::OnPaintWindow);
}


RCaptWindow::~RCaptWindow()
{
}

bool RCaptWindow::Initialize(void)
{
    // Create the camera
    this->cvCamera = new RCvCamera(0, this, CANVAS_CAM, wxPoint(140, 100), wxSize(320, 240), wxSize(640, 480));

    // Create run camera button
    this->btnRunCam = new wxButton(this, BTN_RUN_CAM, _("Run camera"), wxPoint(10, 10), wxSize(100, 30));
    // Create capture camera button
    this->btnCaptCam = new wxButton(this, BTN_CAPT_CAM, _("Start capture"), wxPoint(this->btnRunCam->m_x, this->btnRunCam->m_y + 50), wxSize(100, 30));
    this->btnCaptCam->Disable();

    // Create settings text boxes
    this->lblFrameDelay = new wxStaticText(this, wxID_ANY, _("Frame delay:"), wxPoint(10, this->btnCaptCam->m_y + 50));
    this->txtFrameDelay = new wxTextCtrl(this, TXT_FRAME_DELAY, _(""), wxPoint(20, this->lblFrameDelay->m_y + 25), wxSize(50, 25), wxTE_PROCESS_ENTER);
    this->txtFrameDelay->SetMaxLength(4);
    this->txtFrameDelay->SetValue(wxString::Format(_("%d"), this->cvCamera->GetFrameDelay()));
    this->lblMilisec = new wxStaticText(this, wxID_ANY, _("ms"), wxPoint(this->txtFrameDelay->m_x + 55, this->txtFrameDelay->m_y + 5));
    this->lblCameraID = new wxStaticText(this, wxID_ANY, _("Camera ID"), wxPoint(10, this->txtFrameDelay->m_y + 45));
    this->lblDevVideo = new wxStaticText(this, wxID_ANY, _("/dev/video"), wxPoint(20, this->lblCameraID->m_y + 25));
    this->txtCameraID = new wxTextCtrl(this, TXT_CAM_ID, _(""), wxPoint(this->lblDevVideo->m_x + 70, this->lblDevVideo->m_y - 5), wxSize(30, 25), wxTE_PROCESS_ENTER);
    this->txtCameraID->SetMaxLength(1);
    this->txtCameraID->SetValue(wxString::Format(_("%d"), this->cvCamera->GetCamID()));
    // Create static bitmaps for displaying captured phase images
    this->DrawPhaseFrames();

    // Create resolution settings combo boxes
    const wxString cbxResVals[14] = {_("1280x800"), _("1024x768"), _("840x525"), _("800x600"), _("800x512"), _("720x450"), _("700x525"),
        _("680x384"), _("640x512"), _("640x480"), _("576x432"), _("512x384"), _("400x300"), _("320x240")};
    this->lblProjRes = new wxStaticText(this, wxID_ANY, _("Proj. resolution:"), wxPoint(10, this->txtCameraID->m_y + 50));
    this->cbxProjRes = new wxComboBox(this, CBX_PROJ_RES, cbxResVals[1], wxPoint(15, this->lblProjRes->m_y + 25),
            wxSize(110, 25), 14, cbxResVals);
    this->lblCamRes = new wxStaticText(this, wxID_ANY, _("Cam. resolution:"), wxPoint(10, this->cbxProjRes->m_y + 50));
    this->cbxCamRes = new wxComboBox(this, CBX_CAM_RES, cbxResVals[9], wxPoint(15, this->lblCamRes->m_y + 25),
            wxSize(110, 25), 14, cbxResVals);
    

    // Create shifting method selection combo box
    const wxString cbxShiftMetVals[2] = {_("3 phase imgs."), _("4 phase imgs.")};
    this->lblShiftMet = new wxStaticText(this, wxID_ANY, _("Number of imgs:"), wxPoint(10, this->cbxCamRes->m_y + 50));
    this->cbxShiftMet = new wxComboBox(this, CBX_SHIFT_MET, cbxShiftMetVals[0], wxPoint(15, this->lblShiftMet->m_y + 25),
            wxSize(110, 25), 2, cbxShiftMetVals);
    this->numPhImgs = 3;

    // Create capture reference plane images checkbox
    this->chxRefImgs = new wxCheckBox(this, CHX_REF_IMGS, _("Ref. images"), wxPoint(10, this->cbxShiftMet->m_y + 50));

    // Create labels and textboxes for setting save and pattern paths
    this->lblSavePath = new wxStaticText(this, wxID_ANY, _("Capture storage: "), wxPoint(150, 20));
    this->txtSavePath = new wxTextCtrl(this, TXT_SAVE_PATH, _(""), wxPoint(this->lblSavePath->m_x + 120, this->lblSavePath->m_y - 5), wxSize(430, 25), wxTE_PROCESS_ENTER);
    this->txtSavePath->SetValue(this->cvCamera->GetFileSavePath());
    this->btnSavePath = new wxButton(this, BTN_SAVE_PATH, _("Choose..."), wxPoint(this->txtSavePath->m_x + 435, this->lblSavePath->m_y - 8));
    this->lblPatPath = new wxStaticText(this, wxID_ANY, _("Pattern storage: "), wxPoint(150, this->lblSavePath->m_y + 40));
    this->txtPatPath = new wxTextCtrl(this, TXT_PAT_PATH, _(""), wxPoint(this->lblPatPath->m_x + 120, this->lblPatPath->m_y - 5), wxSize(430, 25), wxTE_PROCESS_ENTER);
    this->btnPatPath = new wxButton(this, BTN_PAT_PATH, _("Choose..."), wxPoint(this->txtPatPath->m_x + 435, this->lblPatPath->m_y - 8));
    this->txtPatPath->SetValue(this->cvCamera->GetProjPatPath());


    // Register events
    this->cvCamera->Connect(CANVAS_CAM, wxEVT_PAINT, (wxObjectEventFunction)&RCvCamera::OnCamWndPaint);
    this->Connect(BTN_RUN_CAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCaptWindow::OnBtnRunCamClicked);
    this->Connect(TXT_SAVE_PATH, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCaptWindow::OnTxtSavePathEnter);
    this->Connect(TXT_PAT_PATH, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCaptWindow::OnTxtPatPathEnter);
    this->Connect(BTN_CAPT_CAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCaptWindow::OnBtnCaptCamClicked);
    this->Connect(TXT_CAM_ID, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCaptWindow::OnTxtCamIDEnter);
    this->Connect(TXT_FRAME_DELAY, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCaptWindow::OnTxtFrameDelayEnter);
    this->Connect(BTN_SAVE_PATH, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCaptWindow::OnBtnSavePathClicked);
    this->Connect(BTN_PAT_PATH, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCaptWindow::OnBtnPatPathClicked);
    this->Connect(CBX_SHIFT_MET, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RCaptWindow::OnCbxShiftMetSelected);
    this->Connect(CHX_REF_IMGS, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&RCaptWindow::OnChxRefImgsClicked);
    this->Connect(CBX_CAM_RES, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RCaptWindow::OnCbxCamResSelected);


    return true;
}

void RCaptWindow::OnCloseWindow(wxCloseEvent& event)
{
    this->cvCamera->StopCamera();
    this->Destroy();

}

void RCaptWindow::OnBtnRunCamClicked(wxCommandEvent& event)
{
    if(this->cvCamera->GetRunCamera())
    {
        this->btnRunCam->SetLabel("Run camera");
        this->cvCamera->StopCamera();

        // Disable capture
        this->btnCaptCam->Disable();
        // Enable settings
        this->txtCameraID->Enable(true);
        this->txtFrameDelay->Enable(true);
        this->cbxCamRes->Enable(true);
        this->cbxProjRes->Enable(true);
    }
    else
    {
        if(!this->cvCamera->RunCamera())
            return;
        this->btnRunCam->SetLabel("Stop camera");

        // Enable capture
        this->btnCaptCam->Enable(true);
        // Disable settings
        this->txtCameraID->Disable();
        this->txtFrameDelay->Disable();
        this->cbxCamRes->Disable();
        this->cbxProjRes->Disable();
    }
}

void RCaptWindow::OnBtnCaptCamClicked(wxCommandEvent& event)
{
    if(this->cvCamera->GetRunCamera())
    {
        this->btnCaptCam->Disable();

        // Get current projector resolution
        int projWidth, projHeight;
        if(sscanf(this->cbxProjRes->GetValue().ToAscii(), "%dx%d", &projWidth, &projHeight) != 2)
            return;

        this->cvCamera->CapturePhases(atoi(this->txtCameraID->GetValue()), this->numPhImgs, atoi(this->txtFrameDelay->GetValue()),
                projWidth, projHeight, this->chxRefImgs->GetValue());
        this->DrawPhaseFrames();
        this->btnCaptCam->Enable(true);
    }
}

void RCaptWindow::OnPaintWindow(wxPaintEvent& event)
{
    this->DrawPhaseFrames();
}

void RCaptWindow::OnTxtSavePathEnter(wxCommandEvent& event)
{
    if(wxDir::Exists(this->txtSavePath->GetValue()))
    {
        this->cvCamera->SetFileSavePath(this->txtSavePath->GetValue());
        if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
            this->btnCaptCam->Enable(true);
        this->txtSavePath->SetBackgroundColour(wxColour(255, 255, 255));
    }
    else
    {
        this->btnCaptCam->Disable();
        this->txtSavePath->SetBackgroundColour(wxColour(255, 0, 0));
    }
}

void RCaptWindow::OnTxtPatPathEnter(wxCommandEvent& event)
{
    if(wxDir::Exists(this->txtPatPath->GetValue()))
    {
        this->cvCamera->SetProjPatPath(this->txtPatPath->GetValue());
        if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
            this->btnCaptCam->Enable(true);
        this->txtPatPath->SetBackgroundColour(wxColour(255, 255, 255));
    }
    else
    {
        this->btnCaptCam->Disable();
        this->txtPatPath->SetBackgroundColour(wxColour(255, 0, 0));
    }
}

void RCaptWindow::OnBtnSavePathClicked(wxCommandEvent& event)
{
    // Show choose directory dialog
    wxDirDialog *dlg = new wxDirDialog(this, _("Choose directory"), this->cvCamera->GetFileSavePath(), 0, wxDefaultPosition);
    if(dlg->ShowModal() == wxID_OK)
    {
        this->cvCamera->SetFileSavePath(dlg->GetPath());
        this->txtSavePath->SetBackgroundColour(wxColour(255, 255, 255));
        this->txtSavePath->SetValue(this->cvCamera->GetFileSavePath());
        if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
            this->btnCaptCam->Enable(true);
    }
}

void RCaptWindow::OnBtnPatPathClicked(wxCommandEvent& event)
{
    // Show choose directory dialog
    wxDirDialog *dlg = new wxDirDialog(this, _("Choose directory"), this->cvCamera->GetProjPatPath(), 0, wxDefaultPosition);
    if(dlg->ShowModal() == wxID_OK)
    {
        this->cvCamera->SetProjPatPath(dlg->GetPath());
        this->txtPatPath->SetBackgroundColour(wxColour(255, 255, 255));
        this->txtPatPath->SetValue(this->cvCamera->GetProjPatPath());
        if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
            this->btnCaptCam->Enable(true);
    }
}

void RCaptWindow::OnTxtFrameDelayEnter(wxCommandEvent& event)
{
    if(!this->cvCamera->GetRunCamera())
        this->cvCamera->SetFrameDelay(atoi(this->txtFrameDelay->GetValue()));
}

void RCaptWindow::OnTxtCamIDEnter(wxCommandEvent& event)
{
    if(!this->cvCamera->GetRunCamera())
        this->cvCamera->SetCamID(atoi(this->txtCameraID->GetValue()));
}

void RCaptWindow::OnCbxShiftMetSelected(wxCommandEvent& event)
{
    // Set numPhImgs variable
    // Also set new projector patterns storage
    switch(this->cbxShiftMet->GetCurrentSelection())
    {
        case 0:
            this->txtPatPath->SetValue(_("/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/pattern/horizontal"));
            this->numPhImgs = 3;
            break;
        case 1:
            this->txtPatPath->SetValue(_("/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/pattern/horizontal4"));
            this->numPhImgs = 4;
            break;
    }

    this->cvCamera->SetProjPatPath(this->txtPatPath->GetValue());
}

void RCaptWindow::OnChxRefImgsClicked(wxCommandEvent& event)
{
    
}

void RCaptWindow::OnCbxCamResSelected(wxCommandEvent& event)
{
    // Get current camera resolution
    int camWidth, camHeight;
    if(sscanf(this->cbxCamRes->GetValue().ToAscii(), "%dx%d", &camWidth, &camHeight) != 2)
        return;

    this->cvCamera->SetCamResWidth(camWidth);
    this->cvCamera->SetCamResHeight(camHeight);
}

void RCaptWindow::DrawPhaseFrames(void)
{
    // Prepare image, used for resizing
    wxImage img = wxImage();
    wxInitAllImageHandlers();
    // Prepare bitmap
    wxBitmap bmp;
    // Get drawing context
    wxPaintDC dc(this);
    // Set font and brush
    dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
    dc.SetTextForeground(*wxWHITE);
    dc.SetBrush(wxBrush(wxColour(50, 50, 50), wxSOLID));
    // Create bitmap with proper size and draw
    char fName[128] = "\0";
    strcat(fName, this->cvCamera->GetFileSavePath());
    strcat(fName, "phase1.jpg\0");
    if(wxFile::Exists(fName))
    {
        img.LoadFile(fName);
        bmp = wxBitmap(img.Scale(320, 240));
        dc.DrawBitmap(bmp, wxPoint(this->cvCamera->m_x + 330, 100));
    }
    else
        dc.DrawRectangle(wxPoint(this->cvCamera->m_x + 330, 100), wxSize(320, 240));
    // Get next phase image
    fName[0] = '\0';
    strcat(fName, this->cvCamera->GetFileSavePath());
    strcat(fName, "phase1.jpg\0");
    if(wxFile::Exists(fName))
    {
        img.LoadFile(fName);
        bmp = wxBitmap(img.Scale(320, 240));
        dc.DrawBitmap(bmp, wxPoint(140, this->cvCamera->m_y + 250));
    }
    else
        dc.DrawRectangle(wxPoint(140, this->cvCamera->m_y + 250), wxSize(320, 240));
    // Get next phase image
    fName[0] = '\0';
    strcat(fName, this->cvCamera->GetFileSavePath());
    strcat(fName, "phase1.jpg\0");
    if(wxFile::Exists(fName))
    {
        img.LoadFile(fName);
        bmp = wxBitmap(img.Scale(320, 240));
        dc.DrawBitmap(bmp, wxPoint(this->cvCamera->m_x + 330, this->cvCamera->m_y + 250));
    }
    else
        dc.DrawRectangle(wxPoint(this->cvCamera->m_x + 330, this->cvCamera->m_y + 250), wxSize(320, 240));
    
    // Draw bitmap labels
    dc.DrawText(_("Phase 1"), wxPoint(this->cvCamera->m_x + 335, 105));
    dc.DrawText(_("Phase 2"), wxPoint(145, this->cvCamera->m_y + 255));
    dc.DrawText(_("Phase 3"), wxPoint(this->cvCamera->m_x + 335, this->cvCamera->m_y + 255));

    // Release drawing context
    dc.Destroy();
}




