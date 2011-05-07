/* 
 * File:   RProjWindow.h
 * Author: jenicek
 *
 * Created on July 29, 2010, 7:36 AM
 */

#ifndef _RPROJWINDOW_H
#define	_RPROJWINDOW_H

#include "Global.h"

class RImageWindow : public wxFrame
{
private:
    wxString patternPath;
    wxBitmap bmpImage;
    wxString labelText;
    bool bDrawLabel;

public:
    RImageWindow(wxFrame *parent, const wxPoint& pt, const wxSize& size, const wxString& title, const wxString& patternPath);
    virtual ~RImageWindow();

    // Class methods
    bool Initialize(void);
    void ShowPattern(char *fileName);
    void ShowPattern(IplImage *image);
    
    // Setters
    void SetLabelText(const wxString& text) { this->labelText = text; }
    void SetDrawLabel(bool draw) { this->bDrawLabel = draw; }

    // Getters
    bool GetDrawLabel(void) { return this->bDrawLabel; }

    // Event handlers
    void OnImageWndPaint(wxPaintEvent& event);
};

#endif	/* _RPROJWINDOW_H */

