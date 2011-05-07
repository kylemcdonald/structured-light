/* 
 * File:   RCvCamera.h
 * Author: jenicek
 *
 * Created on July 28, 2010, 7:34 AM
 */

#ifndef _RCVCAMERA_H
#define	_RCVCAMERA_H

#define TIMER_ID 32000

#include "Global.h"
#include "RImageWindow.h"

class RCvCamera : public wxGLCanvas
{
private:
    int camResWidth;
    int camResHeight;
    int frameDelay;
    int camID;

    int camWndWidth;
    int camWndHeight;

    bool bRunCamera;

    wxString fileSavePath;
    wxString projPatPath;

    CvCapture* capture;
    wxBitmap bmpCamImage;

    wxTimer *drawTimer;
    
    RImageWindow *projWnd;
    
public:
    RCvCamera(int camID, wxFrame *parent, int wndID, const wxPoint& pos, const wxSize& camWndSize, const wxSize& camRes);
    virtual ~RCvCamera();

    // Class methods
    bool CapturePhases(int camID, int numPhases, int frameDelay, int projResWidth, int projResHeight, bool bRefImgs);
    bool RunCamera(void);
    void StopCamera(void ) { this->bRunCamera = false; this->drawTimer->Stop(); cvReleaseCapture(&(this->capture)); }
    bool DrawFrame(void);
    bool CvtIplToBmp(IplImage* frame, wxBitmap *bmp, int srcWidth, int srcHeight, int dstWidth, int dstHeight);
    IplImage* CamGetFrame(void) { if(this->capture) return cvQueryFrame(this->capture); else return NULL; }


    // Setters
    void SetCamID(int id) { this->camID = id; }
    void SetFrameDelay(int delay) { this->frameDelay = delay; }
    void SetCamResWidth(int w) { this->camResWidth = w; }
    void SetCamResHeight(int h) { this->camResHeight = h; }
    void SetFileSavePath(const wxString& filePath) { this->fileSavePath = filePath; }
    void SetProjPatPath(const wxString& filePath) { this->projPatPath = filePath; }
    void SetCamWndWidth(int w) { this->camWndWidth = w; }
    void SetCamWndHeight(int h) { this->camWndHeight = h; }

    // Getters
    int GetCamResWidth(void) { return this->camResWidth; }
    int GetCamResHeight(void) { return this->camResHeight; }
    int GetCamID(void) { return this->camID; }
    int GetFrameDelay(void) { return this->frameDelay; }
    bool GetRunCamera(void) { return this->bRunCamera; }
    wxString GetFileSavePath(void) { return this->fileSavePath; }
    wxString GetProjPatPath(void) { return this->projPatPath; }
    int GetCamWndWidth(void) { return this->camWndWidth; }
    int GetCamWndHeight(void) { return this->camWndHeight; }
    CvCapture* GetCamCapture(void) { return this->capture; }

    // Event handlers
    void OnCamWndPaint(wxPaintEvent& event);
    void OnTimerTick(wxTimerEvent& event);
};

#endif	/* _RCVCAMERA_H */

