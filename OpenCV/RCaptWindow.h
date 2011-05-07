/* 
 * File:   RCaptWindow.h
 * Author: jenicek
 *
 * Created on July 28, 2010, 7:14 AM
 */

#ifndef _RCAPTWINDOW_H
#define	_RCAPTWINDOW_H

#include "Global.h"
#include "RCvCamera.h"

class RCaptWindow : public wxFrame
{
private:
    // Number of phase images to capture
    int numPhImgs;

    RCvCamera *cvCamera;

    // Buttons
    wxButton *btnRunCam;
    wxButton *btnCaptCam;
    wxButton *btnSavePath;
    wxButton *btnPatPath;

    // Text boxes
    wxTextCtrl *txtSavePath;
    wxTextCtrl *txtPatPath;
    wxTextCtrl *txtFrameDelay;
    wxTextCtrl *txtCameraID;

    // Combo boxes
    wxComboBox *cbxProjRes;
    wxComboBox *cbxCamRes;
    wxComboBox *cbxShiftMet;

    // Check boxes
    wxCheckBox *chxRefImgs;

    // Labels
    wxStaticText *lblSavePath;
    wxStaticText *lblPatPath;
    wxStaticText *lblFrameDelay;
    wxStaticText *lblCameraID;
    wxStaticText *lblMilisec;
    wxStaticText *lblDevVideo;
    wxStaticText *lblProjRes;
    wxStaticText *lblCamRes;
    wxStaticText *lblShiftMet;

    // Private methods
    void DrawPhaseFrames(void);

    enum
    {
        CANVAS_CAM = 200,
        BTN_RUN_CAM,
        TXT_SAVE_PATH,
        TXT_PAT_PATH,
        BTN_SAVE_PATH,
        BTN_PAT_PATH,
        BTN_CAPT_CAM,
        TXT_FRAME_DELAY,
        TXT_CAM_ID,
        CBX_PROJ_RES,
        CBX_CAM_RES,
        CBX_SHIFT_MET,
        CHX_REF_IMGS,
    };
    
public:
    RCaptWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~RCaptWindow();
    
    bool Initialize(void);

    // Event handlers
    void OnCloseWindow(wxCloseEvent& event);
    void OnBtnRunCamClicked(wxCommandEvent& event);
    void OnBtnCaptCamClicked(wxCommandEvent& event);
    void OnPaintWindow(wxPaintEvent& event);
    void OnTxtSavePathEnter(wxCommandEvent& event);
    void OnTxtPatPathEnter(wxCommandEvent& event);
    void OnBtnSavePathClicked(wxCommandEvent& event);
    void OnBtnPatPathClicked(wxCommandEvent& event);
    void OnTxtFrameDelayEnter(wxCommandEvent& event);
    void OnTxtCamIDEnter(wxCommandEvent& event);
    void OnCbxShiftMetSelected(wxCommandEvent& event);
    void OnChxRefImgsClicked(wxCommandEvent& event);
    void OnCbxCamResSelected(wxCommandEvent& event);
};

#endif	/* _RCAPTWINDOW_H */

