
#include "RScanUtil.h"


// Line plane intersection in 3D
void IntersectLinePlane3D(const float *q, const float *v, const float *w, float *p, float *depth)
{
    // Evalueate inner products
    float nDotQ = 0, nDotV = 0;
    for(int i = 0; i < 3; i++)
    {
        nDotQ += w[i] * q[i];
        nDotV += w[i] * v[i];
    }

    // Evaluate point of intersection P
    (*depth) = (w[3] - nDotQ) / nDotV;
    for(int i = 0; i < 3; i++)
        p[i] = q[i] + (*depth) * v[i];
}

// Line line intersection in 3D
void IntersectLineLine3D(const float *q1, const float *v1, const float *q2, const float *v2, float *p)
{
    // Define intermediate quantities
    float q12[3], v1DotV1 = 0, v2DotV2 = 0, v1DotV2 = 0, q12DotV1 = 0, q12DotV2 = 0;
    for(int i = 0; i < 3; i++)
    {
        q12[i] = q1[i] - q2[i];
        v1DotV1 += v1[i] * v1[i];
        v2DotV2 += v2[i] * v2[i];
        v1DotV2 += v1[i] * v2[i];
        q12DotV1 += q12[i] * v1[i];
        q12DotV2 += q12[i] * v2[i];
    }

    // Calculate scale factors
    float s, t, denom;
    denom = v1DotV1 * v2DotV2 - v1DotV2 * v1DotV2;
    s = (v1DotV2 / denom) * q12DotV2 - (v2DotV2 / denom) * q12DotV1;
    t = -(v1DotV2 / denom) * q12DotV1 - (v1DotV1 / denom) * q12DotV2;

    // Evaluate closest point
    for(int i = 0; i < 3; i++)
        p[i] = ((q1[i] + s * v1[i]) + (q2[i] + t * v2[i])) / 2;
}

// Change phase using ratio value
void ChangePhaseFL(CvMat** phase, float ratio)
{
    for(int r = 0; r < (*phase)->rows; r++)
    {
        for(int c = 0; c < (*phase)->cols; c++)
        {
            (*phase)->data.fl[r * (*phase)->cols + c] *= ratio;
        }
    }
}

// Change image values using ratio value
void ChangeImg3CHUCH(CvMat** img, unsigned char ratio)
{
    for(int r = 0; r < (*img)->rows; r++)
    {
        for(int c = 0; c < (*img)->cols; c++)
        {
            (*img)->data.ptr[r * (*img)->cols * 3 + c * 3] *= ratio;
            (*img)->data.ptr[r * (*img)->cols * 3 + c * 3 + 1] *= ratio;
            (*img)->data.ptr[r * (*img)->cols * 3 + c * 3 + 2] *= ratio;
        }
    }
}

// Convert 3 channel float image to 3 channel uchar image
void ImgFL3ToUCH3(CvMat* phaseFl, CvMat** phaseUch)
{
    for(int r = 0; r < phaseFl->rows; r++)
    {
        for(int c = 0; c < phaseFl->cols * 3; c++)
        {
            (*phaseUch)->data.ptr[r * phaseFl->cols * 3 + c] = phaseFl->data.fl[r * phaseFl->cols * 3 + c] * 255;
        }
    }
}

// Convert 1 channel float image to 3 channel uchar image
void Img1FLTo3UCH(CvMat* phaseFl, CvMat** phaseUch)
{
    for(int r = 0; r < phaseFl->rows; r++)
    {
        for(int c = 0; c < phaseFl->cols; c++)
        {
            (*phaseUch)->data.ptr[r * phaseFl->cols * 3 + c * 3] = phaseFl->data.fl[r * phaseFl->cols + c] * 255;
            (*phaseUch)->data.ptr[r * phaseFl->cols * 3 + c * 3 + 1] = phaseFl->data.fl[r * phaseFl->cols + c] * 255;
            (*phaseUch)->data.ptr[r * phaseFl->cols * 3 + c * 3 + 2] = phaseFl->data.fl[r * phaseFl->cols + c] * 255;
        }
    }
}

// Convert 1 channel uchar image to 3 channel uchar image
void Img1UCHTo3UCH(CvMat* phase1Uch, CvMat** phase3Uch)
{
    for(int r = 0; r < phase1Uch->rows; r++)
    {
        for(int c = 0; c < phase1Uch->cols; c++)
        {
            (*phase3Uch)->data.ptr[r * phase1Uch->cols * 3 + c * 3] = phase1Uch->data.ptr[r * phase1Uch->cols + c];
            (*phase3Uch)->data.ptr[r * phase1Uch->cols * 3 + c * 3 + 1] = phase1Uch->data.ptr[r * phase1Uch->cols + c];
            (*phase3Uch)->data.ptr[r * phase1Uch->cols * 3 + c * 3 + 2] = phase1Uch->data.ptr[r * phase1Uch->cols + c];
        }
    }
}

// Find maximum of 3 floats
float GetMax(float x, float y, float z)
{
    if(x > y)
        return x > z ? x : z;
    else
        return y > z ? y : z;
}

// Find minimum of 3 floats
float GetMin(float x, float y, float z)
{
    if(x < y)
        return x < z ? x : z;
    else
        return y < z ? y : z;
}

// Find median of 3 floats
float GetMed(float x, float y, float z)
{
    if(x < y)
        if(z < x)
            return x;
        else
            return z < y ? z : y;
    else
        if(z < y)
            return y;
        else
            return z < x ? z : x;
}

// Get average brightness of a pixel
float GetAvgBrightness(float red, float green, float blue)
{
    return (red + green + blue) / (255.0 * 3.0);
}

// Get difference between two neighbour values
float GetDiff(float val1, float val2)
{
    // Get the positive difference
    float diff = val1 < val2 ? val2 - val1 : val1 - val2;
    // Return difference value(always < 0.5)
    return diff < 0.5 ? diff : 1 - diff;
}

// Unwrapping phase intensity ratio
float GetIntRatio(float ph1, float ph2, float ph3)
{
    float r = 0.0;
    float maxPh = GetMax(ph1, ph2, ph3);
    float minPh = GetMin(ph1, ph2, ph3);
    float medPh = GetMed(ph1, ph2, ph3);

    // This equation is also from Zhang's PhD disertation
    // It removes triangular shape from original intensity ratio map
    r = (medPh - minPh) / (maxPh - minPh  + 0.00001);

    return r;
}

// Get wrapped phase
float GetPhase(float ph1, float ph2, float ph3)
{
    /*
     * According to Fig. 4.1(p. 77) in Zhang's PhD disertation
     * Value N is chosen using table below
     *  N   max     med     min
     *  1   ph1     ph2     ph3
     *  2   ph2     ph1     ph3
     *  3   ph2     ph3     ph1
     *  4   ph3     ph2     ph1
     *  5   ph3     ph1     ph2
     *  6   ph1     ph3     ph2
    */
    int N = 0;
    short invert = 1;
    float r = 0.0;
    float ph = 0.0;
    float maxPh = GetMax(ph1, ph2, ph3);
    float minPh = GetMin(ph1, ph2, ph3);
    float medPh = GetMed(ph1, ph2, ph3);
    if(maxPh == ph1 && minPh == ph2)
        N = 0;
    else if(maxPh == ph3 && minPh == ph2)
    {
        invert = -1;
        N = 2;
    }
    else if(maxPh == ph3 && minPh == ph1)
        N = 2;
    else if(maxPh == ph2 && minPh == ph1)
    {
        invert = -1;
        N = 4;
    }
    else if(maxPh == ph2 && minPh == ph3)
        N = 4;
    else
    {
        invert = -1;
        N = 6;
    }

    // Use method introduced by Zhang(slightly modified)
    if(maxPh != minPh)
        r = N + invert * ((medPh - minPh) / (maxPh - minPh));
    else
        r += N * (invert);
    // Return phase
    return r / 6.0;
}

// Get wrapped phase
float GetPhase2(float ph1, float ph2, float ph3, float *intRatio)
{
    /*
     * According to Fig. 4.1(p. 77) in Zhang's PhD disertation
     * Value N is chosen using table below
     *  N   max     med     min
     *  1   ph1     ph3     ph2
     *  2   ph3     ph1     ph2
     *  3   ph3     ph2     ph1
     *  4   ph2     ph3     ph1
     *  5   ph2     ph1     ph3
     *  6   ph1     ph2     ee pdf downloadph3
    */
    int N = 0;
    short invert = 1;
    *intRatio = 0.0;
    float ph = 0.0;
    float maxPh = GetMax(ph1, ph2, ph3);
    float minPh = GetMin(ph1, ph2, ph3);
    float medPh = GetMed(ph1, ph2, ph3);
    if(maxPh == ph1 && minPh == ph2)
        N = 1;
    else if(maxPh == ph3 && minPh == ph2)
    {
        invert = -1;
        N = 2;
    }
    else if(maxPh == ph3 && minPh == ph1)
        N = 3;
    else if(maxPh == ph2 && minPh == ph1)
    {
        invert = -1;
        N = 4;
    }
    else if(maxPh == ph2 && minPh == ph3)
        N = 5;
    else
    {
        invert = -1;
        N = 6;
    }

    // This equation is from Zhang's fast three-step phase-shifting method
    *intRatio = (medPh - minPh) / (maxPh - minPh  + 0.00001);
    *intRatio = 2.0 * round(N / 2) + (invert) * (*intRatio);
    ph = (MATH_PI / 3.0) * (*intRatio);

    // Return normalized phase
    return ph / (2 * MATH_PI);
}

// Get wrapped phase
float GetPhase3(float ph1, float ph2, float ph3, float *intRatio)
{
    /*
     * According to Fig. 4.1(p. 77) in Zhang's PhD disertation
     * Value N is chosen using table below
     *  N   max     med     min
     *  1   ph2     ph1     ph3
     *  2   ph1     ph2     ph3
     *  3   ph1     ph3     ph2
     *  4   ph3     ph1     ph2
     *  5   ph3     ph2     ph1
     *  6   ph2     ph3     ph1
    */
    int N = 0;
    short invert = 1;
    *intRatio = 0.0;
    float ph = 0.0;
    float maxPh = GetMax(ph1, ph2, ph3);
    float minPh = GetMin(ph1, ph2, ph3);
    float medPh = GetMed(ph1, ph2, ph3);
    if(maxPh == ph2 && minPh == ph3)
        N = 1;
    else if(maxPh == ph1 && minPh == ph3)
    {
        invert = -1;
        N = 2;
    }
    else if(maxPh == ph1 && minPh == ph2)
        N = 3;
    else if(maxPh == ph3 && minPh == ph2)
    {
        invert = -1;
        N = 4;
    }
    else if(maxPh == ph3 && minPh == ph1)
        N = 5;
    else
    {
        invert = -1;
        N = 6;
    }

    // This equation is from Zhang's fast three-step phase-shifting method
    *intRatio = (medPh - minPh) / (maxPh - minPh + 0.00001);
    ph = 2.0 * round(N / 2) + (invert) * (*intRatio);
    ph *= (MATH_PI / 3.0);
    ph /= (2.0 * MATH_PI);
    
    // Return normalized phase
    return ph;
}

void MakeRefPhaseMap(char *fPath, CvMat** refPhase)
{
    // Initialize and load new phase images
    IplImage **grayPhImages = (IplImage**)malloc(sizeof(IplImage*) * 3);
    char fName[256];
    for(int i = 0; i < 3; i++)
    {
        sprintf(fName, "%s/ref%d.jpg", fPath, i + 1);
        grayPhImages[i] = cvLoadImage((char*)fName, CV_LOAD_IMAGE_GRAYSCALE);
    }
    int phaseWidth = grayPhImages[0]->widthStep;
    int phaseHeight = grayPhImages[0]->height;

    float iRat;
    // Loop through each pixel in the phase images and wrap them
    for(int r = 0; r < phaseHeight; r++)
    {
        for(int c = 0; c < phaseWidth; c++)
        {
            // Get pixel position
            int pPos = r * phaseWidth + c;

            // Compute phase values
            // (get and process the pixel color from each phase image)
            uchar phase0 = grayPhImages[0]->imageData[pPos];
            uchar phase1 = grayPhImages[1]->imageData[pPos];
            uchar phase2 = grayPhImages[2]->imageData[pPos];
                 
            // Get the reference phase
            (*refPhase)->data.fl[pPos] = GetPhase3(phase0, phase1, phase2, &iRat);
        }
    }

    //cvShowImage("refPhase", *refPhase);
}

void MakeDepth4(CvMat* phase, CvMat* mask, CvMat **depth)
{
    CvMat *refPhase = cvCreateMat(phase->rows, phase->cols, CV_32FC1);
    MakeRefPhaseMap((char*)"/home/jenicek/NetBeansProjects/ReconstApp/dist/Debug/GNU-Linux-x86/capture/globe4", &refPhase);

    // Loop through each pixel in the phase maps and compute depth
    for(int r = 0; r < phase->rows; r++)
    {
        for(int c = 0; c < phase->cols; c++)
        {
            if(!mask->data.ptr[r * phase->cols + c])
                // Get the reference phase
                (*depth)->data.fl[r * phase->cols + c] = phase->data.fl[r * phase->cols + c] - refPhase->data.fl[r * phase->cols + c];
            else
                (*depth)->data.fl[r * phase->cols + c] = 0.0f;
        }
    }

    cvReleaseMat(&refPhase);    
}

// Wrapping the phases
void PhaseWrap(IplImage** phaseImages, CvMat** phase, CvMat** process, CvMat** iRatio, CvMat** colors,
        float cutoffThresh)
{
    // Prepare square-root of number 3(needed in the cycle, so to avoid computing it
    // in the cycle again and again)(*process)->data.ptr[pPos]
    float sqrt3 = sqrt(3.0);

    // Get phase width and height
    int phaseWidth = phaseImages[0]->widthStep;
    int phaseHeight = phaseImages[0]->height;

    // Create grayscale copies of the colored phase images
    IplImage** grayPhImages = (IplImage**)malloc(sizeof(IplImage*) * 3);
    for(int i = 0; i < 3; i++)
    {
        grayPhImages[i] = cvCreateImage(cvSize(phaseWidth / 3, phaseHeight), IPL_DEPTH_8U, 1);
        cvCvtColor(phaseImages[i], grayPhImages[i], CV_RGB2GRAY);
    }

    // Prepare variable for temporary storage of iRatio values
    float iRat;
    // Prepare variables for color computations
    float colImg1, colImg2, colImg3, tmp1, tmp2;
    // Loop through each pixel in the phase images and wrap them
    for(int r = 0; r < phaseHeight; r++)
    {
        for(int c = 0; c < (phaseWidth / 3); c++)
        {
            // Get pixel position
            int pPos = r * (phaseWidth / 3) + c;

            // Compute phase values
            // (get and process the pixel color from each phase image)
            uchar phase0 = grayPhImages[0]->imageData[r * phaseWidth / 3 + c];
            uchar phase1 = grayPhImages[1]->imageData[r * phaseWidth / 3 + c];
            uchar phase2 = grayPhImages[2]->imageData[r * phaseWidth / 3 + c];

            // Compute phase range
            float phaseRange = GetMax(phase0, phase1, phase2) - GetMin(phase0, phase1, phase2);

            //(*iRatio)->data.fl[pPos] = GetIntRatio(phase0, phase1, phase2);
            //(*iRatio)->data.fl[pPos] = fabs(sqrt(3.0 * (phase0 - phase2) * (phase0 - phase2) + (2.0 * phase1 - phase0 - phase2) * (2.0 * phase1 - phase0 - phase2)) / (phase0 + phase1 + phase2));
            // Set ignore noise mask and intensity ratio
            (*process)->data.ptr[pPos] = !(phaseRange <= cutoffThresh);

            // Get the phase
            //(*phase)->data.fl[pPos] = GetPhase(phase0, phase1, phase2);
            // Equate phase(equation presented by Zhang in one of his papers)
            //(*phase)->data.fl[pPos] = atan2(sqrt3 *  (phase0 - phase2), 2.0 * phase1 - phase0 - phase2) / (2.0 * MATH_PI);

            (*phase)->data.fl[pPos] = GetPhase3(phase0, phase1, phase2, &iRat);
            (*iRatio)->data.fl[pPos] = iRat;

            // Get color information based on the lightest channels from all three images
            // (use original colored phase images)
            // (apply BGR -> RGB conversion)
            // Red            if(arr[index] == dR)
            (*colors)->data.fl[r * phaseWidth + c * 3 + 2] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3] / 128.0)) / 3.0;
            // Green
            (*colors)->data.fl[r * phaseWidth + c * 3 + 1] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3 + 1] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3 + 1] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3 + 1] / 128.0)) / 3.0;
            // Blue
            (*colors)->data.fl[r * phaseWidth + c * 3] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3 + 2] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3 + 2] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3 + 2] / 128.0)) / 3.0;
        }
    }
}

void PhaseWrap4(IplImage** phaseImages, CvMat** phase, CvMat** process, CvMat** iRatio, CvMat** colors,
        float cutoffThresh)
{
    // Get phase width and height
    int phaseWidth = phaseImages[0]->widthStep;
    int phaseHeight = phaseImages[0]->height;

    // Create grayscale copies of the colored phase images
    IplImage** grayPhImages = (IplImage**)malloc(sizeof(IplImage*) * 4);
    for(int i = 0; i < 4; i++)
    {
        grayPhImages[i] = cvCreateImage(cvSize(phaseWidth / 3, phaseHeight), IPL_DEPTH_8U, 1);
        cvCvtColor(phaseImages[i], grayPhImages[i], CV_RGB2GRAY);
    }

    // Loop through each pixel in the phase images and wrap them
    for(int r = 0; r < phaseHeight; r++)
    {
        for(int c = 0; c < (phaseWidth / 3); c++)
        {
            // Get pixel position
            int pPos = r * (phaseWidth / 3) + c;

            // Compute phase values
            // (get and process the pixel color from each phase image)
            uchar phase0 = grayPhImages[0]->imageData[r * phaseWidth / 3 + c];
            uchar phase1 = grayPhImages[1]->imageData[r * phaseWidth / 3 + c];
            uchar phase2 = grayPhImages[2]->imageData[r * phaseWidth / 3 + c];
            uchar phase3 = grayPhImages[3]->imageData[r * phaseWidth / 3 + c];

            // Compute phase range
            float phaseRange = sqrt((phase3 - phase1) * (phase3 - phase1) + (phase0 - phase2) * (phase0 - phase2)) / 2.0;
   
            (*iRatio)->data.fl[pPos] = 2 * sqrt((phase3 - phase1) * (phase3 - phase1) + (phase0 - phase2) * (phase0 - phase2)) / phase0 + phase1 + phase2 + phase3;;
            //(*iRatio)->data.fl[pPos] = GetIntRatio(phase0, phase1, phase2);
            //(*iRatio)->data.fl[pPos] = fabs(sqrt(3.0 * (phase0 - phase2) * (phase0 - phase2) + (2.0 * phase1 - phase0 - phase2) * (2.0 * phase1 - phase0 - phase2)) / (phase0 + phase1 + phase2));
            // Set ignore noise mask and intensity ratio
            (*process)->data.ptr[pPos] = !(phaseRange <= cutoffThresh);

            // (the divider could be also simply MATH_PI)
            (*phase)->data.fl[pPos] = atan((phase3 - phase1) / (phase0 - phase2 + 0.00001)) / ((3 / 2) * MATH_PI);
            
            // Get color information based on the lightest channels from all three images
            // (use original colored phase images)
            // (apply BGR -> RGB conversion)
            // Red            if(arr[index] == dR)
            (*colors)->data.fl[r * phaseWidth + c * 3 + 2] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3] / 128.0) +
                    fabs((float)phaseImages[3]->imageData[r * phaseWidth + c * 3] / 128.0)) / 4.0;
            // Green
            (*colors)->data.fl[r * phaseWidth + c * 3 + 1] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3 + 1] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3 + 1] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3 + 1] / 128.0) +
                    fabs((float)phaseImages[3]->imageData[r * phaseWidth + c * 3 + 1] / 128.0)) / 4.0;
            // Blue
            (*colors)->data.fl[r * phaseWidth + c * 3] = (fabs((float)phaseImages[0]->imageData[r * phaseWidth + c * 3 + 2] / 128.0) +
                    fabs((float)phaseImages[1]->imageData[r * phaseWidth + c * 3 + 2] / 128.0) +
                    fabs((float)phaseImages[2]->imageData[r * phaseWidth + c * 3 + 2] / 128.0) +
                    fabs((float)phaseImages[3]->imageData[r * phaseWidth + c * 3 + 2] / 128.0)) / 4.0;
        }
    }
}

// Create LUT for error compensation
void CreateLUT(double **lut, long lutSize)
{
    // Divide <0, 1> interval into 256 values
    // Compute intensity ratio error for each of that values
    // And simultaneously, fill the lut in
    double step = 1.0 / lutSize;
    for(double val = 0.0f; val <= 1.0f; val += step)
        (*lut)[(long)(val * lutSize)] = 1.0 / 2.0 + (sqrt(3.0) / 2.0) * tan(val - (MATH_PI / 6)) - val / (MATH_PI / 3.0);
}

// Apply LUT on the phase map
void ApplyLUT(CvMat** phase, double *lut, long lutSize)
{
    // Go through each phase value and apply error compensation using LUT
    for(int r = 0; r < (*phase)->rows; r++)
    {
        for(int c = 0; c < (*phase)->cols; c++)
        {
            (*phase)->data.fl[r * (*phase)->cols + c] -= lut[(long)((*phase)->data.fl[r * (*phase)->cols + c] * lutSize)];
        }
    }
}

// Release memory allocated for LUT
void ReleaseLUT(double **lut)
{
    free(*lut);
    *lut = NULL;
}

// Wrap differences of wrapped phase
float WrapPhaseDifference(float ph1, float ph2)
{
    // Estimates the true gradient by wrapping the differences of wrapped phase
    float innerDiff = ph1 - ph2;
    return innerDiff < 0.5 ? innerDiff : 1.0 - innerDiff;
}
// Create quality map
void CreateQualityMap(CvMat *phase, CvMat* process, CvMat** quality)
{
    /*
     * This solution for obtaining the quality map is from
     * Multilevel quality-guided phase unwrapping algorithm for real-time three-dimensional shape reconstruction
     * by Zhang, Li and Xau
     */

    // Clear whole quality map
    cvSet((*quality), cvScalarAll(0.0), NULL);

    // Go through each pixel in the phase map except the borders
    float up, down, left, right;
    for(int r = 1; r < phase->rows - 1; r++)
    {
        for(int c = 1; c < phase->cols - 1; c++)
        {
            int pPos = r * phase->cols + c;

            // If this pixel should be processed
            if(process->data.ptr[r * phase->cols + c])
            {
                // Compute wrapped phase differences
                up = WrapPhaseDifference(phase->data.fl[pPos], phase->data.fl[pPos - phase->cols]);
                down = WrapPhaseDifference(phase->data.fl[pPos + phase->cols], phase->data.fl[pPos]);
                left = WrapPhaseDifference(phase->data.fl[pPos], phase->data.fl[pPos - 1]);
                right = WrapPhaseDifference(phase->data.fl[pPos + 1], phase->data.fl[pPos]);
                // Quality is the highest of the four values
                (*quality)->data.fl[pPos] = up;
                if((*quality)->data.fl[pPos] < down)
                    (*quality)->data.fl[pPos] = down;
                else if((*quality)->data.fl[pPos] < left)
                    (*quality)->data.fl[pPos] = left;
                else if((*quality)->data.fl[pPos] < right)
                    (*quality)->data.fl[pPos] = right;

            }
            else
                (*quality)->data.fl[pPos] = PIX_BACKGROUND;
        }
    }
}

void ComputeThresholds(CvMat *quality, float **thresholds, int numThresholds)
{
    // Image size
    int numVals = 0;
    // Compute quality mean value
    float meanValue = 0.0f;
    for(int r = 0; r < quality->rows; r++)
    {
        for(int c = 0; c < quality->cols; c++)
        {
            if(quality->data.fl[r * quality->cols + c] != PIX_BACKGROUND)
            {
                meanValue += quality->data.fl[r * quality->cols + c];
                numVals++;
            }
        }
    }
    meanValue /= numVals;
    // Compute standard deviation
    float stdDev = 0.0f;
    for(int r = 0; r < quality->rows; r++)
    {
        for(int c = 0; c < quality->cols; c++)
        {
            if(quality->data.fl[r * quality->cols + c] != PIX_BACKGROUND)
                stdDev += (quality->data.fl[r * quality->cols + c] - meanValue) * (quality->data.fl[r * quality->cols + c] - meanValue);
        }
    }
    stdDev = sqrt(stdDev / numVals);

    // Get thresholds
    // First threshold is equal to mean Value
    (*thresholds)[0] = meanValue;
    for(int i = 1; i < numThresholds; i++)
    {
        (*thresholds)[i] = (*thresholds)[0] + (1 << (i - 1)) * stdDev;
    }
}

void PhaseUnwrapScanLine(CvMat **phase, CvMat **mask, CvMat **quality, float *thresholds, int numThresholds)
{
    // For now simply choose the center pixel
    CvPoint ptStart = cvPoint((*quality)->cols / 2, (*quality)->rows / 2);

    // Array for storing offsets
    CvMat *phOffset = cvCreateMat((*quality)->rows, (*quality)->cols, CV_32FC1);
    cvSet(phOffset, cvScalarAll(0.0), NULL);

    // If the quality of this point is not 0, unwrap it and turn it to zero, because it's being uwrapped now
    // If it is 0, leave this point
    if((*quality)->data.fl[ptStart.y * (*quality)->cols + ptStart.x] != 0.0)
        (*quality)->data.fl[ptStart.y * (*quality)->cols + ptStart.x] = PIX_UNWRAPPED;

    // Set phase offset at the starting point to 0
    phOffset->data.fl[ptStart.y * (*quality)->cols + ptStart.x] = 0.0;

    // Unwrap all quality levels
    for(int i = 0; i < numThresholds; i++)
    {
        UnwrapPatch(phase, quality, &phOffset, thresholds[i], ptStart, 0, 0, -1, -1);
        UnwrapPatch(phase, quality, &phOffset, thresholds[i], ptStart, 0, (*quality)->rows, -1, 1);
        UnwrapPatch(phase, quality, &phOffset, thresholds[i], ptStart, (*quality)->cols, 0, 1, -1);
        UnwrapPatch(phase, quality, &phOffset, thresholds[i], ptStart, (*quality)->cols, (*quality)->rows, 1, 1);
    }

    // Set all mask pixels to 1
    cvSet(*mask, cvScalarAll(1), NULL);

    // Now make the pixels unwrapped
    for(int r = 0; r < (*phase)->rows; r++)
    {
        for(int c = 0; c < (*phase)->cols; c++)
        {
            int posCur = r * (*phase)->cols + c;
            // If the pixel has been unwrapped, actualize it's phase and also set mask for this pixel
            if((*quality)->data.fl[posCur] == PIX_UNWRAPPED)
            {
                // Unwrap this pixel's phase
                (*phase)->data.fl[posCur] += phOffset->data.fl[posCur];
                // Set mask for this pixel to 0
                (*mask)->data.ptr[posCur] = 0;
            }
            else
                // If this pixel was not unwrapped, set it's phase to 0
                (*phase)->data.fl[posCur] = 0;
        }
    }
}

float UnwrapPix(float currPhase, float nbrPhase)
{
    if(currPhase - nbrPhase > 0.5)
        return -1.0;
    else if(currPhase - nbrPhase < -0.5)
        return 1.0;
    else
        return 0.0;
}

void UnwrapPatch(CvMat **phase, CvMat **quality, CvMat **phOffset, float threshold, CvPoint ptStart,
        int borderX, int borderY, int stepX, int stepY)
{
    int posCur = ptStart.y * (*quality)->cols + ptStart.x;
    int posNbr = ptStart.y * (*quality)->cols + ptStart.x;

    // Prepare array for possible remainder pixels that has been left over for further processing
    int numRem = abs(ptStart.x - borderX) * abs(ptStart.y - borderY);
    int *remainders = (int*)malloc(numRem * sizeof(int));
    numRem = 0;

    // Go thorugh each row and column in this patch
    for(int r = ptStart.y; r != borderY - 1; r += stepY)
    {
        for(int c = ptStart.x; c != borderX - 1; c += stepX)
        {
            // Compute current position
            posCur = r * (*quality)->cols + c;

            // If the quality is in the current level
            if((*quality)->data.fl[posCur] <= threshold)
            {
                // Try to unwrap from first start facing pixel
                posNbr = r * (*quality)->cols + (c - stepX);
                if((*quality)->data.fl[posNbr] == PIX_UNWRAPPED)
                {
                    (*phOffset)->data.fl[posCur] = (*phOffset)->data.fl[posNbr] + UnwrapPix((*phase)->data.fl[posCur], (*phase)->data.fl[posNbr]);
                    (*quality)->data.fl[posCur] = PIX_UNWRAPPED;
                }
                else
                {
                    // Try to unwrap from second start facing pixel
                    posNbr = (r - stepY) * (*quality)->cols + c;
                    if((*quality)->data.fl[posNbr] == PIX_UNWRAPPED)
                    {
                        (*phOffset)->data.fl[posCur] = (*phOffset)->data.fl[posNbr] + UnwrapPix((*phase)->data.fl[posCur], (*phase)->data.fl[posNbr]);
                        (*quality)->data.fl[posCur] = PIX_UNWRAPPED;
                    }
                    else
                    {
                        // If it wasn't able to unwrap from any start facing pixel, check border facing pixels and
                        // if there's valid one, add current pixel to the leftovers
                        if((*quality)->data.fl[r * (*quality)->cols + (c + stepX)] != PIX_BACKGROUND
                                || (*quality)->data.fl[(r + stepY) * (*quality)->cols + c] != PIX_BACKGROUND)
                            remainders[numRem++] = posCur;
                    }
                }
            }
        }
    }
    
    // Now process remainder pixels that has been left over
    for(int i = numRem - 1; i >= 0; i--)
    {
        // Get current pixel's position
        posCur = remainders[i];

        // Try to unwrap from the first border facing pixel
        posNbr = posCur + stepX;
        if((*quality)->data.fl[posNbr] == PIX_UNWRAPPED)
        {
            (*phOffset)->data.fl[posCur] = (*phOffset)->data.fl[posNbr] + UnwrapPix((*phase)->data.fl[posCur], (*phase)->data.fl[posNbr]);
            (*quality)->data.fl[posCur] = PIX_UNWRAPPED;
        }
        else
        {
            // Try to unwrap from the second border facing pixel
            posNbr = posCur + ((*quality)->cols * stepY);
            if((*quality)->data.fl[posNbr] == PIX_UNWRAPPED)
            {
                (*phOffset)->data.fl[posCur] = (*phOffset)->data.fl[posNbr] + UnwrapPix((*phase)->data.fl[posCur], (*phase)->data.fl[posNbr]);
                (*quality)->data.fl[posCur] = PIX_UNWRAPPED;
            }
            else
            {
                continue;
            }
        }
    }
}

CvMat* FastBlur(CvMat* img, CvMat* process, int k)
{
    // Prepare some variables
    int i;
    int phWidth = img->cols;
    int phHeight = img->rows;
    CvMat *integral = cvCreateMat(phHeight, phWidth, CV_32SC1);
    CvMat *sum = cvCreateMat(phHeight, phWidth, CV_32SC1);

    // Left side
    for(int i = 0; i < (phWidth * phHeight); i += phWidth)
        integral->data.i[i] = img->data.ptr[i];
    // Top side
    for(int i = 0; i < phWidth; i++)
        integral->data.i[i] = img->data.ptr[i];
    // Start on second row
    i = phWidth;
    for(int r = 1; r < phHeight; r++)
    {
        // Skip left side
        i++;
        for(int c = 1; c < phWidth; c++)
        {
            integral->data.i[i] = img->data.ptr[i] + // Current
                    integral->data.i[i - 1] + // Everything to the left
                    integral->data.i[i - phWidth] - // Everything to the top
                    integral->data.i[i - phWidth - 1];  // The overlap
            // Next pixel
            i++;
        }
    }

    for(int r = k; r < phHeight - k; r++)
    {
        for(int c = k; c < phWidth - k; c++)
        {
            int i = r * phWidth + c;
            if(process->data.ptr[i])
            {
                sum->data.i[i] = integral->data.i[i + k + k * phWidth] + // Bottom right
                        integral->data.i[i - k - k * phWidth] - // Top left
                        integral->data.i[i - k + k * phWidth] - // Bottom left
                        integral->data.i[i + k - k * phWidth];  // Top right
            }
        }
    }

    //cvShowImage("Integral", integral);
    //cvShowImage("Sum", sum);
    //cvWaitKey(0);

    return sum;
}

CvPoint GetStart(CvMat *process)
{
    CvMat *sum = FastBlur(process, process, 64);
    int max = 0;
    for(int r = 0; r < sum->rows; r++)
    {
        for(int c = 0; c < sum->cols; c++)
            if(sum->data.i[r * sum->cols + c] > sum->data.i[max])
                max = r * sum->cols + c;
    }

    return cvPoint(max % sum->cols, max / sum->cols);
}

// Unwrapping phase images
CvPoint PhaseUnwrap(CvMat** phase, CvMat** process, CvMat** mask)
{
    // Create process queue
    T_PixelQueue procQueue;
    PQInit(&procQueue, 32000);

    // Get starting point using fast blur
    CvPoint start = GetStart((*process));
    //CvPoint start = cvPoint((*phase)->cols / 2, (*phase)->rows / 2 - 20);
    //CvPoint start = cvPoint((*phase)->cols / 2, (*phase)->rows / 2);
    CvPoint pos = start;
    // Unwrap the starting point
    UnwrapPixel(&procQueue, phase, process, mask, (*phase)->data.fl[pos.y * (*phase)->cols + pos.x], pos.y, pos.x);

    // Process items in the queue
    while(!PQIsEmpty(&procQueue))
    {
        // Get item from the queue
        if(PQRemove(&procQueue, &pos))
        {
            int pPos = pos.y * (*phase)->cols + pos.x;

            // Get next pixel phase
            float ph = (*phase)->data.fl[pPos];

            // Unwrap another pixels
            if(pos.y > 0)
                UnwrapPixel(&procQueue, phase, process, mask, ph, pos.y - 1, pos.x);
            if(pos.y < (*phase)->rows - 1)
                UnwrapPixel(&procQueue, phase, process, mask, ph, pos.y + 1, pos.x);
            if(pos.x > 0)
                UnwrapPixel(&procQueue, phase, process, mask, ph, pos.y, pos.x - 1);
            if(pos.x < (*phase)->cols - 1)
                UnwrapPixel(&procQueue, phase, process, mask, ph, pos.y, pos.x + 1);
        }
        else
            fprintf(stderr, "No more points in the queue!\n");
    }

    return start;
}


void UnwrapPixel(T_PixelQueue *pq, CvMat** phase, CvMat** process, CvMat **mask, float basePhase, int r, int c)
{
    // Unwrap the phase if it's expected to be unwrapped
    if((*process)->data.ptr[r * (*phase)->cols + c])
    {
        float phaseDiff = (*phase)->data.fl[r * (*phase)->cols + c] - (basePhase - floor(basePhase));
        if(phaseDiff > 0.5)
            phaseDiff -= 1.0;
        if(phaseDiff < -0.5)
            phaseDiff += 1.0;
        // Recompute new phase for this pixel
        (*phase)->data.fl[r * (*phase)->cols + c] = basePhase + phaseDiff;
        // Mark this pixel as already processed
        (*process)->data.ptr[r * (*phase)->cols + c] = 0;
        (*mask)->data.ptr[r * (*phase)->cols + c] = 0;
        // Add this pixel to the queue
        PQAdd(pq, cvPoint(c, r));
    }
}

// Make the depth from the unwrapped phase map
void MakeDepth(CvMat* phase, CvMat* mask, CvMat** depth, CvPoint startPt, float zskew, float zscale, bool bVertical)
{
    // For each point in the phase map
    int startInd = startPt.y * phase->cols + startPt.x;

    if(bVertical)
    {
        float planeZero = (float)(startInd % phase->cols) / (float)phase->cols;

        for(int r = 0; r < phase->rows; r++)
        {
            for(int c = 0; c < phase->cols; c++)
            {
                float x = (float)((r * phase->cols + c) % phase->cols);
                float planePhase = ((x / phase->cols) - planeZero) * zskew;
                
                if(!mask->data.ptr[r * phase->cols + c])
                    (*depth)->data.fl[r * phase->cols + c] =
                            (phase->data.fl[r * phase->cols + c] - planePhase) * zscale;
                else
                    (*depth)->data.fl[r * phase->cols + c] = 0.0;
            }
        }
    }
    else
    {
        float planeZero = (float)(startInd / phase->cols) / (float)phase->rows;
      
        for(int r = 0; r < phase->rows; r++)
        {
            for(int c = 0; c < phase->cols; c++)
            {
                float y = (float)((r * phase->cols + c) / phase->cols);
                float planePhase = ((y / phase->rows) - planeZero) * zskew;
                
                if(!mask->data.ptr[r * phase->cols + c])
                    (*depth)->data.fl[r * phase->cols + c] =
                            (phase->data.fl[r * phase->cols + c] - planePhase) * zscale;
                else
                    (*depth)->data.fl[r * phase->cols + c] = 0.0;
            }
        }
    }
}

// Make the depth from the unwrapped phase map
void MakeDepth2(CvMat* phase, CvMat* mask, CvMat** depth, float f1, float s, float d)
{
    // Compute coeficient k1
    float k1 = (-2 * MATH_PI * f1 * d) / s;

    // For each point in the phase map
    for(int r = 0; r < phase->rows; r++)
    {
        for(int c = 0; c < phase->cols; c++)
        {
            if(!mask->data.ptr[r * phase->cols + c])
                (*depth)->data.fl[r * phase->cols + c] =
                        phase->data.fl[r * phase->cols + c] / k1;
            else
                (*depth)->data.fl[r * phase->cols + c] = 0.0;
        }
    }
}


void MakeDepth3(CvMat* phase, CvMat* mask, CvMat** depth, float theta, float d0, float l0, float fringePitch)
{
    // Compute spatial frequency on reference phase f0
    float f0 = 1.0 / (fringePitch / cos(theta));
    f0 = fabs(f0);

    // Compute phase modulation for each x coordinate(for each column)
    // Allocate memory for phase modulations
    float *phaseMod = (float*)malloc(phase->cols * sizeof(float));
    for(int x = 0; x < phase->cols; x++)
        phaseMod[x] = -(2 * MATH_PI * f0 * x * x * sin(theta) * cos(theta)) / (l0 + x * sin(theta) * cos(theta));
 
    // For each point in the phase map, find phase-height correspondence(compute depth values)
    for(int r = 0; r < phase->rows; r++)
    {
        for(int c = 0; c < phase->cols; c++)
        {
            // Compute compensated phase
            float phaseComp = phase->data.fl[r * phase->cols + c] - phaseMod[c];

            // Compute depth only if the pixel is not masket out
           if(!mask->data.ptr[r * phase->cols + c])
                (*depth)->data.fl[r * phase->cols + c] = (l0 * phaseComp) / (2 * MATH_PI * d0 * f0 + phaseComp);
           else
                (*depth)->data.fl[r * phase->cols + c] = 0.0;
        }
    }

    // Release allocated memory
    free(phaseMod);
}

// Save phase map to file
int SavePhaseMap(char *fileName, CvMat *phase)
{
    // Opne a file for writing
    FILE* fOut = fopen(fileName, "w");
    if(fOut == NULL)
        return 1;

    // Go throught point values in the matrix and save them to file
    // one line = one point, line format is: xPos; yPos; zPos
    for(int r = 0; r < phase->rows; r++)
    {
        for(int c = 0; c < phase->cols; c++)
        {
            // When saving, convert integal x(c) and y(r) values to floating point ones
            fprintf(fOut, "%f;%f;%f\n", (float)c, (float)r, phase->data.fl[r * phase->cols + c]);
        }
    }

    // Close the file
    fclose(fOut);

    return 0;
}

// Save color map to file
int SaveColorMap(char *fileName, CvMat *colors)
{
    // Opne a file for writing
    FILE* fOut = fopen(fileName, "w");
    if(fOut == NULL)
        return 1;

    // Go throught point values in the matrix and save them to file
    // one line = one point, line format is: xPos; yPos; zPos
    for(int r = 0; r < colors->rows; r++)
    {
        for(int c = 0; c < colors->cols; c++)
        {
            // When saving, convert integal x(c) and y(r) values to floating point ones
            fprintf(fOut, "%f;%f;%f\n", colors->data.fl[r * colors->cols + c * 3], colors->data.fl[r * colors->cols + c * 3 + 1],
                    colors->data.fl[r * colors->cols + c * 3 + 2]);
        }
    }

    // Close the file
    fclose(fOut);

    return 0;
}

int SaveMask(char *fileName, CvMat *mask)
{
    // Open a file for writing
    FILE* fOut = fopen(fileName, "w");
    if(fOut == NULL)
        return 1;

    // Go through mask values and save them
    // one line = one point mask data, line format is: bMasked
    for(int r = 0; r < mask->rows; r++)
    {
        for(int c = 0; c < mask->cols; c++)
        {
            // When saving, convert integal x(c) and y(r) values to floating point ones
            fprintf(fOut, "%d\n", (int)mask->data.ptr[r * mask->cols + c]);
        }
    }

    // Close the file
    fclose(fOut);

    return 0;
}

bool QSortFLArray(float *array, int left, int right, bool bAscending)
{
    float median;
    float tmp;
    int i, j = 0;

    if(array == NULL || left > right)
        return false;

    i = left;
    j = right;
    median = array[(i + j) / 2];
    do
    {
        //If it should be sorted ascending
        if(bAscending)
        {
            while(array[i] < median)
                i++;
            while(array[j] > median)
                j--;
        }
        //Sort descending is only other option
        else
        {
            while(array[i] > median)
                i++;
            while(array[j] < median)
                j--;
        }

        //Change found elements
        if(i <= j)
        {
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            i++;
            j--;
        }
    }
    while(i <= j);
    //Call quicksorts recursively on each new part of the array
    if(left < j)
        QSortFLArray(array, left, j, bAscending);
    if(i < right)
        QSortFLArray(array, i, right, bAscending);

    return true;
}

void PhaseRemNoise(CvMat **phase, CvMat *mask)
{
    // Storage array for current 3 x 3 matrix
    float neighbours[9] = {0.0f};
    // For indexing of neighbours array
    int index = 0;

    // Create new temporary matrix for storing smoothed vertices
    CvMat *smoothPh = cvCreateMat((*phase)->rows, (*phase)->cols, CV_32FC1);
    cvCopy((*phase), smoothPh);

    // Apply smooth procedure on each pixel in the scene
    for(int r = 1; r < (*phase)->rows - 1; r++)
    {
        for(int c = 1; c < (*phase)->cols - 1; c++)
        {
            // Fill neighbours array
            index = 0;
            for(int y = -1; y <= 1; y++)
            {
                for(int x = -1; x <= 1; x++)
                {
                    if(!mask->data.ptr[(r + y) * (*phase)->cols + (c + x)])
                        neighbours[index++] = (*phase)->data.fl[(r + y) * (*phase)->cols + (c + x)];
                }                    
            }

            // Sort the 3 x 3 matrix array
            QSortFLArray(neighbours, 0, index - 1, true);
            // Choose sorted array median as a new vertex phase value
            if(index > 0)
                smoothPh->data.fl[r * smoothPh->cols + c] = neighbours[index / 2];
        }
    }    

    // Rewrite original phase with the smoothed one
    cvCopy(smoothPh, (*phase));

    // Release resulting matrix temporary storage
    cvReleaseMat(&smoothPh);
}

