/* 
 * File:   RCalibWindow.cpp
 * Author: jenicek
 * 
 * Created on August 19, 2010, 8:14 AM
 */

#include "RCalibWindow.h"

RCalibWindow::RCalibWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    this->Connect(-1, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&RCalibWindow::OnCloseWindow);
    this->Connect(-1, wxEVT_PAINT, (wxObjectEventFunction)&RCalibWindow::OnPaintWindow);
}

RCalibWindow::~RCalibWindow()
{
    // Release memory allocated for calibration output parameters
    if(this->calOutput.camIntrinsic)
        cvReleaseMat(&this->calOutput.camIntrinsic);
    if(this->calOutput.camExtrinsic)
        cvReleaseMat(&this->calOutput.camExtrinsic);
    if(this->calOutput.camDistortion)
        cvReleaseMat(&this->calOutput.camDistortion);
    if(this->calOutput.camCenter)
        cvReleaseMat(&this->calOutput.camCenter);
    if(this->calOutput.projIntrinsic)
        cvReleaseMat(&this->calOutput.projIntrinsic);
    if(this->calOutput.projExtrinsic)
        cvReleaseMat(&this->calOutput.projExtrinsic);
    if(this->calOutput.projDistortion)
        cvReleaseMat(&this->calOutput.projDistortion);
    if(this->calOutput.projCenter)
        cvReleaseMat(&this->calOutput.projCenter);
}

bool RCalibWindow::Initialize(void)
{
    /* Calibration input parameters */
    // Common params
    this->calInput.frameDelay = 4000;
    this->calInput.numberBoards = 10;

    // Camera params
    this->calInput.camResWidth = 640;
    this->calInput.camResHeight = 480;
    this->calInput.camGain = 100.0f;
    this->calInput.camBoardWidth = 8;
    this->calInput.camBoardHeight = 6;
    this->calInput.camBoardSqMmW = 29;
    this->calInput.camBoardSqMmH = 29;

    // Projector params
    this->calInput.projResWidth = 1024;
    this->calInput.projResHeight = 768;
    this->calInput.projGain = 100.0f;
    this->calInput.projBoardWidth = 8;
    this->calInput.projBoardHeight = 6;
    this->calInput.projBoardSqPxW = 50;
    this->calInput.projBoardSqPxH = 50;
    /* * * * * * * *  * * * * * * * */

    /* Calibration output parameters */
    // Camera params
    this->calOutput.camIntrinsic = cvCreateMat(3, 3, CV_32FC1);
    this->calOutput.camDistortion = cvCreateMat(5, 1, CV_32FC1);
    this->calOutput.camExtrinsic = cvCreateMat(2, 3, CV_32FC1);
    this->calOutput.camCenter = cvCreateMat(3, 1, CV_32FC1);

    // Projector params
    this->calOutput.projIntrinsic = cvCreateMat(3, 3, CV_32FC1);
    this->calOutput.projDistortion = cvCreateMat(5, 1, CV_32FC1);
    this->calOutput.projExtrinsic = cvCreateMat(2, 3, CV_32FC1);
    this->calOutput.projCenter = cvCreateMat(3, 1, CV_32FC1);
    /* * * * * * * *  * * * * * * * */

    this->cvCamera = new RCvCamera(0, this, CANVAS_CAM, wxPoint(270, 200), wxSize(520, 390), wxSize(640, 480));

    this->lblProjImgStorage = new wxStaticText(this, wxID_ANY, _("Proj. storage: "), wxPoint(this->cvCamera->m_x + 10, this->cvCamera->m_y - 35));
    this->txtProjImgStorage = new wxTextCtrl(this, TXT_CAM_STORAGE, _(""), wxPoint(this->lblProjImgStorage->m_x + 100, this->lblProjImgStorage->m_y - 5),
            wxSize(300, 25), wxTE_PROCESS_ENTER);
    this->btnProjImgStorage = new wxButton(this, BTN_CAM_STORAGE, _("Choose..."), wxPoint(this->txtProjImgStorage->m_x + 310,
            this->txtProjImgStorage->m_y - 3));
    this->lblCamImgStorage = new wxStaticText(this, wxID_ANY, _("Cam. storage: "), wxPoint(this->cvCamera->m_x + 10, this->lblProjImgStorage->m_y - 35));
    this->txtCamImgStorage = new wxTextCtrl(this, TXT_CAM_STORAGE, _(""), wxPoint(this->lblCamImgStorage->m_x + 100, this->lblCamImgStorage->m_y - 5),
            wxSize(300, 25), wxTE_PROCESS_ENTER);
    this->btnCamImgStorage = new wxButton(this, BTN_CAM_STORAGE, _("Choose..."), wxPoint(this->txtCamImgStorage->m_x + 310,
            this->txtCamImgStorage->m_y - 3));

    this->lblProjGain = new wxStaticText(this, wxID_ANY, _("Proj. gain: "), wxPoint(this->lblCamImgStorage->m_x, this->lblCamImgStorage->m_y - 35));
    this->sldrProjGain = new wxSlider(this, SLDR_PROJ_GAIN, (int)this->calInput.projGain, 0, 100, wxPoint(this->lblProjGain->m_x + 100, this->lblProjGain->m_y - 5), wxSize(300, 25));
    this->txtProjGain = new wxTextCtrl(this, TXT_PROJ_GAIN, wxString::Format("%.2f", this->calInput.projGain), wxPoint(this->sldrProjGain->m_x + 310, this->sldrProjGain->m_y), wxSize(75, 25), wxTE_PROCESS_ENTER);
    this->lblCamGain = new wxStaticText(this, wxID_ANY, _("Cam. gain: "), wxPoint(this->lblProjGain->m_x, this->lblProjGain->m_y - 35));
    this->sldrCamGain = new wxSlider(this, SLDR_CAM_GAIN, (int)this->calInput.camGain, 0, 100, wxPoint(this->lblCamGain->m_x + 100, this->lblCamGain->m_y - 5), wxSize(300, 25));
    this->txtCamGain = new wxTextCtrl(this, TXT_CAM_GAIN, wxString::Format("%.2f", this->calInput.camGain), wxPoint(this->sldrCamGain->m_x + 310, this->sldrCamGain->m_y), wxSize(75, 25), wxTE_PROCESS_ENTER);

    this->btnRunCam = new wxButton(this, BTN_RUN_CAM, _("Run camera"), wxPoint(this->lblCamGain->m_x, this->lblCamGain->m_y - 45));
    this->btnCompGeometry = new wxButton(this, BTN_COMP_GEOMETRY, _("Comp. geometry"), wxPoint(this->btnRunCam->m_x + 100, this->btnRunCam->m_y));
    this->btnCompGeometry->Disable();
    this->btnCalibCam = new wxButton(this, BTN_CALIB_CAM, _("Calib. camera"), wxPoint(this->btnCompGeometry->m_x + 152, this->btnCompGeometry->m_y));
    this->btnCalibCam->Disable();
    this->btnCalibProjCam = new wxButton(this, BTN_CALIB_PROJCAM, _("Calib. proj.-cam."), wxPoint(this->btnCalibCam->m_x + 113, this->btnCalibCam->m_y));
    this->btnCalibProjCam->Disable();
    
    this->lblNumFrames = new wxStaticText(this, wxID_ANY, _("Capture frames: "), wxPoint(40, 20));
    this->spinNumFrames = new wxSpinCtrl(this, SPIN_NUM_FRAMES, wxString::Format("%d", this->calInput.numberBoards),
            wxPoint(this->lblNumFrames->m_x + 120, this->lblNumFrames->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 2, 40);

    // Camera settings panel
    const wxString cbxResVals[14] = {_("1280x800"), _("1024x768"), _("840x525"), _("800x600"), _("800x512"), _("720x450"), _("700x525"),
        _("680x384"), _("640x512"), _("640x480"), _("576x432"), _("512x384"), _("400x300"), _("320x240")};
    this->panelCamSet = new wxPanel(this, wxID_ANY, wxPoint(10, 50), wxSize(250, 265));
    this->panelCamSet->SetBackgroundColour(wxColour(200, 200, 200));
    this->lblCamSet = new wxStaticText(this->panelCamSet, wxID_ANY, _("Camera settings"), wxPoint(10, 10));
    this->lblCamRes = new wxStaticText(this->panelCamSet, wxID_ANY, _("Resolution: "), wxPoint(this->lblCamSet->m_x + 5, this->lblCamSet->m_y + 35));
    this->cbxCamRes = new wxComboBox(this->panelCamSet, CBX_CAM_RES, cbxResVals[9], wxPoint(this->lblCamRes->m_x + 85, this->lblCamRes->m_y - 5),
            wxSize(110, 25), 14, cbxResVals);
    this->lblCamBoard = new wxStaticText(this->panelCamSet, wxID_ANY, _("Chessboard (squares):"), wxPoint(this->lblCamRes->m_x, this->lblCamRes->m_y + 30));
    this->lblCamBoardWidth = new wxStaticText(this->panelCamSet, wxID_ANY, _("width: "), wxPoint(this->lblCamBoard->m_x + 5, this->lblCamBoard->m_y + 25));
    this->spinCamBoardWidth = new wxSpinCtrl(this->panelCamSet, SPIN_CAM_B_W, wxString::Format("%.2d", this->calInput.camBoardWidth),
            wxPoint(this->lblCamBoardWidth->m_x + 50, this->lblCamBoardWidth->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 2, 20);
    this->lblCamBoardHeight = new wxStaticText(this->panelCamSet, wxID_ANY, _("height: "), wxPoint(this->spinCamBoardWidth->m_x + 65, this->lblCamBoardWidth->m_y));
    this->spinCamBoardHeight = new wxSpinCtrl(this->panelCamSet, SPIN_CAM_B_H, wxString::Format("%.2d", this->calInput.camBoardHeight),
            wxPoint(this->lblCamBoardHeight->m_x + 50, this->lblCamBoardHeight->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 2, 20);
    this->lblCamBoardSq = new wxStaticText(this->panelCamSet, wxID_ANY, _("Chessboard squares (milimeters):"), wxPoint(this->lblCamRes->m_x, this->lblCamBoardWidth->m_y + 30));
    this->lblCamBoardSqMmW = new wxStaticText(this->panelCamSet, wxID_ANY, _("width: "), wxPoint(this->lblCamBoardSq->m_x + 5, this->lblCamBoardSq->m_y + 25));
    this->spinCamBoardSqMmW = new wxSpinCtrl(this->panelCamSet, SPIN_CAM_SQMM_W, wxString::Format("%.2d", this->calInput.camBoardSqMmW),
            wxPoint(this->lblCamBoardSqMmW->m_x + 50, this->lblCamBoardSqMmW->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 10, 100);
    this->lblCamBoardSqMmH = new wxStaticText(this->panelCamSet, wxID_ANY, _("height: "), wxPoint(this->spinCamBoardSqMmW->m_x + 65, this->lblCamBoardSqMmW->m_y));
    this->spinCamBoardSqMmH = new wxSpinCtrl(this->panelCamSet, SPIN_CAM_SQMM_H, wxString::Format("%.2d", this->calInput.camBoardSqMmH),
            wxPoint(this->lblCamBoardSqMmH->m_x + 50, this->lblCamBoardSqMmH->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 10, 100);


    // Projector settings panel
    this->panelProjSet = new wxPanel(this, wxID_ANY, wxPoint(this->panelCamSet->m_x, this->panelCamSet->m_y + 275), wxSize(250, 265));
    this->panelProjSet->SetBackgroundColour(wxColour(200, 200, 200));
    this->lblProjSet = new wxStaticText(this->panelProjSet, wxID_ANY, _("Projector settings"), wxPoint(10, 10));
    this->lblProjRes = new wxStaticText(this->panelProjSet, wxID_ANY, _("Resolution: "), wxPoint(this->lblProjSet->m_x + 5, this->lblProjSet->m_y + 35));
    this->cbxProjRes = new wxComboBox(this->panelProjSet, CBX_PROJ_RES, cbxResVals[1], wxPoint(this->lblProjRes->m_x + 85, this->lblProjRes->m_y - 5),
            wxSize(110, 25), 14, cbxResVals);
    this->lblProjBoard = new wxStaticText(this->panelProjSet, wxID_ANY, _("Chessboard (squares):"), wxPoint(this->lblProjRes->m_x, this->lblProjRes->m_y + 30));
    this->lblProjBoardWidth = new wxStaticText(this->panelProjSet, wxID_ANY, _("width: "), wxPoint(this->lblProjBoard->m_x + 5, this->lblProjBoard->m_y + 25));
    this->spinProjBoardWidth = new wxSpinCtrl(this->panelProjSet, SPIN_PROJ_B_W, wxString::Format("%.2d", this->calInput.projBoardWidth),
            wxPoint(this->lblProjBoardWidth->m_x + 50, this->lblProjBoardWidth->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 2, 20);
    this->lblProjBoardHeight = new wxStaticText(this->panelProjSet, wxID_ANY, _("height: "), wxPoint(this->spinProjBoardWidth->m_x + 65, this->lblProjBoardWidth->m_y));
    this->spinProjBoardHeight = new wxSpinCtrl(this->panelProjSet, SPIN_PROJ_B_H, wxString::Format("%.2d", this->calInput.projBoardHeight),
            wxPoint(this->lblProjBoardHeight->m_x + 50, this->lblProjBoardHeight->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 2, 20);
    this->lblProjBoardSq = new wxStaticText(this->panelProjSet, wxID_ANY, _("Chessboard squares (pixels):"), wxPoint(this->lblProjRes->m_x, this->lblProjBoardWidth->m_y + 30));
    this->lblProjBoardSqPxW = new wxStaticText(this->panelProjSet, wxID_ANY, _("width: "), wxPoint(this->lblProjBoardSq->m_x + 5, this->lblProjBoardSq->m_y + 25));
    this->spinProjBoardSqPxW = new wxSpinCtrl(this->panelProjSet, SPIN_PROJ_SQPX_W, wxString::Format("%.2d", this->calInput.projBoardSqPxW),
            wxPoint(this->lblProjBoardSqPxW->m_x + 50, this->lblProjBoardSqPxW->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 5, 300);
    this->lblProjBoardSqPxH = new wxStaticText(this->panelProjSet, wxID_ANY, _("height: "), wxPoint(this->spinProjBoardSqPxW->m_x + 65, this->lblProjBoardSqPxW->m_y));
    this->spinProjBoardSqPxH = new wxSpinCtrl(this->panelProjSet, SPIN_PROJ_SQPX_H, wxString::Format("%.2d", this->calInput.projBoardSqPxH),
            wxPoint(this->lblProjBoardSqPxH->m_x + 50, this->lblProjBoardSqPxH->m_y - 5), wxSize(50, 25), wxSP_ARROW_KEYS, 5, 300);

    // Register events
    this->cvCamera->Connect(CANVAS_CAM, wxEVT_PAINT, (wxObjectEventFunction)&RCvCamera::OnCamWndPaint);
    this->Connect(TXT_CAM_STORAGE, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCalibWindow::OnTxtCamImgStorageEnter);
    this->Connect(TXT_PROJ_STORAGE, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCalibWindow::OnTxtProjImgStorageEnter);
    this->Connect(BTN_CAM_STORAGE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnCamImgStorageClicked);
    this->Connect(BTN_PROJ_STORAGE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnProjImgStorageClicked);
    this->Connect(SLDR_CAM_GAIN, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSldrCamGainScroll);
    this->Connect(SLDR_PROJ_GAIN, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSldrProjGainScroll);
    this->Connect(TXT_CAM_GAIN, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCalibWindow::OnTxtCamGainEnter);
    this->Connect(TXT_PROJ_GAIN, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RCalibWindow::OnTxtProjGainEnter);
    this->Connect(BTN_RUN_CAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnRunCamClicked);
    this->Connect(BTN_COMP_GEOMETRY, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnCompGeometryClicked);
    this->Connect(BTN_CALIB_CAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnCalibCamClicked);
    this->Connect(BTN_CALIB_PROJCAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RCalibWindow::OnBtnCalibProjCamClicked);
    this->Connect(CBX_CAM_RES, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RCalibWindow::OnCbxCamResSelected);
    this->Connect(CBX_PROJ_RES, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RCalibWindow::OnCbxProjResSelected);
    this->Connect(SPIN_CAM_B_W, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinCamBoardWidthUpdated);
    this->Connect(SPIN_CAM_B_H, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinCamBoardHeightUpdated);
    this->Connect(SPIN_CAM_SQMM_W, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinCamBoardSqMmWUpdated);
    this->Connect(SPIN_CAM_SQMM_H, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinCamBoardSqMmHUpdated);
    this->Connect(SPIN_PROJ_B_W, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinProjBoardWidthUpdated);
    this->Connect(SPIN_PROJ_B_H, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinProjBoardHeightUpdated);
    this->Connect(SPIN_PROJ_SQPX_W, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinProjBoardSqPxWUpdated);
    this->Connect(SPIN_PROJ_SQPX_H, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinProjBoardSqPxHUpdated);
    this->Connect(SPIN_NUM_FRAMES, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&RCalibWindow::OnSpinNumFramesUpdated);
}

void RCalibWindow::ResetCalibOutput(void)
{
    cvSet(this->calOutput.camIntrinsic, cvScalarAll(0), NULL);
    cvSet(this->calOutput.camExtrinsic, cvScalarAll(0), NULL);
    cvSet(this->calOutput.camDistortion, cvScalarAll(0), NULL);
    cvSet(this->calOutput.camCenter, cvScalarAll(0), NULL);
    cvSet(this->calOutput.projIntrinsic, cvScalarAll(0), NULL);
    cvSet(this->calOutput.projExtrinsic, cvScalarAll(0), NULL);
    cvSet(this->calOutput.projDistortion, cvScalarAll(0), NULL);
    cvSet(this->calOutput.projCenter, cvScalarAll(0), NULL);
}

void RCalibWindow::OnCloseWindow(wxCommandEvent& event)
{
    this->cvCamera->StopCamera();
    this->Destroy();
    
    // Release memory allocated for calibration output parameters
    if(this->calOutput.camIntrinsic)
        cvReleaseMat(&this->calOutput.camIntrinsic);
    if(this->calOutput.camExtrinsic)
        cvReleaseMat(&this->calOutput.camExtrinsic);
    if(this->calOutput.camDistortion)
        cvReleaseMat(&this->calOutput.camDistortion);
    if(this->calOutput.camCenter)
        cvReleaseMat(&this->calOutput.camCenter);
    if(this->calOutput.projIntrinsic)
        cvReleaseMat(&this->calOutput.projIntrinsic);
    if(this->calOutput.projExtrinsic)
        cvReleaseMat(&this->calOutput.projExtrinsic);
    if(this->calOutput.projDistortion)
        cvReleaseMat(&this->calOutput.projDistortion);
    if(this->calOutput.projCenter)
        cvReleaseMat(&this->calOutput.projCenter);
}

void RCalibWindow::OnPaintWindow(wxPaintEvent& event)
{
    
}

void RCalibWindow::OnBtnCamImgStorageClicked(wxCommandEvent& event)
{
    // Show choose directory dialog
    wxDirDialog *dlg = new wxDirDialog(this, _("Choose directory"), this->txtCamImgStorage->GetValue(), 0, wxDefaultPosition);
    if(dlg->ShowModal() == wxID_OK)
    {
        //this->cvCamera->SetFileSavePath(dlg->GetPath());
        this->txtCamImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
        this->txtCamImgStorage->SetValue(dlg->GetPath());
        //if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
        //    this->btnCaptCam->Enable(true);
    }
}

void RCalibWindow::OnBtnProjImgStorageClicked(wxCommandEvent& event)
{
    // Show choose directory dialog
    wxDirDialog *dlg = new wxDirDialog(this, _("Choose directory"), this->txtProjImgStorage->GetValue(), 0, wxDefaultPosition);
    if(dlg->ShowModal() == wxID_OK)
    {
        //this->cvCamera->SetFileSavePath(dlg->GetPath());
        this->txtProjImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
        this->txtProjImgStorage->SetValue(dlg->GetPath());
        //if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
        //    this->btnCaptCam->Enable(true);
    }
}

void RCalibWindow::OnTxtCamImgStorageEnter(wxCommandEvent& event)
{
    if(wxDir::Exists(this->txtCamImgStorage->GetValue()))
    {
        this->cvCamera->SetFileSavePath(this->txtCamImgStorage->GetValue());
        //if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
        //    this->btnCaptCam->Enable(true);
        this->txtCamImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
    }
    else
    {
        //this->btnCaptCam->Disable();
        this->txtCamImgStorage->SetBackgroundColour(wxColour(255, 0, 0));
    }
}

void RCalibWindow::OnTxtProjImgStorageEnter(wxCommandEvent& event)
{
    if(wxDir::Exists(this->txtProjImgStorage->GetValue()))
    {
        //this->cvCamera->SetFileSavePath(this->txtCamImgStorage->GetValue());
        //if(this->cvCamera->GetRunCamera() && !this->btnCaptCam->IsEnabled())
        //    this->btnCaptCam->Enable(true);
        this->txtProjImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
    }
    else
    {
        //this->btnCaptCam->Disable();
        this->txtProjImgStorage->SetBackgroundColour(wxColour(255, 0, 0));
    }
}

void RCalibWindow::OnSldrCamGainScroll(wxCommandEvent& event)
{
    this->calInput.camGain = this->sldrCamGain->GetValue();
    this->txtCamGain->SetValue(wxString::Format("%.2f", this->calInput.camGain));
}

void RCalibWindow::OnSldrProjGainScroll(wxCommandEvent& event)
{
    this->calInput.projGain = this->sldrProjGain->GetValue();
    this->txtProjGain->SetValue(wxString::Format("%.2f", this->calInput.projGain));
}

void RCalibWindow::OnTxtCamGainEnter(wxCommandEvent& event)
{
    this->calInput.camGain = atof(this->txtCamGain->GetValue());

    if(this->calInput.camGain > 100.0)
    {
        this->calInput.camGain = 100.0;
        this->txtCamGain->SetValue(wxString::Format("%.2f", this->calInput.camGain));
    }
    else if(this->calInput.camGain < 0.0)
    {
        this->calInput.camGain = 0.0;
        this->txtCamGain->SetValue(wxString::Format("%.2f", this->calInput.camGain));
    }

    this->sldrCamGain->SetValue((int)this->calInput.camGain);
}

void RCalibWindow::OnTxtProjGainEnter(wxCommandEvent& event)
{
    this->calInput.projGain = atof(this->txtProjGain->GetValue());

    if(this->calInput.projGain > 100.0)
    {
        this->calInput.projGain = 100.0;
        this->txtProjGain->SetValue(wxString::Format("%.2f", this->calInput.projGain));
    }
    else if(this->calInput.projGain < 0.0)
    {
        this->calInput.projGain = 0.0;
        this->txtProjGain->SetValue(wxString::Format("%.2f", this->calInput.projGain));
    }

    this->sldrProjGain->SetValue((int)this->calInput.projGain);
}

void RCalibWindow::OnBtnRunCamClicked(wxCommandEvent& event)
{
if(this->cvCamera->GetRunCamera())
    {
        this->btnRunCam->SetLabel("Run camera");
        this->cvCamera->StopCamera();

        // Disable calibration
        this->btnCalibCam->Disable();
        this->btnCalibProjCam->Disable();
        
        // Enable settings
        this->panelCamSet->Enable(true);
        this->panelProjSet->Enable(true);
    }
    else
    {
        if(!this->cvCamera->RunCamera())
            return;
        this->btnRunCam->SetLabel("Stop camera");

        // Enable calibration
        this->btnCalibCam->Enable(true);
        this->btnCalibProjCam->Enable(true);
       
        // Disable settings
        this->panelCamSet->Disable();
        this->panelProjSet->Disable();
    }
}

void RCalibWindow::OnBtnCompGeometryClicked(wxCommandEvent& event)
{
    ComputePCGeometry(&this->calInput, &this->calOutput);
}

void RCalibWindow::OnBtnCalibCamClicked(wxCommandEvent& event)
{
    // Create fullscreen window (for controlling camera's display)
    RImageWindow *camWnd = new RImageWindow(NULL ,wxPoint(10, 10), wxSize(this->calInput.camResWidth, this->calInput.camResHeight),
            _("Camera window"), _(""));
    camWnd->Show();
    camWnd->Initialize();

    CalibrateCamera(this->cvCamera->GetCamCapture(), camWnd, &this->calInput, &this->calOutput);

    // Notify that calibration's done
    wxMessageBox("Camera calibration's done.", "Done", wxOK | wxICON_INFORMATION);

    camWnd->Destroy();
}

void RCalibWindow::OnBtnCalibProjCamClicked(wxCommandEvent& event)
{
    // Create window (for controlling camera's display)
    RImageWindow *camWnd = new RImageWindow(NULL ,wxPoint(10, 10), wxSize(this->calInput.camResWidth, this->calInput.camResHeight),
            _("Camera window"), _(""));
    camWnd->Show();
    camWnd->Initialize();

    // Create window (for controlling projector's display)
    RImageWindow *projWnd = new RImageWindow(NULL ,wxPoint(10, 10), wxSize(this->calInput.projResWidth, this->calInput.projResHeight),
            _("Projector window"), _(""));
    projWnd->Show();
    projWnd->Initialize();

    CalibrateCamProjSyst(this->cvCamera->GetCamCapture(), camWnd, projWnd, &this->calInput, &this->calOutput);

    // Notify that calibration's done
    wxMessageBox("Projector-camera calibration's done.", "Done", wxOK | wxICON_INFORMATION);

    camWnd->Destroy();
    projWnd->Destroy();

    // Now we have data to evaluate projector-camera system geometry, enable Compute geometry button
    this->btnCompGeometry->Enable(true);
}

void RCalibWindow::OnCbxCamResSelected(wxCommandEvent& event)
{
    // Get checkbox current value and parse it
    int eWidth, eHeight;
    if(sscanf(this->cbxCamRes->GetValue().ToAscii(), "%dx%d", &eWidth, &eHeight) != 2)
        return;
    this->calInput.camResWidth = eWidth;
    this->calInput.camResHeight = eHeight;
}

void RCalibWindow::OnCbxProjResSelected(wxCommandEvent& event)
{
    // Get checkbox current value and parse it
    int eWidth, eHeight;
    if(sscanf(this->cbxProjRes->GetValue().ToAscii(), "%dx%d", &eWidth, &eHeight) != 2)
        return;
    this->calInput.projResWidth = eWidth;
    this->calInput.projResHeight = eHeight;
}

void RCalibWindow::OnSpinCamBoardWidthUpdated(wxCommandEvent& event)
{
    this->calInput.camBoardWidth = this->spinCamBoardWidth->GetValue();
}

void RCalibWindow::OnSpinCamBoardHeightUpdated(wxCommandEvent& event)
{
    this->calInput.camBoardHeight = this->spinCamBoardHeight->GetValue();
}

void RCalibWindow::OnSpinCamBoardSqMmWUpdated(wxCommandEvent& event)
{
    this->calInput.camBoardSqMmW = this->spinCamBoardSqMmW->GetValue();
}

void RCalibWindow::OnSpinCamBoardSqMmHUpdated(wxCommandEvent& event)
{
    this->calInput.camBoardSqMmH = this->spinCamBoardSqMmH->GetValue();
}

void RCalibWindow::OnSpinProjBoardWidthUpdated(wxCommandEvent& event)
{
    this->calInput.projBoardWidth = this->spinProjBoardWidth->GetValue();
}

void RCalibWindow::OnSpinProjBoardHeightUpdated(wxCommandEvent& event)
{
    this->calInput.projBoardHeight = this->spinProjBoardHeight->GetValue();
}

void RCalibWindow::OnSpinProjBoardSqPxWUpdated(wxCommandEvent& event)
{
    this->calInput.projBoardSqPxW = this->spinProjBoardSqPxW->GetValue();
}

void RCalibWindow::OnSpinProjBoardSqPxHUpdated(wxCommandEvent& event)
{
    this->calInput.projBoardSqPxH = this->spinProjBoardSqPxH->GetValue();
}

void RCalibWindow::OnSpinNumFramesUpdated(wxCommandEvent& event)
{
    this->calInput.numberBoards = this->spinNumFrames->GetValue();
}