/* 
 * File:   RCalibWindow.h
 * Author: jenicek
 *
 * Created on August 19, 2010, 8:14 AM
 */

#ifndef _RCALIBWINDOW_H
#define	_RCALIBWINDOW_H

#include "Global.h"
#include "RCvCamera.h"
#include "RCalibUtil.h"

class RCalibWindow : public wxFrame
{
private:
    T_CalibInput calInput;
    T_CalibOutput calOutput;

    RCvCamera *cvCamera;
    
    wxString *camImgStorage;
    wxString *projImgStorage;

    // Buttons
    wxButton *btnCamImgStorage;
    wxButton *btnProjImgStorage;
    wxButton *btnRunCam;
    wxButton *btnCalibCam;
    wxButton *btnCalibProjCam;
    wxButton *btnCompGeometry;
    
    // Text boxes
    wxTextCtrl *txtCamImgStorage;
    wxTextCtrl *txtProjImgStorage;
    wxTextCtrl *txtCamGain;
    wxTextCtrl *txtProjGain;

    // Combo boxes
    wxComboBox *cbxCamRes;
    wxComboBox *cbxProjRes;

    // Spin controls
    wxSpinCtrl *spinCamBoardWidth;
    wxSpinCtrl *spinCamBoardHeight;
    wxSpinCtrl *spinCamBoardSqMmW;
    wxSpinCtrl *spinCamBoardSqMmH;
    wxSpinCtrl *spinProjBoardWidth;
    wxSpinCtrl *spinProjBoardHeight;
    wxSpinCtrl *spinProjBoardSqPxW;
    wxSpinCtrl *spinProjBoardSqPxH;
    wxSpinCtrl *spinNumFrames;

    // Sliders
    wxSlider *sldrCamGain;
    wxSlider *sldrProjGain;

    // Panels
    wxPanel *panelCamSet;
    wxPanel *panelProjSet;
    
    // Labels
    wxStaticText *lblCamImgStorage;
    wxStaticText *lblProjImgStorage;
    wxStaticText *lblCamGain;
    wxStaticText *lblProjGain;
    wxStaticText *lblCamRes;
    wxStaticText *lblProjRes;
    wxStaticText *lblCamBoard;
    wxStaticText *lblCamBoardWidth;
    wxStaticText *lblCamBoardHeight;
    wxStaticText *lblCamBoardSq;
    wxStaticText *lblCamBoardSqMmW;
    wxStaticText *lblCamBoardSqMmH;
    wxStaticText *lblProjBoard;
    wxStaticText *lblProjBoardWidth;
    wxStaticText *lblProjBoardHeight;
    wxStaticText *lblProjBoardSq;
    wxStaticText *lblProjBoardSqPxW;
    wxStaticText *lblProjBoardSqPxH;
    wxStaticText *lblCamSet;
    wxStaticText *lblProjSet;
    wxStaticText *lblNumFrames;

    enum
    {
        CANVAS_CAM,
        BTN_CAM_STORAGE,
        BTN_PROJ_STORAGE,
        TXT_CAM_STORAGE,
        TXT_PROJ_STORAGE,
        SLDR_CAM_GAIN,
        SLDR_PROJ_GAIN,
        TXT_CAM_GAIN,
        TXT_PROJ_GAIN,
        BTN_RUN_CAM,
        BTN_CALIB_CAM,
        BTN_CALIB_PROJCAM,
        BTN_COMP_GEOMETRY,
        CBX_CAM_RES,
        CBX_PROJ_RES,
        SPIN_CAM_B_W,
        SPIN_CAM_B_H,
        SPIN_CAM_SQMM_W,
        SPIN_CAM_SQMM_H,
        SPIN_PROJ_B_W,
        SPIN_PROJ_B_H,
        SPIN_PROJ_SQPX_W,
        SPIN_PROJ_SQPX_H,
        SPIN_NUM_FRAMES,
    };

public:
    RCalibWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~RCalibWindow();

    bool Initialize(void);
    void ResetCalibOutput(void);

    // Event handlers
    void OnCloseWindow(wxCommandEvent &event);
    void OnPaintWindow(wxPaintEvent &event);
    void OnBtnCamImgStorageClicked(wxCommandEvent &event);
    void OnBtnProjImgStorageClicked(wxCommandEvent &event);
    void OnTxtCamImgStorageEnter(wxCommandEvent &event);
    void OnTxtProjImgStorageEnter(wxCommandEvent &event);
    void OnSldrCamGainScroll(wxCommandEvent &event);
    void OnSldrProjGainScroll(wxCommandEvent &event);
    void OnTxtCamGainEnter(wxCommandEvent &event);
    void OnTxtProjGainEnter(wxCommandEvent &event);
    void OnBtnRunCamClicked(wxCommandEvent &event);
    void OnBtnCompGeometryClicked(wxCommandEvent &event);
    void OnBtnCalibCamClicked(wxCommandEvent &event);
    void OnBtnCalibProjCamClicked(wxCommandEvent &event);
    void OnCbxCamResSelected(wxCommandEvent &event);
    void OnCbxProjResSelected(wxCommandEvent &event);
    void OnSpinCamBoardWidthUpdated(wxCommandEvent &event);
    void OnSpinCamBoardHeightUpdated(wxCommandEvent &event);
    void OnSpinCamBoardSqMmWUpdated(wxCommandEvent &event);
    void OnSpinCamBoardSqMmHUpdated(wxCommandEvent &event);
    void OnSpinProjBoardWidthUpdated(wxCommandEvent &event);
    void OnSpinProjBoardHeightUpdated(wxCommandEvent &event);
    void OnSpinProjBoardSqPxWUpdated(wxCommandEvent &event);
    void OnSpinProjBoardSqPxHUpdated(wxCommandEvent &event);
    void OnSpinNumFramesUpdated(wxCommandEvent &event);
};

#endif	/* _RCALIBWINDOW_H */

