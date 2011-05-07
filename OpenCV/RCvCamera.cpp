/* 
 * File:   RCvCamera.cpp
 * Author: jenicek
 * 
 * Created on July 28, 2010, 7:34 AM
 */

#include "RCvCamera.h"

RCvCamera::RCvCamera(int camID, wxFrame *parent, int wndID, const wxPoint& pos, const wxSize& camWndSize, const wxSize& camRes)
: wxGLCanvas(parent, wndID, pos, camWndSize, wxSUNKEN_BORDER, _("Camera window"))
{
    this->camResWidth = camRes.x;
    this->camResHeight = camRes.y;
    this->camWndWidth = camWndSize.x;
    this->camWndHeight = camWndSize.y;
    // Set frame delay in ms
    this->frameDelay = 4000;
    this->camID = 0;
    this->bRunCamera = false;

    // Capture
    this->capture = NULL;
    this->projWnd = NULL;

    // Set file paths
    this->fileSavePath = _("capture/");
    this->projPatPath = _("pattern/horizontal");

    // Set drawing timer
    this->drawTimer = new wxTimer(this, TIMER_ID);
    this->Connect(TIMER_ID, wxEVT_TIMER, (wxObjectEventFunction)&RCvCamera::OnTimerTick);
}

RCvCamera::~RCvCamera()
{
    if(this->capture)
        cvReleaseCapture(&(this->capture));
}

bool RCvCamera::RunCamera(void)
{
    // Show camera images
    // Start capturing from camera 
    this->capture = cvCreateCameraCapture(this->camID);
    // Check the capture
    if(capture == NULL)
    {
        fprintf(stderr, "Wrong camera ID!\n");
        return false;
    }
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, this->camResWidth);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, this->camResHeight);
    if(cvQueryFrame(capture) == NULL)
    {
        fprintf(stderr, "No frame available from the camera!\n");
        return false;
    }

    // Run the camera
    this->bRunCamera = true;

    // Set timer
    this->drawTimer->Start(200);

    return true;
}

bool RCvCamera::DrawFrame(void)
{
    // Get new frame and convert it to wxBitmap
    this->CvtIplToBmp(this->CamGetFrame(), &this->bmpCamImage, this->camResWidth, this->camResHeight,
            this->camWndWidth, this->camWndHeight);

    // Get DC
    wxPaintDC dc(this);

    // Create DC and check it's availablity
    if(!dc.Ok() || !this->bRunCamera)
        return false;

    //dc.BeginDrawing();
    int x, y, w, h;
    // Get DC clipping box
    dc.GetClippingBox(&x, &y, &w, &h);
    dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
    dc.SetTextForeground(*wxWHITE);
    // Draw a frame
    dc.DrawBitmap(this->bmpCamImage, x, y);
    dc.DrawText(_("Camera view"), 5, 5);
    //dc.EndDrawing();

    return true;
}

bool RCvCamera::CvtIplToBmp(IplImage* frame, wxBitmap *bmp, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
    if(frame == NULL)
        return false;

    IplImage *dstImg = cvCloneImage(frame);
    // Convert from IplImage to wxBitmap
    unsigned char *rawData;
    CvSize roiSize;
    int step = 0;
    // Get raw data from IplImage
    cvGetRawData(dstImg, &rawData, &step, &roiSize);
    // Convert raw data to wxImg
    wxImage wxImg = wxImage(srcWidth, srcHeight, rawData, true);
    // Convert wxImg to wxBitmap
    (*bmp) = wxBitmap(wxImg.Scale(dstWidth, dstHeight));

    cvReleaseImage(&dstImg);

    return true;
}

bool RCvCamera::CapturePhases(int camID, int numPhases, int frameDelay, int projResWidth, int projResHeight, bool bRefImgs)
{
    this->camID = camID;
    this->frameDelay = frameDelay;

    // Create fullscreen window (for controlling projector display)
    this->projWnd = new RImageWindow(NULL ,wxPoint(10, 10), wxSize(projResWidth, projResHeight), _("Projector window"), this->projPatPath);
    projWnd->Show();
    projWnd->Initialize();

    // Get the 3 phase images
    IplImage* phPattern = NULL;
    char fNamePrep[256] = "\0";
    char fNamePat[256] = "\0";
   
    // Show info that program's waiting for user to continue
    if(bRefImgs)
        wxMessageBox("Prepare the scene for reference plane capture(remove the object).", "Waiting", wxOK | wxICON_INFORMATION);
    else
        wxMessageBox("Set the scene up and then continue pressing Ok button.", "Waiting", wxOK | wxICON_INFORMATION);

    /*******Capture reference images********/
    if(bRefImgs)
    {
        // Skip few camera frames
        wxThread::Sleep(this->frameDelay);
        //cvWaitKey(this->frameDelay);
        cvQueryFrame(capture);
        cvQueryFrame(capture);
        cvQueryFrame(capture);
        for(int i = 0; i < numPhases; i++)
        {
            // Load projector pattern and show it in projector window
            sprintf(fNamePrep, "/i%d.png", i + 1);
            projWnd->ShowPattern(fNamePrep);
            projWnd->Update();
            // Make another frame capture safe one
            wxThread::Sleep(this->frameDelay);
            //cvWaitKey(this->frameDelay);
            cvQueryFrame(capture);
            cvQueryFrame(capture);
            cvQueryFrame(capture);
            cvQueryFrame(capture);
            // Capture camera frame and save it to file
            IplImage* camFrame = cvQueryFrame(capture);
            sprintf(fNamePrep, "/ref%d.jpg", i + 1);
            strcpy(fNamePat, this->fileSavePath.ToAscii());
            strcat(fNamePat, fNamePrep);
            cvSaveImage(fNamePat, camFrame, NULL);
        }
        wxMessageBox("Reference plane captured. Now place object and then continue.", "Waiting", wxOK | wxICON_INFORMATION);
    }
    /***************************************/
            
    // Skip few camera frames
    wxThread::Sleep(this->frameDelay);
    //cvWaitKey(this->frameDelay);
    cvQueryFrame(capture);
    cvQueryFrame(capture);
    cvQueryFrame(capture);
    for(int i = 0; i < numPhases; i++)
    {
        // Load projector pattern and show it in projector window
        sprintf(fNamePrep, "/i%d.png", i + 1);
        projWnd->ShowPattern(fNamePrep);
        projWnd->Update();
        // Make another frame capture safe one
        wxThread::Sleep(this->frameDelay);
        //cvWaitKey(this->frameDelay);
        cvQueryFrame(capture);
        cvQueryFrame(capture);
        cvQueryFrame(capture);
        cvQueryFrame(capture);
        // Capture camera frame and save it to file
        IplImage* camFrame = cvQueryFrame(capture);
        sprintf(fNamePrep, "/phase%d.jpg", i + 1);
        strcpy(fNamePat, this->fileSavePath.ToAscii());
        strcat(fNamePat, fNamePrep);
        cvSaveImage(fNamePat, camFrame, NULL);
    }

    // Notify that capture's done
    wxMessageBox("Capture's done.", "Done", wxOK | wxICON_INFORMATION);

    // Destroy projector window
    this->projWnd->Destroy();
}

void RCvCamera::OnCamWndPaint(wxPaintEvent& event)
{
    if(this->bRunCamera)
        this->DrawFrame();
}

void RCvCamera::OnTimerTick(wxTimerEvent& event)
{
    if(this->bRunCamera)
    {
        this->DrawFrame();
    }
}


