/* 
 * File:   R3DScene.h
 * Author: jenicek
 *
 * Created on July 27, 2010, 12:35 PM
 */

#ifndef _R3DSCENE_H
#define	_R3DSCENE_H

#include "Global.h"
#include "R3DCamera.h"

class R3DScene : wxGLCanvas
{
private:
    T_GLVector2f ptOldMousePos;

    short drawMode;

    bool bShowCoords;
    bool bShowZeroPixels;
    bool bTextured;
    wxColour colNonZero;
    wxColour colZero;

    int objWidth;
    int objHeight;
    GLfloat objZMed;
    long numObjVertices;
    GLfloat *objVertices;
    GLfloat *objColors;
    bool *objMask;

    int sceneWidth;
    int sceneHeight;

    R3DCamera camera;

    // GL functions
    void DisplayCoordinates(void);
    void Display(void);
    void DrawCloud(void);
    void DrawVectors(void);
    void DrawMesh(void);
    void DrawWireframe(void);
    void Reshape(int w, int h);
    void InitGL();

public:
    R3DScene(wxFrame* parent, const wxPoint& pos, const wxSize& size, const wxString& title);

    // Class methods
    bool LoadVertices(char *fileName, int objWidth, int objHeight);
    bool LoadVertices(CvMat *depthMap);
    bool LoadVectors(CvMat *vectMap);
    bool LoadColors(char *fileName, int objWidth, int objHeight);
    bool LoadColors(CvMat *colorMap);
    bool LoadMask(char *fileName, int objWidth, int objHeight);
    bool LoadMask(CvMat *mask);
    void RenderGLWindow(void);
    void RedrawGLWindow(void);
    void ResetCamera(void);

    // Event handlers
    void OnKeyPressed(wxKeyEvent& event);
    void OnMousePressedMoved(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseRightDown(wxMouseEvent& event);

    // Setters
    bool SetColNonZeroStr(const wxString& hexCol) { return (this->colNonZero.Set(hexCol)); }
    bool SetColZeroStr(const wxString& hexCol) { return (this->colZero.Set(hexCol)); }
    void SetShowCoords(bool show) { this->bShowCoords = show; }
    void SetShowZeroPix(bool show) { this->bShowZeroPixels = show; }
    void SetTextured(bool textured) { this->bTextured = textured; }
    void SetDrawMode(short mode) { this->drawMode = mode; }

    // Getters
    int GetObjWidth(void) { return this->objWidth; }
    int GetObjheight(void) { return this->objHeight; }
    GLfloat* GetObjVertices(void) { return this->objVertices; }
    long GetNumObjVertices(void) { return this->numObjVertices; }
    GLfloat GetMousePosX(void) { return this->ptOldMousePos.x; }
    wxString GetColNonZeroStr(void) { return this->colNonZero.GetAsString(wxC2S_HTML_SYNTAX); }
    wxString GetColZeroStr(void) { return this->colZero.GetAsString(wxC2S_HTML_SYNTAX); }
    bool GetShowCoords(void) { return this->bShowCoords; }
    bool GetShowZeroPix(void) { return this->bShowZeroPixels; }
    bool GetTextured(void) { return this->bTextured; }
    short GetDrawMode(void) { return this->drawMode; }
};

#endif	/* _R3DSCENE_H */

