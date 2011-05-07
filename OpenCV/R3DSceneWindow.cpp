/* 
 * File:   R3DSceneWindow.cpp
 * Author: jenicek
 * 
 * Created on July 26, 2010, 2:53 PM
 */

#include "R3DSceneWindow.h"

R3DSceneWindow::R3DSceneWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(parent, -1, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    this->Connect(-1, wxEVT_PAINT, (wxObjectEventFunction)&R3DSceneWindow::OnSceneWndPaint);
}

R3DSceneWindow::~R3DSceneWindow()
{
}

bool R3DSceneWindow::Initialize(void)
{
    // Create side panel
    this->panelSide = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(200, 600));

    // Zero pixels and non zero pixels color text boxes and labels
    this->lblPixelCol = new wxStaticText(this->panelSide, wxID_ANY, _("Pixel colors:"), wxPoint(15, this->panelSide->m_y + 10));
    this->lblNonZeroCol = new wxStaticText(this->panelSide, wxID_ANY, _("Non-Zero:"), wxPoint(25, this->lblPixelCol->m_y + 28));
    this->txtNonZeroCol = new wxTextCtrl(this->panelSide, TXT_COL_NON_ZERO, _(""), wxPoint(95, this->lblNonZeroCol->m_y - 5), wxDefaultSize, wxTE_PROCESS_ENTER);
    this->txtNonZeroCol->SetMaxLength(7);
    this->lblZeroCol = new wxStaticText(this->panelSide, wxID_ANY, _("Zero:"), wxPoint(25, this->lblNonZeroCol->m_y + 28));
    this->txtZeroCol = new wxTextCtrl(this->panelSide, TXT_COL_ZERO, _(""), wxPoint(95, this->lblZeroCol->m_y - 5), wxDefaultSize, wxTE_PROCESS_ENTER);
    this->txtZeroCol->SetMaxLength(7);
    // Show zero pixels and show coords checkboxes
    this->chxShowZeroPix = new wxCheckBox(this->panelSide, CHX_SHOW_ZERO, _("Show zero pixels"), wxPoint(25, this->txtZeroCol->m_y + 40));
    this->chxShowCoords = new wxCheckBox(this->panelSide, CHX_SHOW_COORD, _("Show coordinates"), wxPoint(25, this->chxShowZeroPix->m_y + 30));
    this->chxTextured = new wxCheckBox(this->panelSide, CHX_TEXTURED, _("Show texture"), wxPoint(25, this->chxShowCoords->m_y + 30));
    // Draw model combo box
    this->lblDrawModel = new wxStaticText(this->panelSide, wxID_ANY, _("Object render mode"), wxPoint(15, this->chxTextured->m_y + 40));
    const wxString cbxDMVals[3] = {_("Point Cloud"), _("Wireframe"), _("Mesh")};
    this->cbxDrawModel = new wxComboBox(this->panelSide, CBX_DRAW_MODE, cbxDMVals[0], wxPoint(30, this->lblDrawModel->m_y + 25),
            wxSize(130, 25), 3, cbxDMVals);
    this->cbxDrawModel->SetSelection(0);

    // Reset camera button
    this->btnResetCam = new wxButton(this->panelSide, BTN_RES_CAM, _("Reset camera"), wxPoint(50, this->cbxDrawModel->m_y + 40));

    // Connect these controls with events
    this->Connect(BTN_RES_CAM, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&R3DSceneWindow::OnBtnResCamClicked);
    this->Connect(CHX_SHOW_ZERO, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&R3DSceneWindow::OnChxShowZeroClicked);
    this->Connect(CHX_SHOW_COORD, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&R3DSceneWindow::OnChxShowCoordClicked);
    this->Connect(TXT_COL_NON_ZERO, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&R3DSceneWindow::OnTxtColNonZeroEnter);
    this->Connect(TXT_COL_ZERO, wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&R3DSceneWindow::OnTxtColZeroEnter);
    this->Connect(CHX_TEXTURED, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&R3DSceneWindow::OnChxShowTexturedClicked);
    this->Connect(CBX_DRAW_MODE, wxEVT_COMMAND_COMBOBOX_SELECTED, (wxObjectEventFunction)&R3DSceneWindow::OnCbxDrawModeChanged);
}

void R3DSceneWindow::InitGLWindow(const wxPoint& pos, const wxSize& size, int objWidth, int objHeight)
{
    this->glScene = new R3DScene(this, wxPoint(200, 0), wxSize(600, 600), _("Scene"));
    // Set check boxes default values
    this->chxShowCoords->SetValue(this->glScene->GetShowCoords());
    this->chxShowZeroPix->SetValue(this->glScene->GetShowZeroPix());
    this->chxTextured->SetValue(this->glScene->GetTextured());
    // Set text boxes default values
    this->txtNonZeroCol->SetValue(this->glScene->GetColNonZeroStr());
    this->txtZeroCol->SetValue(this->glScene->GetColZeroStr());

    this->glScene->LoadVertices((char*)"/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/points.csv", 640, 480);
    this->glScene->LoadColors((char*)"/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/colors.csv", 640, 480);
    this->glScene->LoadMask((char*)"/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/mask.csv", 640, 480);
    this->glScene->RenderGLWindow();
}

void R3DSceneWindow::UpdateGLVertices(CvMat* depthMap)
{
    this->glScene->LoadVertices(depthMap);
}

void R3DSceneWindow::UpdateGLVectors(CvMat* vectMap)
{
    this->glScene->LoadVectors(vectMap);
}

void R3DSceneWindow::UpdateGLColors(CvMat* colorMap)
{
    this->glScene->LoadColors(colorMap);
}

void R3DSceneWindow::UpdateMask(CvMat* mask)
{
    this->glScene->LoadMask(mask);
}

void R3DSceneWindow::RedrawScene(void)
{
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnSceneWndPaint(wxCommandEvent& event)
{
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnBtnResCamClicked(wxCommandEvent& event)
{
    this->glScene->ResetCamera();
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnChxShowCoordClicked(wxCommandEvent& event)
{
    this->glScene->SetShowCoords(event.IsChecked());
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnChxShowZeroClicked(wxCommandEvent& event)
{
    this->glScene->SetShowZeroPix(event.IsChecked());
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnChxShowTexturedClicked(wxCommandEvent& event)
{
    this->glScene->SetTextured(event.IsChecked());
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnTxtColNonZeroEnter(wxCommandEvent& event)
{
    if(!this->glScene->SetColNonZeroStr(this->txtNonZeroCol->GetValue()))
        this->txtNonZeroCol->SetBackgroundColour(wxColour(255, 0, 0));
    else
        this->txtNonZeroCol->SetBackgroundColour(wxColour(255, 255, 255));
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnTxtColZeroEnter(wxCommandEvent& event)
{
    if(!this->glScene->SetColZeroStr(this->txtZeroCol->GetValue()))
        this->txtZeroCol->SetBackgroundColour(wxColour(255, 0, 0));
    else
        this->txtZeroCol->SetBackgroundColour(wxColour(255, 255, 255));
    this->glScene->RedrawGLWindow();
}

void R3DSceneWindow::OnCbxDrawModeChanged(wxCommandEvent& event)
{
    this->glScene->SetDrawMode(this->cbxDrawModel->GetCurrentSelection());
    this->glScene->RedrawGLWindow();
}