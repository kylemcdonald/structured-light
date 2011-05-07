/* 
 * File:   RImageWindow.cpp
 * Author: jenicek
 * 
 * Created on July 29, 2010, 7:36 AM
 */

#include "RImageWindow.h"

RImageWindow::RImageWindow(wxFrame *parent, const wxPoint& pt, const wxSize& size, const wxString& title, const wxString& patternPath)
: wxFrame(parent, wxID_ANY, title, pt, size)
{
    this->bmpImage = wxBitmap();
    wxInitAllImageHandlers();
    this->patternPath = patternPath;
    this->labelText = wxString();
    this->bDrawLabel = false;
}

RImageWindow::~RImageWindow()
{
}

bool RImageWindow::Initialize(void)
{
    // Register events
    this->Connect(-1, wxEVT_PAINT, (wxObjectEventFunction)&RImageWindow::OnImageWndPaint);
    
    // Show first pattern, if there's any
    if(this->patternPath.CompareTo(""))
        this->ShowPattern((char*)"/i1.png");

    return true;
}

void RImageWindow::OnImageWndPaint(wxPaintEvent& event)
{
    // Get DC and draw the bitmap
    wxPaintDC dc(this);
    dc.DrawBitmap(this->bmpImage, 0, 0);

    if(this->bDrawLabel)
    {
        // Set text font and then draw label also
        dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
        dc.SetTextForeground(*wxWHITE);
        dc.DrawText(this->labelText, wxPoint(10, 10));
    }
}

void RImageWindow::ShowPattern(char* fileName)
{
    // Load proper image
    wxImage img = wxImage();
    char patPath[256] = "\0";
    sprintf(patPath, "%s%s", this->patternPath.ToAscii(), fileName);
    img.LoadFile(patPath);
    // Scale this image to fit the window
    this->bmpImage = wxBitmap(img.Scale(this->m_width, this->m_height));
    // Get DC and draw the bitmap
    wxPaintDC dc(this);
    dc.DrawBitmap(this->bmpImage, 0, 0);

    if(this->bDrawLabel)
    {
        // Set text font and then draw label also
        dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
        dc.SetTextForeground(*wxWHITE);
        dc.DrawText(this->labelText, wxPoint(10, 10));
    }

    dc.Destroy();
}

void RImageWindow::ShowPattern(IplImage *image)
{
    // Load proper image
    wxImage img = wxImage();

    // Convert the IplImage into wxImage by temporarily saving it to file
    cvSaveImage("tmp/temp.png", image);
    img.LoadFile(_("tmp/temp.png"));
    // Scale this image to fit the window
    this->bmpImage = wxBitmap(img.Scale(this->m_width, this->m_height));
    // Get DC and draw the bitmap
    wxPaintDC dc(this);
    dc.DrawBitmap(this->bmpImage, 0, 0);

    if(this->bDrawLabel)
    {
        // Set text font and then draw label also
        dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
        dc.SetTextForeground(*wxWHITE);
        dc.DrawText(this->labelText, wxPoint(10, 10));
    }
    
    dc.Destroy();
}

