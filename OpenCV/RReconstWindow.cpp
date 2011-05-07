/* 
 * File:   RReconstWindow.cpp
 * Author: jenicek
 * 
 * Created on July 28, 2010, 7:08 AM
 */

#include "RReconstWindow.h"

RReconstWindow::RReconstWindow(wxFrame* parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    this->bAllowRepaint = true;

    this->sceneWnd = NULL;

    this->imgWidth = 640;
    this->imgHeight = 480;
    this->cutoffThresh = 0.0f;

    this->numPhImages = 3;

    this->cmColors = NULL;
    this->cmDepth = NULL;
    this->cmIRatio = NULL;
    this->cmMask = NULL;
    this->cmWPhase = NULL;
    this->cmUPhase = NULL;
    this->cmImg3ChInt = NULL;
    this->cmProcess = NULL;
    this->phaseUnwStart = cvPoint(0, 0);
    this->errLut = NULL;

    this->phaseImages = NULL;

    this->phImagesPath = _("");

    this->fringePitch = 0.0f;
    this->planeDist = 0.0f;
    this->projCamAngle = 0.0f;
    this->projCamDist = 0.0f;
    this->cutoffThresh = cutoffThresh;

    // Create menu
    // Create menu file
    this->menuFile = new wxMenu;
    this->menuFile->Append(ID_MENU_SAVE_PLY, "Save PL&Y");
    this->menuFile->Enable(ID_MENU_SAVE_PLY, false);
    this->menuFile->AppendSeparator();
    this->menuFile->Append(ID_MENU_QUIT, "E&xit");
    // Create menu window
    wxMenu *menuWindow = new wxMenu;
    menuWindow->Append(ID_MENU_CALIB, "&Calibration");
    menuWindow->Append(ID_MENU_CAPT, "Ca&pture");
    menuWindow->Append(ID_MENU_3D, "&3&D Scene");
    // Create submenu reconstruction
    this->submWrap = new wxMenu;
    this->submWrap->AppendRadioItem(ID_MENU_RADIO_3PHASE, "&3-Phase shifting");
    this->submWrap->AppendRadioItem(ID_MENU_RADIO_4PHASE, "&4-Phase shifting");
    // Create submenu unwrapping
    this->submUnwrap = new wxMenu;
    this->submUnwrap->AppendRadioItem(ID_MENU_RADIO_UWFLOODFILL, "&Simple Flood-Fill");
    this->submUnwrap->AppendRadioItem(ID_MENU_RADIO_UWSCANLINE, "&Quality guided Scan-Line");
    // Create menu settings
    wxMenu *menuSettings = new wxMenu;
    menuSettings->AppendSubMenu(this->submWrap, "&Phase wrapping");
    menuSettings->AppendSubMenu(this->submUnwrap, "&Phase unwrapping");
    // Create menu help
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(ID_MENU_ABOUT, "&About...");
    // Create menu bar
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(this->menuFile, "&File");
    menuBar->Append(menuWindow, "&Window");
    menuBar->Append(menuSettings, "&Settings");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    // Register events
    this->Connect(-1, wxEVT_PAINT, (wxObjectEventFunction)&RReconstWindow::OnReconstWndPaint);
    this->Connect(ID_MENU_QUIT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuQuit);
    this->Connect(ID_MENU_ABOUT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuAbout);
    this->Connect(ID_MENU_CALIB, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuCalib);
    this->Connect(ID_MENU_CAPT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuCapt);
    this->Connect(ID_MENU_3D, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuScene);
    this->Connect(ID_MENU_SAVE_PLY, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuSavePly);
    this->Connect(ID_MENU_RADIO_3PHASE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenu3PhaseSelected);
    this->Connect(ID_MENU_RADIO_4PHASE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenu4PhaseSelected);
    this->Connect(ID_MENU_RADIO_UWFLOODFILL, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuUWFFSelected);
    this->Connect(ID_MENU_RADIO_UWSCANLINE, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnMenuUWSLSelected);
}

RReconstWindow::~RReconstWindow()
{
    if(this->phaseImages)
    {
        for(int i = 0; i < this->numPhImages; i++)
        {
            if(this->phaseImages[i])
                cvReleaseImage(&phaseImages[i]);
        }
        free(this->phaseImages);
    }

    if(this->cmWPhase)
        cvReleaseMat(&this->cmWPhase);
    if(this->cmUPhase)
        cvReleaseMat(&this->cmUPhase);
    if(this->cmMask)
        cvReleaseMat(&this->cmMask);
    if(this->cmProcess)
        cvReleaseMat(&this->cmProcess);
    if(this->cmColors)
        cvReleaseMat(&this->cmColors);
    if(this->cmIRatio)
        cvReleaseMat(&this->cmIRatio);
    if(this->cmDepth)
        cvReleaseMat(&this->cmDepth);
    if(this->cmImg3ChInt)
        cvReleaseMat(&this->cmImg3ChInt);

    if(this->errLut)
        ReleaseLUT(&this->errLut);
}

bool RReconstWindow::Initialize(char* phImagesPath, float cutoffThresh, float pcAngle, float pcDist, float plDist, float fPitch)
{
    this->phImagesPath = _(phImagesPath);
    
    // Initialize and load phase images
    this->numPhImages = 3;
    this->phaseImages = (IplImage**)malloc(sizeof(IplImage*) * this->numPhImages);
    char fName[256];
    for(int i = 0; i < this->numPhImages; i++)
    {
        sprintf(fName, "%s/phase%d.jpg", phImagesPath, i + 1);
        this->phaseImages[i] = cvLoadImage((char*)fName, CV_LOAD_IMAGE_UNCHANGED);
        if(!this->phaseImages[i])
            return false;
    }

    // Set image width and height
    this->imgWidth = this->phaseImages[0]->widthStep / 3;
    this->imgHeight = this->phaseImages[0]->height;

    // Set cuttof threshold
    this->cutoffThresh = cutoffThresh;

    // Allocate storages
    this->cmWPhase = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmUPhase = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmMask = cvCreateMat(imgHeight, imgWidth, CV_8UC1);
    this->cmProcess = cvCreateMat(imgHeight, imgWidth, CV_8UC1);
    this->cmIRatio = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmColors = cvCreateMat(imgHeight, imgWidth, CV_32FC3);
    this->cmDepth = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmImg3ChInt = cvCreateMat(imgHeight, imgWidth, CV_8UC3);

    // Allocate memory for error comepnsation lut
    this->errLut = (double*)malloc(LUT_SIZE * sizeof(double));
    CreateLUT(&this->errLut, LUT_SIZE);

    // Set depth reconstruction values
    this->projCamAngle = pcAngle;
    this->projCamDist = pcDist;
    this->planeDist = plDist;
    this->fringePitch = fPitch;

    // Place controls
    // Create comboboxes for choosing which images to be shown
    const wxString cbxChooseVals[7] = {_("Wrapped phase"), _("Unwrapped phase"), _("Intensity ratio"), _("Process map"), _("Mask map"),
        _("Color map"), _("Depth map")};
    this->cbxChooseView1 = new wxComboBox(this, CBX_CHOOSE_1, cbxChooseVals[0], wxPoint(475, 20), wxSize(160, 30), 7, cbxChooseVals);
    this->cbxChooseView1->SetSelection(0);
    this->cbxChooseView2 = new wxComboBox(this, CBX_CHOOSE_2, cbxChooseVals[1], wxPoint(475, 315), wxSize(160, 30), 7, cbxChooseVals);
    this->cbxChooseView2->SetSelection(1);
    // Create buttons for showing full size images
    this->btnFullSize1 = new wxButton(this, BTN_FULL_SIZE_1, _("Full size"), wxPoint(this->cbxChooseView1->m_x + 200, this->cbxChooseView1->m_y));
    this->btnFullSize2 = new wxButton(this, BTN_FULL_SIZE_2, _("Full size"), wxPoint(this->cbxChooseView2->m_x + 200, this->cbxChooseView2->m_y));
    // Create reconstruct button and checkbox
    this->btnReconst = new wxButton(this, BTN_RECONST, _("Reconstruct 3D"), wxPoint(80, 20));
    this->chxActReconst = new wxCheckBox(this, wxID_ANY, _("Actualize on change"), wxPoint(this->btnReconst->m_x + 150,
            this->btnReconst->m_y + 5));
    // Phase settings
    this->panelPhSet = new wxPanel(this, wxID_ANY, wxPoint(30, this->btnReconst->m_y + 50), wxSize(410, 200));
    this->panelPhSet->SetBackgroundColour(wxColour(200, 200, 200));
    this->lblPhaseSet = new wxStaticText(this->panelPhSet, wxID_ANY, _("Phase settings:"), wxPoint(10, 10));
    this->chxReconstPhase = new wxCheckBox(this->panelPhSet, wxID_ANY, _("Reconstruct phase"), wxPoint(this->panelPhSet->m_x +
            this->panelPhSet->m_width - 180, 5));
    this->chxReconstPhase->SetValue(true);
    this->lblPhImgStorage = new wxStaticText(this->panelPhSet, wxID_ANY, _("Ph. imgs.:"), wxPoint(20, this->lblPhaseSet->m_y + 30));
    this->txtPhImgStorage = new wxTextCtrl(this->panelPhSet, TXT_PHASE_STORAGE, _(""), wxPoint(this->lblPhImgStorage->m_x + 75,
            this->lblPhImgStorage->m_y - 5), wxSize(220, 25), wxTE_PROCESS_ENTER);
    this->btnPhImgStorage = new wxButton(this->panelPhSet, BTN_PHASE_STORAGE, _("Choose..."), wxPoint(this->txtPhImgStorage->m_x + 225,
            this->lblPhImgStorage->m_y - 8));
    this->txtPhImgStorage->SetValue(this->phImagesPath);
    this->lblCutoffThresh = new wxStaticText(this->panelPhSet, wxID_ANY, _("Cutoff threshold:"), wxPoint(20, this->lblPhImgStorage->m_y + 40));
    this->sldrCutoffThresh = new wxSlider(this->panelPhSet, SLDR_CUTOFF_TH, (int)(this->cutoffThresh * 10), 0, 1000, wxPoint(30, this->lblCutoffThresh->m_y + 20),
            wxSize(290, 25));
    this->sldrCutoffThresh->SetPageSize(1);
    this->txtCutoffThresh = new wxTextCtrl(this->panelPhSet, TXT_CUTOFF_TH, wxString::Format("%.2f", this->cutoffThresh), wxPoint(this->sldrCutoffThresh->m_x + 295,
            this->sldrCutoffThresh->m_y), wxSize(50, 25), wxTE_PROCESS_ENTER);
    this->chxUseLUT = new wxCheckBox(this->panelPhSet, CHX_USE_LUT, _("Linearize phase"), wxPoint(20, this->sldrCutoffThresh->m_y + 40));
    this->chxUseLUT->SetValue(false);
    // Depth settings
    this->panelDepthSet = new wxPanel(this, wxID_ANY, wxPoint(30, this->panelPhSet->m_y + 220), wxSize(410, 300));
    this->panelDepthSet->SetBackgroundColour(wxColour(200, 200, 200));
    this->lblDepthSet = new wxStaticText(this->panelDepthSet, wxID_ANY, _("Depth settings:"), wxPoint(10, 10));
    this->chxReconstDepth = new wxCheckBox(this->panelDepthSet, wxID_ANY, _("Reconstruct depth"), wxPoint(this->panelDepthSet->m_x +
            this->panelDepthSet->m_width - 180, 5));
    this->chxReconstDepth->SetValue(true);
    this->lblDepthMet = new wxStaticText(this->panelDepthSet, wxID_ANY, _("Method:"), wxPoint(20, this->lblDepthSet->m_y + 30));
    const wxString cbxDMVals[3] = {_("1"), _("2"), _("3")};
    this->cbxDepthMethod = new wxComboBox(this->panelDepthSet, CBX_DEPTH_MET, cbxDMVals[0], wxPoint(this->lblDepthMet->m_x + 60, this->lblDepthMet->m_y - 5),
            wxSize(100, 25), 3, cbxDMVals);
    this->cbxDepthMethod->SetSelection(0);
    this->lblPatType = new wxStaticText(this->panelDepthSet, wxID_ANY, _("Pattern:"), wxPoint(this->lblDepthMet->m_x + 190, this->lblDepthMet->m_y));
    const wxString cbxPTVals[2] = {_("Horizontal"), _("Vertical")};
    this->cbxPatternType = new wxComboBox(this->panelDepthSet, CBX_PAT_TYPE, cbxPTVals[0], wxPoint(this->lblPatType->m_x + 60, this->lblPatType->m_y - 5),
            wxSize(125, 25), 2, cbxPTVals);
    this->cbxPatternType->SetSelection(0);

    this->panelDepthM1 = new wxPanel(this->panelDepthSet, wxID_ANY, wxPoint(25, this->lblDepthMet->m_y + 25), wxSize(380, 230));
    this->panelDepthM1->SetBackgroundColour(wxColour(200, 200, 200));
    this->lblZScale = new wxStaticText(this->panelDepthM1, wxID_ANY, _("ZScale:"), wxPoint(0, 5));
    this->sldrZScale = new wxSlider(this->panelDepthM1, SLDR_ZSCALE, (int)(this->projCamAngle * 10), -5000, 5000, wxPoint(10, this->lblZScale->m_y + 15),
            wxSize(290, 25));
    this->sldrZScale->SetPageSize(1);
    this->txtZScale = new wxTextCtrl(this->panelDepthM1, TXT_ZSCALE, wxString::Format("%.2f", this->projCamAngle), wxPoint(this->sldrZScale->m_x + 295,
            this->sldrZScale->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);
    this->lblZSkew = new wxStaticText(this->panelDepthM1, wxID_ANY, _("ZSkew:"), wxPoint(0, this->sldrZScale->m_y + 40));
    this->sldrZSkew = new wxSlider(this->panelDepthM1, SLDR_ZSKEW, (int)(this->projCamDist * 10), -5000, 5000, wxPoint(10, this->lblZSkew->m_y + 15),
            wxSize(290, 25));
    this->sldrZSkew->SetPageSize(1);
    this->txtZSKew = new wxTextCtrl(this->panelDepthM1, TXT_ZSKEW, wxString::Format("%.2f", this->projCamDist), wxPoint(this->sldrZSkew->m_x + 295,
            this->sldrZSkew->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);

    this->panelDepthM2 = new wxPanel(this->panelDepthSet, wxID_ANY, wxPoint(25, this->lblDepthMet->m_y + 25), wxSize(380, 230));
    this->panelDepthM2->SetBackgroundColour(wxColour(200, 200, 200));
    this->panelDepthM2->Hide();
    this->lblPCAngle = new wxStaticText(this->panelDepthM2, wxID_ANY, _("PCAngle:"), wxPoint(0, 5));
    this->sldrPCAngle = new wxSlider(this->panelDepthM2, SLDR_PCANGLE, (int)(this->projCamAngle * 100), 0, 18000, wxPoint(10, this->lblPCAngle->m_y + 15),
            wxSize(290, 25));
    this->sldrPCAngle->SetPageSize(1);
    this->txtPCAngle = new wxTextCtrl(this->panelDepthM2, TXT_PCANGLE, wxString::Format("%.2f", this->projCamAngle), wxPoint(this->sldrPCAngle->m_x + 295,
            this->sldrPCAngle->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);
    this->lblPCDist = new wxStaticText(this->panelDepthM2, wxID_ANY, _("PCDist:"), wxPoint(0, this->sldrPCAngle->m_y + 40));
    this->sldrPCDist = new wxSlider(this->panelDepthM2, SLDR_PCDIST, (int)(this->projCamDist * 10), 0, 30000, wxPoint(10, this->lblPCDist->m_y + 15),
            wxSize(290, 25));
    this->sldrPCDist->SetPageSize(10);
    this->txtPCDist = new wxTextCtrl(this->panelDepthM2, TXT_PCDIST, wxString::Format("%.2f", this->projCamDist), wxPoint(this->sldrPCDist->m_x + 295,
            this->sldrPCDist->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);
    this->lblPlDist = new wxStaticText(this->panelDepthM2, wxID_ANY, _("PlDist:"), wxPoint(0, this->sldrPCDist->m_y + 40));
    this->sldrPlDist = new wxSlider(this->panelDepthM2, SLDR_PLDIST, (int)(this->planeDist * 10), 0, 30000, wxPoint(10, this->lblPlDist->m_y + 15),
            wxSize(290, 25));
    this->sldrPlDist->SetPageSize(10);
    this->txtPlDist = new wxTextCtrl(this->panelDepthM2, TXT_PLDIST, wxString::Format("%.2f", this->planeDist), wxPoint(this->sldrPlDist->m_x + 295,
            this->sldrPlDist->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);
    this->lblFPitch = new wxStaticText(this->panelDepthM2, wxID_ANY, _("FPitch:"), wxPoint(0, this->sldrPlDist->m_y + 40));
    this->sldrFPitch = new wxSlider(this->panelDepthM2, SLDR_FPITCH, (int)(this->fringePitch * 10), 0, 1000, wxPoint(10, this->lblFPitch->m_y + 15),
            wxSize(290, 25));
    this->sldrFPitch->SetPageSize(1);
    this->txtFPitch = new wxTextCtrl(this->panelDepthM2, TXT_FPITCH, wxString::Format("%.2f", this->fringePitch), wxPoint(this->sldrFPitch->m_x + 295,
            this->sldrFPitch->m_y), wxSize(70, 25), wxTE_PROCESS_ENTER);

    this->panelDepthM3 = new wxPanel(this->panelDepthSet, wxID_ANY, wxPoint(25, this->lblDepthMet->m_y + 25), wxSize(380, 230));
    this->panelDepthM3->SetBackgroundColour(wxColour(200, 200, 200));
    this->panelDepthM3->Hide();


    // Register events
    this->Connect(CBX_CHOOSE_1, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnCbxChooseView1Selected);
    this->Connect(CBX_CHOOSE_2, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnCbxChooseView2Selected);
    this->Connect(BTN_FULL_SIZE_1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RReconstWindow::OnBtnFullSize1Clicked);
    this->Connect(BTN_FULL_SIZE_2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RReconstWindow::OnBtnFullSize2Clicked);
    this->Connect(BTN_RECONST, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RReconstWindow::OnBtnReconstClicked);
    this->Connect(TXT_PHASE_STORAGE, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtPhImgStorageEnter);
    this->Connect(BTN_PHASE_STORAGE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RReconstWindow::OnBtnPhImgStorageClick);
    this->Connect(TXT_CUTOFF_TH, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtCutoffThEnter);
    this->Connect(SLDR_CUTOFF_TH, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrCutoffThScroll);
    this->Connect(CBX_DEPTH_MET, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnCbxDepthMetSelected);
    this->Connect(SLDR_ZSCALE, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrZScaleScroll);
    this->Connect(SLDR_ZSKEW, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrZSkewScroll);
    this->Connect(TXT_ZSCALE, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtZScaleEnter);
    this->Connect(TXT_ZSKEW, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtZSkewEnter);
    this->Connect(SLDR_PCANGLE, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrPCAngleScroll);
    this->Connect(SLDR_PCDIST, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrPCDistScroll);
    this->Connect(TXT_PCANGLE, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtPCAngleEnter);
    this->Connect(TXT_PCDIST, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtPCDistEnter);
    this->Connect(SLDR_PLDIST, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrPlDistScroll);
    this->Connect(SLDR_FPITCH, wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&RReconstWindow::OnSldrFPitchScroll);
    this->Connect(TXT_PLDIST, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtPlDistEnter);
    this->Connect(TXT_FPITCH, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&RReconstWindow::OnTxtFPitchEnter);
    this->Connect(CBX_PAT_TYPE, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&RReconstWindow::OnCbxPatTypeSelected);
    this->Connect(CHX_USE_LUT, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&RReconstWindow::OnChxUseLUTClicked);

    return true;
}

bool RReconstWindow::ReloadPhaseImages(void)
{
    this->bAllowRepaint= false;

    if(this->phaseImages)
    {
        for(int i = 0; i < numPhImages; i++)
        {
            if(this->phaseImages[i])
                cvReleaseImage(&phaseImages[i]);
        }
        free(this->phaseImages);
    }

    // Release old storages
    if(this->submWrap->IsChecked(ID_MENU_RADIO_3PHASE))
        this->numPhImages = 3;
    else if(this->submWrap->IsChecked(ID_MENU_RADIO_4PHASE))
        this->numPhImages = 4;

    if(this->cmWPhase)
        cvReleaseMat(&this->cmWPhase);
    if(this->cmUPhase)
        cvReleaseMat(&this->cmUPhase);
    if(this->cmMask)
        cvReleaseMat(&this->cmMask);
    if(this->cmProcess)
        cvReleaseMat(&this->cmProcess);
    if(this->cmColors)
        cvReleaseMat(&this->cmColors);
    if(this->cmIRatio)
        cvReleaseMat(&this->cmIRatio);
    if(this->cmDepth)
        cvReleaseMat(&this->cmDepth);
    if(this->cmImg3ChInt)
        cvReleaseMat(&this->cmImg3ChInt);

    // Initialize and load new phase images
    this->phaseImages = (IplImage**)malloc(sizeof(IplImage*) * this->numPhImages);
    char fName[256];
    for(int i = 0; i < this->numPhImages; i++)
    {
        sprintf(fName, "%s/phase%d.jpg", this->phImagesPath.ToAscii(), i + 1);
        this->phaseImages[i] = cvLoadImage((char*)fName, CV_LOAD_IMAGE_UNCHANGED);
        if(!this->phaseImages[i])
            return false;
    }

    // Set new image width and height
    this->imgWidth = this->phaseImages[0]->widthStep / 3;
    this->imgHeight = this->phaseImages[0]->height;

    // Allocate new storages
    this->cmWPhase = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmUPhase = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmMask = cvCreateMat(imgHeight, imgWidth, CV_8UC1);
    this->cmProcess = cvCreateMat(imgHeight, imgWidth, CV_8UC1);
    this->cmIRatio = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmColors = cvCreateMat(imgHeight, imgWidth, CV_32FC3);
    this->cmDepth = cvCreateMat(imgHeight, imgWidth, CV_32FC1);
    this->cmImg3ChInt = cvCreateMat(imgHeight, imgWidth, CV_8UC3);

    this->bAllowRepaint = true;
}

void RReconstWindow::DrawReconstImage(T_ViewID id, int imageIndex)
{
    // Create bitmap with proper size (according to the image index) and draw
    // Get image size (it's the same for all the images)
    CvSize imgSize = cvSize(this->cmWPhase->cols, this->cmWPhase->rows);
    switch(imageIndex)
    {
        // Wrapped phase
        case 0:
            Img1FLTo3UCH(this->cmWPhase, &this->cmImg3ChInt);
            break;
        // Unwrapped phase
        case 1:
            Img1FLTo3UCH(this->cmUPhase, &this->cmImg3ChInt);
            break;
        // Intensity ratio
        case 2:
            Img1FLTo3UCH(this->cmIRatio, &this->cmImg3ChInt);
            break;
        // Process map
        case 3:
            Img1UCHTo3UCH(this->cmProcess, &this->cmImg3ChInt);
            ChangeImg3CHUCH(&this->cmImg3ChInt, 255);
            break;
        // Mask map
        case 4:
            Img1UCHTo3UCH(this->cmMask, &this->cmImg3ChInt);
            ChangeImg3CHUCH(&this->cmImg3ChInt, 255);
            break;
        // Color map
        case 5:
            ImgFL3ToUCH3(this->cmColors, &this->cmImg3ChInt);
            break;
        // Depth map
        case 6:
            Img1FLTo3UCH(this->cmDepth, &this->cmImg3ChInt);
            break;
        // Others
        default:
            break;
    }
    wxImage img = wxImage(imgSize.width, imgSize.height, this->cmImg3ChInt->data.ptr, true);
    wxBitmap bmp = wxBitmap(img.Scale(320, 240));
    
    // Get image position and label
    int imgPosX = 470;
    int imgPosY = 55;
    wxString imgLabel = this->cbxChooseView1->GetValue();
    if(id == IMG_VIEW_2)
    {
        imgPosY += 295;
        imgLabel = this->cbxChooseView2->GetValue();
    }
    // Get image label

    // Get drawing context
    wxPaintDC dc(this);
    // Set font and brush
    dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
    dc.SetTextForeground(*wxWHITE);
    dc.SetBrush(wxBrush(wxColour(50, 50, 50), wxSOLID));
    // Draw the bitmap
    dc.DrawBitmap(bmp, wxPoint(imgPosX, imgPosY));
    // Draw bitmap label
    dc.DrawText(imgLabel, wxPoint(imgPosX + 5, imgPosY + 5));
    
    // Release drawing context
    dc.Destroy();
}

void RReconstWindow::DrawRecImgFullsize(int imageIndex)
{
    char winName[256] = "\0";
    switch(imageIndex)
    {
        // Wrapped phase
        case 0:
            sprintf(winName, "Wrapped phase");
            Img1FLTo3UCH(this->cmWPhase, &this->cmImg3ChInt);
            break;
        // Unwrapped phase
        case 1:
            sprintf(winName, "Unwrapped phase");
            Img1FLTo3UCH(this->cmUPhase, &this->cmImg3ChInt);
            break;
        // Intensity ratio
        case 2:
            sprintf(winName, "Intensity ratio");
            Img1FLTo3UCH(this->cmIRatio, &this->cmImg3ChInt);
            break;
        // Process map
        case 3:
            sprintf(winName, "Process map");
            Img1UCHTo3UCH(this->cmProcess, &this->cmImg3ChInt);
            ChangeImg3CHUCH(&this->cmImg3ChInt, 255);
            break;
        // Mask map
        case 4:
            sprintf(winName, "Mask map");
            Img1UCHTo3UCH(this->cmMask, &this->cmImg3ChInt);
            ChangeImg3CHUCH(&this->cmImg3ChInt, 255);
            break;
        // Color map
        case 5:
            sprintf(winName, "Color map");
            ImgFL3ToUCH3(this->cmColors, &this->cmImg3ChInt);
            break;
        // Depth map
        case 6:
            sprintf(winName, "Depth map");
            Img1FLTo3UCH(this->cmDepth, &this->cmImg3ChInt);
            break;
        // Others
        default:
            break;
    }
    // Show the image
    cvShowImage(winName, this->cmImg3ChInt);
}

void RReconstWindow::RedrawReconstWnds(void)
{
    // Redraw reconst images
    this->DrawReconstImage(IMG_VIEW_1, this->cbxChooseView1->GetCurrentSelection());
    this->DrawReconstImage(IMG_VIEW_2, this->cbxChooseView2->GetCurrentSelection());
}

void RReconstWindow::ReconstIfAllowed(bool allowed)
{
    if(allowed)
    {
        this->ReconstPhase(this->chxReconstPhase->GetValue());
        this->ReconstDepth(this->chxReconstDepth->GetValue());
        this->RedrawReconstWnds();
    }
}

bool RReconstWindow::Reconstruct3D(bool allowed)
{
    this->ReconstPhase(allowed);
    this->ReconstDepth(allowed);
    this->RedrawReconstWnds();
    
    return true;
}

bool RReconstWindow::ReconstPhase(bool allowed)
{
    if(allowed)
    {
        // Reset storage matrices
        cvSet(this->cmWPhase, cvScalarAll(0), NULL);
        cvSet(this->cmUPhase, cvScalarAll(0), NULL);
        cvSet(this->cmMask, cvScalarAll(1), NULL);
        cvSet(this->cmProcess, cvScalarAll(0), NULL);
        cvSet(this->cmIRatio, cvScalarAll(0), NULL);
        cvSet(this->cmColors, cvScalarAll(0), NULL);

        // Apply chosen phase wrapping method
        if(this->submWrap->IsChecked(ID_MENU_RADIO_3PHASE))
        {
            // Do reconstruction process
            // Wrap three phase images given from camera capture
            PhaseWrap(this->phaseImages, &this->cmWPhase, &this->cmProcess, &this->cmIRatio, &this->cmColors,
                    this->cutoffThresh);
            // If chosen, use LUT to compensate phase errors
            //if(this->chxUseLUT->GetValue())
                //ApplyLUT(&this->cmWPhase, this->errLut, LUT_SIZE);
        }
        else if(this->submWrap->IsChecked(ID_MENU_RADIO_4PHASE))
        {
            // Do reconstruction process
            // Wrap three phase images given from camera capture
            PhaseWrap4(this->phaseImages, &this->cmWPhase, &this->cmProcess, &this->cmIRatio, &this->cmColors,
                    this->cutoffThresh);
        }

        // Copy wrapped phase map to unwrapped phase map
        cvCopy(this->cmWPhase, this->cmUPhase, NULL);

        // Apply chosen phase unwrapping method
        if(this->submUnwrap->IsChecked(ID_MENU_RADIO_UWFLOODFILL))
        {
            // Use Flood-Fill unwrapping algorithm to unwrap the wrapped phase
            this->phaseUnwStart = PhaseUnwrap(&(this->cmUPhase), &(this->cmProcess), &(this->cmMask));
        }
        else if(this->submUnwrap->IsChecked(ID_MENU_RADIO_UWSCANLINE))
        {
            // Use quality guided Scan-Line unwrapping algorithm described in
            // Multilevel quality-guided phase unwrapping algorithm for real-time three-dimensional shape reconstruction
            // by Zhang, Li and Xau
            this->phaseUnwStart = GetStart(this->cmProcess);
            CvMat *quality = cvCreateMat(this->cmWPhase->rows, this->cmWPhase->cols, CV_32FC1);
            CreateQualityMap(this->cmWPhase, this->cmProcess, &quality);
            float *thresholds = (float*)malloc(4 * sizeof(float));
            ComputeThresholds(quality, &thresholds, 4);
            PhaseUnwrapScanLine(&this->cmUPhase, &this->cmMask, &quality, thresholds, 4);
            cvReleaseMat(&quality);
        }

        // Remove noise from unwrapped phase using Gaussian filter
        if(this->chxUseLUT->GetValue())
            PhaseRemNoise(&this->cmUPhase, this->cmMask);
        
        // If the scene window is shown
        if(this->GetChildren().IndexOf(this->sceneWnd) != -1)
        {
            // 3D scene window exists, update color map directly
            this->sceneWnd->UpdateGLColors(this->cmColors);
            // also update mask directly
            this->sceneWnd->UpdateMask(this->cmMask);
            this->sceneWnd->RedrawScene();
        }
        else
        {
            // Save color map to it's ordinary storage
            SaveColorMap((char*)"colors.csv", this->cmColors);
            // Save mask map to it's ordinary storage
            SaveMask((char*)"mask.csv", this->cmMask);
        }
    }

    return true;
}

bool RReconstWindow::ReconstDepth(bool allowed)
{
    if(allowed)
    {
        // Reset storage matrix
        cvSet(this->cmDepth, cvScalarAll(0), NULL);

        // Compute depth map from phase map
        switch(this->cbxDepthMethod->GetCurrentSelection())
        {
            case 0:
                MakeDepth(this->cmUPhase, this->cmMask, &this->cmDepth, this->phaseUnwStart, this->projCamAngle,
                        this->projCamDist, this->cbxPatternType->GetCurrentSelection());
                break;
            case 1:
                //MakeDepth4(this->cmUPhase, this->cmMask, &this->cmUPhase);
                MakeDepth3(this->cmUPhase, this->cmMask, &this->cmDepth, this->projCamAngle, this->projCamDist,
                        this->planeDist, this->fringePitch);
                break;
            case 2:
                MakeDepth4(this->cmWPhase, this->cmMask, &this->cmDepth);
                break;
            default:
                break;
        }

        // If the scene window is shown
        if(this->GetChildren().IndexOf(this->sceneWnd) != -1)
        {
            // 3D scene window exists, show depth map directly
            this->sceneWnd->UpdateGLVertices(this->cmDepth);
            this->sceneWnd->RedrawScene();
        }
        else
            // Save depth map to it's ordinary storage
            SavePhaseMap((char*)"points.csv", this->cmDepth);

        // Enable Save PLY menu button
        if(!this->menuFile->IsEnabled(ID_MENU_SAVE_PLY))
            this->menuFile->Enable(ID_MENU_SAVE_PLY, true);
    }

    return true;
}

void RReconstWindow::OnReconstWndPaint(wxPaintEvent& event)
{
    if(this->bAllowRepaint)
    {
        // Redraw reconst images
        this->DrawReconstImage(IMG_VIEW_1, this->cbxChooseView1->GetCurrentSelection());
        this->DrawReconstImage(IMG_VIEW_2, this->cbxChooseView2->GetCurrentSelection());
    }
}

void RReconstWindow::OnCbxChooseView1Selected(wxCommandEvent& event)
{
    // Redraw reconst image
    this->DrawReconstImage(IMG_VIEW_1, this->cbxChooseView1->GetCurrentSelection());
}

void RReconstWindow::OnCbxChooseView2Selected(wxCommandEvent& event)
{
    // Redraw reconst image
    this->DrawReconstImage(IMG_VIEW_2, this->cbxChooseView2->GetCurrentSelection());
}

void RReconstWindow::OnBtnFullSize1Clicked(wxCommandEvent& event)
{
    // Show current image in fullsize
    this->DrawRecImgFullsize(this->cbxChooseView1->GetCurrentSelection());
}

void RReconstWindow::OnBtnFullSize2Clicked(wxCommandEvent& event)
{
    // Show current image in fullsize
    this->DrawRecImgFullsize(this->cbxChooseView2->GetCurrentSelection());
}

void RReconstWindow::OnBtnReconstClicked(wxCommandEvent& event)
{
    // Reconstruct the scene using given phase images
    this->Reconstruct3D(true);
}

void RReconstWindow::OnTxtPhImgStorageEnter(wxCommandEvent& event)
{
    // Set new phase images storage path
    if(wxDir::Exists(this->txtPhImgStorage->GetValue()))
    {
        this->phImagesPath = this->txtPhImgStorage->GetValue();
        this->ReloadPhaseImages();
        if(!this->btnReconst->IsEnabled())
            this->btnReconst->Enable(true);
        this->txtPhImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
    }
    else
    {
        this->btnReconst->Disable();
        this->txtPhImgStorage->SetBackgroundColour(wxColour(255, 0, 0));
    }
}

void RReconstWindow::OnBtnPhImgStorageClick(wxCommandEvent& event)
{
    // Show choose directory dialog
    wxDirDialog *dlg = new wxDirDialog(this, _("Choose directory"), this->phImagesPath, 0, wxDefaultPosition);
    if(dlg->ShowModal() == wxID_OK)
    {
        this->phImagesPath = dlg->GetPath();
        this->ReloadPhaseImages();
        this->txtPhImgStorage->SetValue(this->phImagesPath);
        this->txtPhImgStorage->SetBackgroundColour(wxColour(255, 255, 255));
        if(!this->btnReconst->IsEnabled())
            this->btnReconst->Enable(true);
    }
}

void RReconstWindow::OnTxtCutoffThEnter(wxCommandEvent& event)
{
    this->cutoffThresh = atof(this->txtCutoffThresh->GetValue());

    if(this->cutoffThresh > 100.0)
    {
        this->cutoffThresh = 100.0;
        this->txtCutoffThresh->SetValue(wxString::Format("%.2f", this->cutoffThresh));
    }
    else if(this->cutoffThresh < 0.0)
    {
        this->cutoffThresh = 0.0;
        this->txtCutoffThresh->SetValue(wxString::Format("%.2f", this->cutoffThresh));
    }
    
    this->sldrCutoffThresh->SetValue((int)(this->cutoffThresh * 10));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrCutoffThScroll(wxCommandEvent& event)
{
    this->cutoffThresh = this->sldrCutoffThresh->GetValue() / 10.0;
    this->txtCutoffThresh->SetValue(wxString::Format("%.2f", this->cutoffThresh));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnCbxDepthMetSelected(wxCommandEvent& event)
{
    switch(this->cbxDepthMethod->GetCurrentSelection())
    {
        case 0:
            this->panelDepthM1->Show(true);
            this->panelDepthM2->Hide();
            this->panelDepthM3->Hide();
            this->projCamAngle = this->sldrZScale->GetValue() / 10.0;
            this->projCamDist = this->sldrZSkew->GetValue() / 10.0;
            break;
        case 1:
            this->panelDepthM1->Hide();
            this->panelDepthM2->Show(true);
            this->panelDepthM3->Hide();
            break;
        case 2:
            this->panelDepthM1->Hide();
            this->panelDepthM2->Hide();
            this->panelDepthM3->Show(true);
            break;
        default:
            break;
    }
}

void RReconstWindow::OnCbxPatTypeSelected(wxCommandEvent& event)
{
    if(this->chxReconstDepth->GetValue())
        this->ReconstDepth(true);
}

void RReconstWindow::OnSldrZScaleScroll(wxCommandEvent& event)
{
    this->projCamAngle = this->sldrZScale->GetValue() / 10.0;
    this->txtZScale->SetValue(wxString::Format("%.2f", this->projCamAngle));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrZSkewScroll(wxCommandEvent& event)
{
    this->projCamDist = this->sldrZSkew->GetValue() / 10.0;
    this->txtZSKew->SetValue(wxString::Format("%.2f", this->projCamDist));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtZScaleEnter(wxCommandEvent& event)
{
    this->projCamAngle = atof(this->txtZScale->GetValue());

    if(this->projCamAngle > 500.0)
    {
        this->projCamAngle = 500.0;
        this->txtZScale->SetValue(wxString::Format("%.2f", this->projCamAngle));
    }
    else if(this->projCamAngle < -500.0)
    {
        this->projCamAngle = -500.0;
        this->txtZScale->SetValue(wxString::Format("%.2f", this->projCamAngle));
    }
    
    this->sldrZScale->SetValue((int)(this->projCamAngle * 10));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtZSkewEnter(wxCommandEvent& event)
{
    this->projCamDist = atof(this->txtZSKew->GetValue());

    if(this->projCamDist > 500.0)
    {
        this->projCamDist = 500.0;
        this->txtZSKew->SetValue(wxString::Format("%.2f", this->projCamDist));
    }
    else if(this->projCamDist < -500.0)
    {
        this->projCamDist = -500.0;
        this->txtZSKew->SetValue(wxString::Format("%.2f", this->projCamDist));
    }
    
    this->sldrZSkew->SetValue((int)(this->projCamDist * 10));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrPCAngleScroll(wxCommandEvent& event)
{
    this->projCamAngle = this->sldrPCAngle->GetValue() / 100.0;
    this->txtPCAngle->SetValue(wxString::Format("%.2f", this->projCamAngle));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrPCDistScroll(wxCommandEvent& event)
{
    this->projCamDist = this->sldrPCDist->GetValue() / 10.0;
    this->txtPCDist->SetValue(wxString::Format("%.2f", this->projCamDist));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtPCAngleEnter(wxCommandEvent& event)
{
    this->projCamAngle = atof(this->txtPCAngle->GetValue());

    if(this->projCamAngle > 180.0)
    {
        this->projCamAngle = 180.0;
        this->txtPCAngle->SetValue(wxString::Format("%.2f", this->projCamAngle));
    }
    else if(this->projCamAngle < 0.0)
    {
        this->projCamAngle = 0.0;
        this->txtPCAngle->SetValue(wxString::Format("%.2f", this->projCamAngle));
    }
    
    this->sldrPCAngle->SetValue((int)(this->projCamAngle * 100));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtPCDistEnter(wxCommandEvent& event)
{

    this->projCamDist = atof(this->txtPCDist->GetValue());

    if(this->projCamDist > 3000.0)
    {
        this->projCamDist = 3000.0;
        this->txtPCDist->SetValue(wxString::Format("%.2f", this->projCamDist));
    }
    else if(this->projCamDist < 0.0)
    {
        this->projCamDist = 0.0;
        this->txtPCDist->SetValue(wxString::Format("%.2f", this->projCamDist));
    }
    
    this->sldrPCDist->SetValue((int)(this->projCamDist * 10));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrPlDistScroll(wxCommandEvent& event)
{
    this->planeDist = this->sldrPlDist->GetValue() / 10.0;
    this->txtPlDist->SetValue(wxString::Format("%.2f", this->planeDist));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnSldrFPitchScroll(wxCommandEvent& event)
{
    this->fringePitch = this->sldrFPitch->GetValue() / 10.0;
    this->txtFPitch->SetValue(wxString::Format("%.2f", this->fringePitch));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtPlDistEnter(wxCommandEvent& event)
{
    this->planeDist = atof(this->txtPlDist->GetValue());

    if(this->planeDist > 3000.0)
    {
        this->planeDist = 3000.0;
        this->txtPlDist->SetValue(wxString::Format("%.2f", this->planeDist));
    }
    else if(this->planeDist < 0.0)
    {
        this->planeDist = 0.0;
        this->txtPlDist->SetValue(wxString::Format("%.2f", this->planeDist));
    }
    
    this->sldrPlDist->SetValue((int)(this->planeDist * 10));
    
    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnTxtFPitchEnter(wxCommandEvent& event)
{

    this->fringePitch = atof(this->txtFPitch->GetValue());

    if(this->fringePitch > 100.0)
    {
        this->fringePitch = 100.0;
        this->txtFPitch->SetValue(wxString::Format("%.2f", this->fringePitch));
    }
    else if(this->fringePitch < 0.0)
    {
        this->fringePitch = 0.0;
        this->txtFPitch->SetValue(wxString::Format("%.2f", this->fringePitch));
    }
    
    this->sldrFPitch->SetValue((int)(this->fringePitch * 10));

    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnChxUseLUTClicked(wxCommandEvent& event)
{
    // If allowed, reconstruct the scene
    this->ReconstIfAllowed(this->chxActReconst->GetValue());
}

void RReconstWindow::OnMenuQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void RReconstWindow::OnMenuAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("3D reconstruction application.", "About", wxOK | wxICON_INFORMATION, this);
}

void RReconstWindow::OnMenuCalib(wxCommandEvent& WXUNUSED(event))
{
    RCalibWindow *calibWnd = new RCalibWindow(this, _("Calibration"), wxPoint(50, 50), wxSize(800, 600));
    calibWnd->Show(true);
    calibWnd->Initialize();
}

void RReconstWindow::OnMenuCapt(wxCommandEvent& WXUNUSED(event))
{
    RCaptWindow *captWnd = new RCaptWindow(this, _("Capture"), wxPoint(50, 50), wxSize(800, 600));
    captWnd->Show(true);
    captWnd->Initialize();
}

void RReconstWindow::OnMenuScene(wxCommandEvent& WXUNUSED(event))
{
    // Create child window
    if(this->GetChildren().IndexOf(this->sceneWnd) != -1)
        this->sceneWnd->Iconize(false);
    else
    {
        this->sceneWnd = new R3DSceneWindow(this, _("3D scene"), wxPoint(50, 50), wxSize(800, 600));
        this->sceneWnd->Show(true);
        this->sceneWnd->Initialize();
        this->sceneWnd->InitGLWindow(wxPoint(200, 0), wxSize(600, 600), 640, 480);
        this->sceneWnd->UpdateGLVertices(this->cmDepth);
        this->sceneWnd->UpdateGLColors(this->cmColors);
        this->sceneWnd->UpdateMask(this->cmMask);
        this->sceneWnd->RedrawScene();
    }
}

void RReconstWindow::OnMenuSavePly(wxCommandEvent& WXUNUSED(event))
{
    ExportCloudToPly((char*)"cloud.ply", this->cmMask, this->cmDepth, this->cmColors);
    ExportMeshToPly((char*)"mesh.ply", this->cmMask, this->cmDepth, this->cmColors);
}

void RReconstWindow::OnMenu3PhaseSelected(wxCommandEvent& event)
{
    this->phImagesPath = _("/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/capture");
    this->txtPhImgStorage->SetValue(this->phImagesPath);
    this->ReloadPhaseImages();
}

void RReconstWindow::OnMenu4PhaseSelected(wxCommandEvent& event)
{
    this->phImagesPath = _("/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/capture4");
    this->txtPhImgStorage->SetValue(this->phImagesPath);
    this->ReloadPhaseImages();
}

void RReconstWindow::OnMenuUWFFSelected(wxCommandEvent& event)
{
    this->Reconstruct3D(true);
}

void RReconstWindow::OnMenuUWSLSelected(wxCommandEvent& event)
{
    this->Reconstruct3D(true);
}