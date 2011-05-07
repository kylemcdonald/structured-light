/* 
 * File:   R3DView.h
 * Author: jenicek
 *
 * Created on July 26, 2010, 2:53 PM
 */

#ifndef _R3DSCENEWINDOW_H
#define	_R3DSCENEWINDOW_H

#include "Global.h"
#include "R3DScene.h"

class R3DSceneWindow : public wxFrame
{
private:
    R3DScene *glScene;

    // Panels
    wxPanel *panelSide;
    // Buttons
    wxButton *btnResetCam;
    // Check boxes
    wxCheckBox *chxShowZeroPix;
    wxCheckBox *chxShowCoords;
    wxCheckBox *chxTextured;
    // Text boxes
    wxTextCtrl *txtZeroCol;
    wxTextCtrl *txtNonZeroCol;
    // List boxes
    wxComboBox *cbxDrawModel;
    // Static texts
    wxStaticText *lblPixelCol;
    wxStaticText *lblZeroCol;
    wxStaticText *lblNonZeroCol;
    wxStaticText *lblDrawModel;

    enum
    {
        BTN_RES_CAM = 100,
        CHX_SHOW_ZERO,
        CHX_SHOW_COORD,
        CHX_TEXTURED,
        TXT_COL_NON_ZERO,
        TXT_COL_ZERO,
        CBX_DRAW_MODE,
    };

public:
    R3DSceneWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~R3DSceneWindow();
    bool Initialize(void);
    
    void InitGLWindow(const wxPoint& pos, const wxSize& size, int objWidth, int objHeight);
    void UpdateGLVertices(CvMat *depthMap);
    void UpdateGLColors(CvMat* colorMap);
    void UpdateGLVectors(CvMat *vectMap);
    void UpdateMask(CvMat* mask);
    void RedrawScene(void);

    // Event handlers
    void OnSceneWndPaint(wxCommandEvent& event);
    void OnBtnResCamClicked(wxCommandEvent& event);
    void OnChxShowZeroClicked(wxCommandEvent& event);
    void OnChxShowCoordClicked(wxCommandEvent& event);
    void OnTxtColNonZeroEnter(wxCommandEvent& event);
    void OnTxtColZeroEnter(wxCommandEvent& event);
    void OnChxShowTexturedClicked(wxCommandEvent& event);
    void OnCbxDrawModeChanged(wxCommandEvent& event);
};

#endif	/* _R3DSCENEWINDOW_H */

