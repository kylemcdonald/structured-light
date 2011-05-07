#include "RExporter.h"

/*** PLY format ***/
int GetNumVertices(CvMat* mask)
{
    int numVerts = 0;

    for(int r = 0; r < mask->rows; r++)
    {
        for(int c = 0; c < mask->cols; c++)
        {
            if(!mask->data.ptr[r * mask->cols + c])
                numVerts++;
        }
    }
    
    return numVerts;
}

bool ExportCloudToPly(char *fileName, CvMat* mask, CvMat* depth, CvMat *colors)
{
    // Prepare output file
    FILE *fOut = fopen(fileName, "w");
    if(fOut == NULL)
        return false;

    // Write PLY file header
    fprintf(fOut, "ply\n");
    fprintf(fOut, "format ascii 1.0\n");
    fprintf(fOut, "element vertex %d\n", GetNumVertices(mask));
    fprintf(fOut, "property float x\n");
    fprintf(fOut, "property float y\n");
    fprintf(fOut, "property float z\n");
    if(colors != NULL)
    {
        fprintf(fOut, "property uchar red\n");
        fprintf(fOut, "property uchar green\n");
        fprintf(fOut, "property uchar blue\n");
    }
    fprintf(fOut, "end_header\n");
    // Write PLY vertices and colors
    for(int r = 0; r < depth->rows; r++)
    {
        for(int c = 0; c < depth->cols; c++)
        {
            if(!mask->data.ptr[r * depth->cols + c])
            {
                // Write vertex data
                fprintf(fOut, "%f %f %f ", (float)c, -(float)r, -depth->data.fl[r * depth->cols + c]);
                // Write color data
                if(colors != NULL)
                    fprintf(fOut, "%d %d %d\n", (int)(colors->data.fl[r * depth->cols + c * 3] * 255),
                        (int)(colors->data.fl[r * depth->cols + c * 3 + 1] * 255),
                        (int)(colors->data.fl[r * depth->cols + c * 3 + 2] * 255));
            }
        }
    }

    // Close the output file
    fclose(fOut);

    return true;
}
/*** ********** ***/

void GetFaceIndices(unsigned int** indices, CvMat* mask)
{
    int numVerts = 0;

    for(int r = 0; r < mask->rows; r++)
    {
        for(int c = 0; c < mask->cols; c++)
        {
            if(!mask->data.ptr[r * mask->cols + c])
                (*indices)[r * mask->cols + c] = numVerts++;
        }
    }
}

int GetNumFaces(CvMat* mask)
{
    int numFaces = 0;

    for(int y = 0; y < mask->rows - 1; y++)
    {
        for(int x = 0; x < mask->cols - 1; x++)
        {
            int nw = y * mask->cols + x;
            int ne = nw + 1;
            int sw = nw + mask->cols;
            int se = ne + mask->cols;
            if(!mask->data.ptr[nw] && !mask->data.ptr[se])
            {
                if(!mask->data.ptr[ne])
                {
                    // Triangle NW, NE, SE
                    numFaces++;
                }
                if(!mask->data.ptr[sw])
                {
                    // Triangle NW, SE, Sw
                    numFaces++;
                }
            }
            else if(!mask->data.ptr[ne]&& !mask->data.ptr[sw])
            {
                if(!mask->data.ptr[nw])
                {
                    // Triangle NW, NE, SW
                    numFaces++;
                }
                if(!mask->data.ptr[se])
                {
                    // Triangle NE, SE, SW
                    numFaces++;
                }
            }
        }
    }

    return numFaces;
}

bool ExportMeshToPly(char *fileName, CvMat* mask, CvMat* depth, CvMat *colors)
{
    // Prepare output file
    FILE *fOut = fopen(fileName, "w");
    if(fOut == NULL)
        return false;

    // Array for indices
    unsigned int* indices = (unsigned int*)malloc(mask->rows * mask->cols * sizeof(unsigned int));
    if(indices == NULL)
        return false;

    // Get face indices
    GetFaceIndices(&indices, mask);
        
    // Write PLY file header
    fprintf(fOut, "ply\n");
    fprintf(fOut, "format ascii 1.0\n");
    fprintf(fOut, "element vertex %d\n", GetNumVertices(mask));
    fprintf(fOut, "property float x\n");
    fprintf(fOut, "property float y\n");
    fprintf(fOut, "property float z\n");
    if(colors != NULL)
    {
        fprintf(fOut, "property uchar red\n");
        fprintf(fOut, "property uchar green\n");
        fprintf(fOut, "property uchar blue\n");
    }
    fprintf(fOut, "element face %d\n", GetNumFaces(mask));
    fprintf(fOut, "property list uchar uint vertex_indices\n");
    fprintf(fOut, "end_header\n");
    // Write PLY vertices and colors
    for(int r = 0; r < depth->rows; r++)
    {
        for(int c = 0; c < depth->cols; c++)
        {
            if(!mask->data.ptr[r * depth->cols + c])
            {
                // Write vertex data
                fprintf(fOut, "%f %f %f ", (float)c, -(float)r, -depth->data.fl[r * depth->cols + c]);
                // Write color data
                if(colors != NULL)
                    fprintf(fOut, "%d %d %d\n", (int)(colors->data.fl[r * depth->cols + c * 3] * 255),
                        (int)(colors->data.fl[r * depth->cols + c * 3 + 1] * 255),
                        (int)(colors->data.fl[r * depth->cols + c * 3 + 2] * 255));
            }
        }
    }
    // Write PLY faces
    for(int y = 0; y < mask->rows - 1; y++)
    {
        for(int x = 0; x < mask->cols - 1; x++)
        {
            int nw = y * mask->cols + x;
            int ne = nw + 1;
            int sw = nw + mask->cols;
            int se = ne + mask->cols;
            if(!mask->data.ptr[nw] && !mask->data.ptr[se])
            {
                if(!mask->data.ptr[ne])
                {
                    // Triangle NW, NE, SE
                    fprintf(fOut, "3 %d %d %d\n", indices[se], indices[ne], indices[nw]);
                }
                if(!mask->data.ptr[sw])
                {
                    // Triangle NW, SE, Sw
                    fprintf(fOut, "3 %d %d %d\n", indices[sw], indices[se], indices[nw]);
                }
            }
            else if(!mask->data.ptr[ne]&& !mask->data.ptr[sw])
            {
                if(!mask->data.ptr[nw])
                {
                    // Triangle NW, NE, SW
                    fprintf(fOut, "3 %d %d %d\n", indices[sw], indices[ne], indices[nw]);
                }
                if(!mask->data.ptr[se])
                {
                    // Triangle NE, SE, SW
                    fprintf(fOut, "3 %d %d %d\n", indices[sw], indices[se], indices[ne]);
                }
            }
        }
    }

    // Close the output file
    fclose(fOut);

    free(indices);

    return true;
}