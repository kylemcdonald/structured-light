/* 
 * File:   R3DScene.cpp
 * Author: jenicek
 * 
 * Created on July 27, 2010, 12:35 PM
 */

#include "R3DScene.h"

float zoom = 10.0f;

R3DScene::R3DScene(wxFrame* parent, const wxPoint& pos, const wxSize& size, const wxString& title)
: wxGLCanvas(parent, -1, pos, size, wxSUNKEN_BORDER, title)
{
    this->ptOldMousePos = Vec2fSet(0.0f, 0.0f);

    this->drawMode = 0;
    this->bShowCoords = true;
    this->bShowZeroPixels = false;
    this->bTextured = true;
    this->colNonZero = wxColour(102, 102, 242, wxALPHA_OPAQUE);
    this->colZero = wxColour(77, 77, 77, wxALPHA_OPAQUE);
    
    this->objZMed = 0.0f;
    this->objWidth = 0;
    this->objHeight = 0;
    this->numObjVertices = 0;
    this->objVertices = NULL;
    this->objColors = NULL;
    this->objMask = NULL;

    this->sceneWidth = size.x;
    this->sceneHeight = size.y;

    // Initialize camera
    this->camera = R3DCamera(-300.0, 0.0, 0.0, this->objWidth / 2, this->objHeight / 2, this->objZMed, 0.0, -1.0, 0.0);
    this->Reshape(size.x, size.y);
    
    // Connect event handlers
    this->Connect(-1, wxEVT_KEY_DOWN, (wxObjectEventFunction)&R3DScene::OnKeyPressed);
    this->Connect(-1, wxEVT_MOTION, (wxObjectEventFunction)&R3DScene::OnMousePressedMoved);
    this->Connect(-1, wxEVT_LEFT_DOWN, (wxObjectEventFunction)&R3DScene::OnMouseLeftDown);
    this->Connect(-1, wxEVT_RIGHT_DOWN, (wxObjectEventFunction)&R3DScene::OnMouseRightDown);
}

bool R3DScene::LoadVertices(char* fileName, int objWidth, int objHeight)
{
    // Set some variables
    this->objWidth = objWidth;
    this->objHeight = objHeight;
    this->numObjVertices = objWidth * objHeight;
    this->objVertices = (GLfloat*)malloc(this->numObjVertices * 3 * sizeof(GLfloat));
    GLfloat maxZ = 0.0f;
    GLfloat minZ = 0.0f;

    // Vertex array has to be allocated
    if(this->objVertices == NULL)
        return false;

    // Try to open the file with the vertex data
    FILE *fIn = fopen(fileName, "r");
    if(fIn == NULL)
        return false;

    // Load points from the file
    // (each line contains one point in format: x;y;z)
    // Vertex array has following format
    // x1, y1, z1, x2, y2, z2, ..... it is 1 dimensional array
    int index = 0, i = 0;
    char input1[128], input2[128], input3[128];
    while((i = fscanf(fIn, "%[^;];%[^;];%[^\n] ", input1, input2, input3)) != EOF)
    {
        // Check whether 3 values has been red from this line
        if(i != 3)
            return 3;
        // 3 values has been red, add the mto the vertex array
        this->objVertices[index] = atof(input1);
        this->objVertices[index + 1] = atof(input2);
        this->objVertices[index + 2] = atof(input3);
        // Store max and min Z value
        if(this->objVertices[index + 2] > maxZ)
            maxZ = this->objVertices[index + 2];
        else if(this->objVertices[index + 2] < minZ)
            minZ = this->objVertices[index + 2];
        // Increment index value
        index += 3;
    }

    // Get Z median value
    this->objZMed = (maxZ + minZ) / 2.0;

    // Set new look at point
    this->camera.SetLookAt(this->objWidth / 2, this->objHeight / 2, this->objZMed);

    // Close the file
    fclose(fIn);
    // Everything's ok
    return true;
}

bool R3DScene::LoadVertices(CvMat* depthMap)
{
    // Set some variables
    this->objWidth = depthMap->cols;
    this->objHeight = depthMap->rows;
    this->numObjVertices = objWidth * objHeight;
    // Free old memroy and allocate new
    if(this->objVertices)
        free(this->objVertices);
    this->objVertices = (GLfloat*)malloc(this->numObjVertices * 3 * sizeof(GLfloat));
    GLfloat maxZ = 0.0f;
    GLfloat minZ = 0.0f;

    // Vertex array has to be allocated
    if(this->objVertices == NULL)
        return false;

    // Load points from OpenCV matrix
    for(int i = 0; i < this->numObjVertices; i++)
    {
        // 3 values has been red, add the mto the vertex array
        this->objVertices[i * 3] = i % depthMap->cols;
        this->objVertices[i * 3 + 1] = i / depthMap->cols;
        this->objVertices[i * 3 + 2] = depthMap->data.fl[i];
        // Store max and min Z value
        if(this->objVertices[i * 3 + 2] > maxZ)
            maxZ = this->objVertices[i * 3 + 2];
        else if(this->objVertices[i * 3 + 2] < minZ)
            minZ = this->objVertices[i * 3 + 2];
    }

    // Get Z median value
    this->objZMed = (maxZ + minZ) / 2.0;

    // Set new look at point
    this->camera.SetLookAt(this->objWidth / 2, this->objHeight / 2, this->objZMed);

    // Everything's ok
    return true;
}

bool R3DScene::LoadVectors(CvMat* vectMap)
{
    // Set some variables
    this->objWidth = vectMap->cols;
    this->objHeight = vectMap->rows;
    this->numObjVertices = objWidth * objHeight * 2;
    // Free old memroy and allocate new
    if(this->objVertices)
        free(this->objVertices);
    this->objVertices = (GLfloat*)malloc(this->numObjVertices * 3 * sizeof(GLfloat));
    GLfloat maxZ = 0.0f;
    GLfloat minZ = 0.0f;

    // Vertex array has to be allocated
    if(this->objVertices == NULL)
        return false;

    int r = 0;
    int c = 0;
    // Load points from OpenCV matrix
    for(int i = 0; i < this->numObjVertices / 2; i++)
    {
        c = i % vectMap->cols;
        r = i / vectMap->cols;
        // Base vector
        this->objVertices[i * 3] = c;
        this->objVertices[i * 3 + 1] = r;
        this->objVertices[i * 3 + 2] = 0;
        // End vector
        this->objVertices[i * 3 + this->numObjVertices / 2] = vectMap->data.fl[i * 3];
        this->objVertices[i * 3 + 1 + this->numObjVertices / 2] = vectMap->data.fl[i * 3 + 1];
        this->objVertices[i * 3 + 2 + this->numObjVertices / 2] = vectMap->data.fl[i * 3 + 2];
    }
    // Store max and min Z value
    maxZ = 1.0;
    minZ = 0.0;

    // Get Z median value
    this->objZMed = (maxZ + minZ) / 2.0;

    // Set new look at point
    this->camera.SetLookAt(this->objWidth / 2, this->objHeight / 2, this->objZMed);

    // Everything's ok
    return true;
}


bool R3DScene::LoadColors(char* fileName, int objWidth, int objHeight)
{
    // Set some variable
    int numPixels = objWidth * objHeight;
    this->objColors = (GLfloat*)malloc(numPixels * 3 * sizeof(GLfloat));

    // Color array has to be allocated
    if(this->objColors == NULL)
        return false;

    // Try to open the file with the color data
    FILE *fIn = fopen(fileName, "r");
    if(fIn == NULL)
        return false;

    // Load colors from the file
    // (each line contains one pixel color in format: r;g;b)
    // Color array has following format
    // r1, g1, b1, r2, g2, b2, ..... it is 1 dimensional array
    int index = 0, i = 0;
    char input1[128], input2[128], input3[128];
    while((i = fscanf(fIn, "%[^;];%[^;];%[^\n] ", input1, input2, input3)) != EOF)
    {
        // Check whether 3 values has been red from this line
        if(i != 3)
            return 3;
        // 3 values has been red, add the mto the vertex array
        this->objColors[index] = atof(input1);
        this->objColors[index + 1] = atof(input2);
        this->objColors[index + 2] = atof(input3);
        // Increment index value
        index += 3;
    }

    // Close the file
    fclose(fIn);
    // Everything's ok
    return true;
}

bool R3DScene::LoadColors(CvMat* colorMap)
{
    // Set some variable
    int numPixels = colorMap->cols * colorMap->rows;
    if(this->objColors)
        free(this->objColors);
    this->objColors = (GLfloat*)malloc(numPixels * 3 * sizeof(GLfloat));

    // Color array has to be allocated
    if(this->objColors == NULL)
        return false;

    // Load colors from OpenCV matrix
    for(int i = 0; i < numPixels * 3; i++)
    {
        // Copy color map 
        this->objColors[i] = colorMap->data.fl[i];
    }

    // Everything's ok
    return true;
}

bool R3DScene::LoadMask(char* fileName, int objWidth, int objHeight)
{
    // Set some variable
    int numPixels = objWidth * objHeight;
    this->objMask = (bool*)malloc(numPixels * sizeof(bool));

    // Mask array has to be allocated
    if(this->objMask == NULL)
        return false;

    // Try to open the file with the mask data
    FILE *fIn = fopen(fileName, "r");
    if(fIn == NULL)
        return false;

    // Load mask from the file
    // (each line contains one pixel mask data(0 or 1))
    int index = 0, i = 0;
    char input1[128];
    while((i = fscanf(fIn, "%[^\n] ", input1)) != EOF)
    {
        // Check whether 3 values has been red from this line
        if(i != 1)
            return false;
        // 3 values has been red, add the mto the vertex array
        this->objMask[index] = (bool)atoi(input1);
        // Increment index value
        index++;
    }
    
    return true;
}

bool R3DScene::LoadMask(CvMat* mask)
{
    // Set some variable
    int numPixels = mask->cols * mask->rows;
    if(this->objMask)
        free(this->objMask);
   this->objMask = (bool*)malloc(numPixels * sizeof(bool));

    // Mask array has to be allocated
    if(this->objMask == NULL)
        return false;

    // Load colors from OpenCV matrix
    for(int i = 0; i < numPixels; i++)
    {
        // Copy color map
        this->objMask[i] = mask->data.ptr[i];
    }

    // Everything's ok
    return true;
}

void R3DScene::RenderGLWindow(void)
{    
    this->SetCurrent();

    if(this->bShowCoords)
        this->DisplayCoordinates();

    if(this->objVertices != NULL)
    {
        this->InitGL();
        this->Reshape(this->sceneWidth, this->sceneHeight);
        this->Display();
    }

    this->SwapBuffers();
}

void R3DScene::RedrawGLWindow(void)
{
    if(this->bShowCoords)
        this->DisplayCoordinates();
    // Render new scene
    this->Display();
}

void R3DScene::ResetCamera(void)
{
    this->camera.SetEye(-300.0, 0.0, 0.0);
    this->camera.SetLookAt(this->objWidth / 2, -this->objHeight / 2, this->objZMed);
    this->camera.SetUpVec(0.0, -1.0, 0.0);
    this->camera.SetROrbX(0.0);
    this->camera.SetROrbY(0.0);
    this->camera.SetZoom(0.0);
}

void R3DScene::OnKeyPressed(wxKeyEvent& event)
{
    switch(event.GetKeyCode())
    {
        case (int)'q':
            exit(0);
            break;
    }

    // Redisplay the scene
    this->RedrawGLWindow();
}

void R3DScene::OnMouseLeftDown(wxMouseEvent& event)
{
    
}

void R3DScene::OnMouseRightDown(wxMouseEvent& event)
{
    
}

void R3DScene::OnMousePressedMoved(wxMouseEvent& event)
{
    // If the left mouse button is pressed
    if(event.m_leftDown)
    {
        //this->camera.UpdateROrbX((event.m_y - this->ptOldMousePos.y) / 10.0f);
        this->camera.UpdateROrbY((event.m_x - this->ptOldMousePos.x) / 10.0f);
      
        // Redisplay the scene
        this->RedrawGLWindow();
    }
    // If the right mouse button is pressed
    if(event.m_rightDown)
    {
        // Zoom the view
        if((event.m_y - this->ptOldMousePos.y) > 0.0f)
            this->camera.UpdateZoom(50.0f);
        else if((event.m_y - this->ptOldMousePos.y) < 0.0f)
            this->camera.UpdateZoom(-50.0f);
        
        // Redisplay the scene
        this->RedrawGLWindow();
    }

    // Store current mouse position
    this->ptOldMousePos.x = event.m_x;
    this->ptOldMousePos.y = event.m_y;
}

/* * * * * * * * * * * GL FUNCTIONS * * * * * * * * * * * * * * */
// Display coordinates function
void R3DScene::DisplayCoordinates(void)
{
    // Get look at and eye vectors
    T_GLVector3f vEye = this->camera.GetEye();
    T_GLVector3f vLookAt = this->camera.GetLookAt();
    // Prepare string
    wxString str;
    // Get drawing context
    wxPaintDC dc(this);
    // Set text font and color
    dc.SetFont(wxFont(9, wxMODERN, wxNORMAL, wxNORMAL, false));
    dc.SetTextForeground(*wxWHITE);
    
    str.Printf("                X           Y           Z");
    dc.DrawText(str, 10, 10);
    str.Printf("Object center:  %-12.4f%-12.4f%-12.4f", this->objWidth / 2.0, this->objHeight / 2.0, this->objZMed);
    dc.DrawText(str, 10, 30);
    //str.Printf("Camera eye:     %-12.4f%-12.4f%-12.4f", vEye.x, vEye.y, vEye.z);
    //dc.DrawText(str, 10, 50);
    str.Printf("Camera look at: %-12.4f%-12.4f%-12.4f", vLookAt.x, vLookAt.y, vLookAt.z);
    dc.DrawText(str, 10, 50);

    // Destroy drawing context
    dc.Destroy();
}

// GL reshape function (viewport settings)
void R3DScene::Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    this->camera.Perspective(45.0, 1.0, 1.0, 5000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Draw point cloud function
void R3DScene::DrawCloud(void)
{
    // Draw the points
    glBegin(GL_POINTS);
    //glVertex3f(0.0, 0.0, 0.0);
    for(int i = 0; i < this->numObjVertices; i++)
    {
        if(!this->objMask[i])
        {
            //Draw non zero
            glColor3f(colNonZero.Red() / 255.0f, colNonZero.Green() / 255.0f, colNonZero.Blue() / 255.0f);
            glArrayElement(i);
        }
        else if(this->bShowZeroPixels)
        {
            //Draw zero
            glColor3f(colZero.Red() / 255.0f, colZero.Green() / 255.0f, colZero.Blue() / 255.0f);
            glArrayElement(i);
        }
    }
    glEnd();
}

// Draw object mesh function
void R3DScene::DrawMesh(void)
{
    // Draw mesh triangles
    glBegin(GL_TRIANGLES);

    for(int y = 0; y < this->objHeight - 1; y++)
    {
        for(int x = 0; x < this->objWidth - 1; x++)
        {
            int nw = y * this->objWidth + x;
            int ne = nw + 1;
            int sw = nw + this->objWidth;
            int se = ne + this->objWidth;
            if(!this->objMask[nw] && !this->objMask[se])
            {
                if(!this->objMask[ne])
                {
                    // Triangle NW, NE, SE
                    glArrayElement(nw);
                    glArrayElement(ne);
                    glArrayElement(se);
                }
                if(!this->objMask[sw])
                {
                    // Triangle NW, SE, Sw
                    glArrayElement(nw);
                    glArrayElement(se);
                    glArrayElement(sw);
                }
            }
            else if(!this->objMask[ne] && !this->objMask[sw])
            {
                if(!this->objMask[nw])
                {
                    // Triangle NW, NE, SW
                    glArrayElement(nw);
                    glArrayElement(ne);
                    glArrayElement(sw);
                }
                if(!this->objMask[se])
                {
                    // Triangle NE, SE, SW
                    glArrayElement(ne);
                    glArrayElement(se);
                    glArrayElement(sw);
                }
            }
        }
    }

    glEnd();
}

// Draw wireframe object model
void R3DScene::DrawWireframe(void)
{
    
}

// Draw vectors
void R3DScene::DrawVectors(void)
{
    // Draw the vectors
    glBegin(GL_LINES);
    for(int i = 0; i < this->numObjVertices / 2; i++)
    {
        if(!this->objMask[i])
        {
            //Draw non zero
            glColor3f(colNonZero.Red() / 255.0f, colNonZero.Green() / 255.0f, colNonZero.Blue() / 255.0f);
            glArrayElement(i);
            glArrayElement(i + this->numObjVertices / 2);
        }
        else if(this->bShowZeroPixels)
        {
            //Draw zero
            glColor3f(colZero.Red() / 255.0f, colZero.Green() / 255.0f, colZero.Blue() / 255.0f);
            glArrayElement(i);
            glArrayElement(i + this->numObjVertices / 2);
        }
    }
    glEnd();
}

// GL display function (rendering)
void R3DScene::Display(void)
{
    glLoadIdentity();
    //Viewing transformation
    this->camera.Place();
    this->camera.Zoom();
    this->camera.RotOrbital();
    
    //Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable color arrays
    if(this->bTextured)
        glEnableClientState(GL_COLOR_ARRAY);
    // Enable vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // Set color array pointer
    if(this->bTextured)
        glColorPointer(3, GL_FLOAT, 0, this->objColors);
    // Set vertex array pointer
    glVertexPointer(3, GL_FLOAT, 0, this->objVertices);

    // Draw desired model
    if(this->drawMode == 0)
        this->DrawCloud();
    else if(this->drawMode == 1)
        this->DrawWireframe();
    else if(this->drawMode == 2)
        this->DrawMesh();
    else if(this->drawMode == 3)
        this->DrawVectors();
    
    // Disable color array
    if(this->bTextured)
        glDisableClientState(GL_COLOR_ARRAY);

    //Don't wait
    //Start processing buffered OpenGL routines
    glFlush();
}

// GL init function
void R3DScene::InitGL()
{
    // Init OpenGL
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}
/* * * * * * * * * * * GL FUNCTIONS (END) * * * * * * * * * * * */


