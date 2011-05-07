/* 
 * File:   RReconstWindow.h
 * Author: jenicek
 *
 * Created on July 28, 2010, 7:08 AM
 */

#ifndef _RRECONSTWINDOW_H
#define	_RRECONSTWINDOW_H

#include "Global.h"
#include "RScanUtil.h"
#include "R3DSceneWindow.h"
#include "RCaptWindow.h"
#include "RCalibWindow.h"
#include "RExporter.h"

class RReconstWindow : public wxFrame
{
    #define LUT_SIZE 16777216   // 24-bit depth camera

private:

    typedef enum
    {
        IMG_VIEW_1 = 0,
        IMG_VIEW_2,
    }T_ViewID;

    // Boolean values
    bool bAllowRepaint;

    // 3D scene window
    R3DSceneWindow *sceneWnd;

    int imgWidth;
    int imgHeight;
    float cutoffThresh;
    // Matrices
    CvMat *cmWPhase;
    CvMat *cmUPhase;
    CvMat *cmMask;
    CvMat *cmProcess;
    CvMat *cmIRatio;
    CvMat *cmColors;
    CvMat *cmDepth;
    CvMat *cmImg3ChInt;
    // Phase images
    IplImage **phaseImages;
    // Start of phase unwrapping
    CvPoint phaseUnwStart;
    // Storage paths
    wxString phImagesPath;
    // Depth creation parameters
    float projCamAngle;
    float projCamDist;
    float planeDist;
    float fringePitch;
    // Error compensation lookup table
    double *errLut;
    // Number of phase images
    int numPhImages;

    // Menus
    wxMenu *menuFile;
    // Submenus
    wxMenu *submWrap;
    wxMenu *submUnwrap;

    // Panels
    wxPanel *panelPhSet;
    wxPanel *panelDepthSet;
    wxPanel *panelDepthM1;
    wxPanel *panelDepthM2;
    wxPanel *panelDepthM3;

    // Buttons
    wxButton *btnFullSize1;
    wxButton *btnFullSize2;
    wxButton *btnReconst;
    wxButton *btnPhImgStorage;

    // Text boxes
    wxTextCtrl *txtPhImgStorage;
    wxTextCtrl *txtCutoffThresh;
    wxTextCtrl *txtZScale;
    wxTextCtrl *txtZSKew;
    wxTextCtrl *txtPCAngle;
    wxTextCtrl *txtPCDist;
    wxTextCtrl *txtPlDist;
    wxTextCtrl *txtFPitch;

    // Check boxes
    wxCheckBox *chxActReconst;
    wxCheckBox *chxReconstPhase;
    wxCheckBox *chxReconstDepth;
    wxCheckBox *chxUseLUT;

    // Combo boxes
    wxComboBox *cbxChooseView1;
    wxComboBox *cbxChooseView2;
    wxComboBox *cbxDepthMethod;
    wxComboBox *cbxPatternType;

    // Sliders
    wxSlider *sldrCutoffThresh;
    wxSlider *sldrZScale;
    wxSlider *sldrZSkew;
    wxSlider *sldrPCAngle;
    wxSlider *sldrPCDist;
    wxSlider *sldrPlDist;
    wxSlider *sldrFPitch;

    // Labels
    wxStaticText *lblPhaseSet;
    wxStaticText *lblDepthSet;
    wxStaticText *lblPhImgStorage;
    wxStaticText *lblCutoffThresh;
    wxStaticText *lblDepthMet;
    wxStaticText *lblPatType;
    wxStaticText *lblZScale;
    wxStaticText *lblZSkew;
    wxStaticText *lblPCAngle;
    wxStaticText *lblPCDist;
    wxStaticText *lblPlDist;
    wxStaticText *lblFPitch;

    // Private methods
    void DrawReconstImage(T_ViewID id, int imageIndex);
    void DrawRecImgFullsize(int imageIndex);
    void RedrawReconstWnds(void);
    void ReconstIfAllowed(bool allowed);
    bool ReloadPhaseImages(void);
    // Menu IDs
    enum
    {
        ID_MENU_QUIT = 1,
        ID_MENU_ABOUT,
        ID_MENU_CALIB,
        ID_MENU_CAPT,
        ID_MENU_3D,
        ID_MENU_SAVE_PLY,
        ID_MENU_RADIO_3PHASE,
        ID_MENU_RADIO_4PHASE,
        ID_MENU_RADIO_UWFLOODFILL,
        ID_MENU_RADIO_UWSCANLINE,
    };
    // Control IDs
    enum
    {
        CBX_CHOOSE_1 = 300,
        CBX_CHOOSE_2,
        BTN_FULL_SIZE_1,
        BTN_FULL_SIZE_2,
        BTN_RECONST,
        TXT_PHASE_STORAGE,
        BTN_PHASE_STORAGE,
        TXT_CUTOFF_TH,
        SLDR_CUTOFF_TH,
        CBX_DEPTH_MET,
        CBX_PAT_TYPE,
        SLDR_ZSCALE,
        SLDR_ZSKEW,
        TXT_ZSCALE,
        TXT_ZSKEW,
        SLDR_PCANGLE,
        SLDR_PCDIST,
        SLDR_PLDIST,
        SLDR_FPITCH,
        TXT_PCANGLE,
        TXT_PCDIST,
        TXT_PLDIST,
        TXT_FPITCH,
        CHX_USE_LUT,
    };

public:
    RReconstWindow(wxFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~RReconstWindow();

    // Class methods
    bool Initialize(char *phImagesPath, float cutoffThresh, float pcAngle, float pcDist, float plDist, float fPitch);
    bool Reconstruct3D(bool allowed);
    bool ReconstPhase(bool allowed);
    bool ReconstDepth(bool allowed);

    // Setters
    void SetCutoffThresh(float thresh) { this->cutoffThresh = thresh; }
    void SetProjCamAngle(float angle) { this->projCamAngle = angle; }
    void SetProjCamDist(float dist) { this->projCamDist = dist; }
    void SetPlaneDist(float dist) { this->planeDist = dist; }
    void SetFringePitch(float pitch) { this->fringePitch = pitch; }

    // Getters
    int GetImgWidth(void) { return this->imgWidth; }
    int GetImgHeight(void) { return this->imgHeight; }
    float GetCutoffThresh(void) { return this->cutoffThresh; }
    float GetProjCamAngle(void) { return this->projCamAngle; }
    float GetProjCamDist(void) { return this->projCamDist; }
    float GetPlaneDist(void) { return this->planeDist; }
    float GetFringePitch(void) { return this->fringePitch; }
    wxString GetPhImagesPath(void) { return this->phImagesPath; }

    // Event handlers
    void OnReconstWndPaint(wxPaintEvent& event);
    void OnCbxChooseView1Selected(wxCommandEvent& event);
    void OnCbxChooseView2Selected(wxCommandEvent& event);
    void OnBtnFullSize1Clicked(wxCommandEvent& event);
    void OnBtnFullSize2Clicked(wxCommandEvent& event);
    void OnBtnReconstClicked(wxCommandEvent& event);
    void OnTxtPhImgStorageEnter(wxCommandEvent& event);
    void OnBtnPhImgStorageClick(wxCommandEvent& event);
    void OnTxtCutoffThEnter(wxCommandEvent& event);
    void OnSldrCutoffThScroll(wxCommandEvent& event);
    void OnCbxDepthMetSelected(wxCommandEvent& event);
    void OnCbxPatTypeSelected(wxCommandEvent& event);
    void OnSldrZScaleScroll(wxCommandEvent& event);
    void OnSldrZSkewScroll(wxCommandEvent& event);
    void OnTxtZScaleEnter(wxCommandEvent& event);
    void OnTxtZSkewEnter(wxCommandEvent& event);
    void OnSldrPCAngleScroll(wxCommandEvent& event);
    void OnSldrPCDistScroll(wxCommandEvent& event);
    void OnSldrPlDistScroll(wxCommandEvent& event);
    void OnSldrFPitchScroll(wxCommandEvent& event);
    void OnTxtPCAngleEnter(wxCommandEvent& event);
    void OnTxtPCDistEnter(wxCommandEvent& event);
    void OnTxtPlDistEnter(wxCommandEvent& event);
    void OnTxtFPitchEnter(wxCommandEvent& event);
    void OnChxUseLUTClicked(wxCommandEvent& event);
    // Menu event handlers
    void OnMenuQuit(wxCommandEvent& event);
    void OnMenuAbout(wxCommandEvent& event);
    void OnMenuCalib(wxCommandEvent& event);
    void OnMenuCapt(wxCommandEvent& event);
    void OnMenuScene(wxCommandEvent& event);
    void OnMenuSavePly(wxCommandEvent& event);
    void OnMenu3PhaseSelected(wxCommandEvent& event);
    void OnMenu4PhaseSelected(wxCommandEvent& event);
    void OnMenuUWFFSelected(wxCommandEvent& event);
    void OnMenuUWSLSelected(wxCommandEvent& event);
};

#endif	/* _RRECONSTWINDOW_H */

