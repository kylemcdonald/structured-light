#include "RCalibUtil.h"

int GenerateProjChboard(T_CalibInput* cInput, IplImage* chBoard, int* borderCols, int* borderRows)
{
    // Calculate chessboard border.
    *borderCols = (int)floor((chBoard->width -(cInput->projBoardWidth + 1) * cInput->projBoardSqPxW) / 2.0);
    *borderRows = (int)floor((chBoard->height-(cInput->projBoardHeight + 1) * cInput->projBoardSqPxH) / 2.0);

    // Check for chessboard errors.
    if((*borderCols < 0) || (*borderRows < 0))
    {
        printf("ERROR: Cannot create chessboard with user-requested dimensions!\n");
        return -1;
    }

    // Initialize chessboard with white image.
    cvSet(chBoard, cvScalarAll(255), NULL);

    // Create odd black squares.
    uchar* data = (uchar*)chBoard->imageData;
    int step = chBoard->widthStep / sizeof(uchar);
    for(int r = 0; r < (cInput->projBoardHeight + 1); r += 2)
	for(int c = 0; c < (cInput->projBoardWidth + 1); c += 2)
            for(int i = (r * cInput->projBoardSqPxH + *borderRows);
		i < ((r + 1) * cInput->projBoardSqPxH + *borderRows); i++)
		for(int j = (c * cInput->projBoardSqPxW + *borderCols);
                    j < ((c + 1) * cInput->projBoardSqPxW + *borderCols); j++)
                    data[i * step + j] = 0;

	// Create even black squares.
	for(int r = 1; r < cInput->projBoardHeight; r += 2)
            for(int c = 1; c < cInput->projBoardWidth; c += 2)
		for(int i = (r * cInput->projBoardSqPxH + *borderRows);
                    i < ((r + 1) * cInput->projBoardSqPxH + *borderRows); i++)
                    for(int j = (c * cInput->projBoardSqPxW + *borderCols);
			j < ((c + 1) * cInput->projBoardSqPxW + *borderCols); j++)
			data[i * step + j] = 0;

	// Return without errors.
	return 0;
}

int GenerateProjChboardCol(T_CalibInput* cInput, IplImage* chBoard, int* borderCols, int* borderRows, short chR, short chB, short chG)
{
    // Calculate chessboard border.
    *borderCols = (int)floor((chBoard->width -(cInput->projBoardWidth + 1) * cInput->projBoardSqPxW) / 2.0);
    *borderRows = (int)floor((chBoard->height-(cInput->projBoardHeight + 1) * cInput->projBoardSqPxH) / 2.0);

    // Check for chessboard errors.
    if((*borderCols < 0) || (*borderRows < 0))
    {
        printf("ERROR: Cannot create chessboard with user-requested dimensions!\n");
        return -1;
    }

    // Initialize chessboard with white image.
    cvSet(chBoard, cvScalarAll(255), NULL);
    printf("%d\n", chBoard->dataOrder);
    // Create odd black squares.
    uchar* data = (uchar*)chBoard->imageData;
    int step = chBoard->widthStep / sizeof(uchar);

    for(int r = 0; r < (cInput->projBoardHeight + 1); r += 2)
	for(int c = 0; c < (cInput->projBoardWidth + 1); c += 2)
            for(int i = (r * cInput->projBoardSqPxH + *borderRows);
		i < ((r + 1) * cInput->projBoardSqPxH + *borderRows); i++)
		for(int j = (c * cInput->projBoardSqPxW + *borderCols);
                    j < ((c + 1) * cInput->projBoardSqPxW + *borderCols); j++)
                    {
                        // B channel
                        data[i * step + j * 3] = chB;
                        // G channel
                        data[i * step + j * 3 + 1] = chG;
                        // R channel
                        data[i * step + j * 3 + 2] = chR;
                   }

	// Create even black squares.
	for(int r = 1; r < cInput->projBoardHeight; r += 2)
            for(int c = 1; c < cInput->projBoardWidth; c += 2)
		for(int i = (r * cInput->projBoardSqPxH + *borderRows);
                    i < ((r + 1) * cInput->projBoardSqPxH + *borderRows); i++)
                    for(int j = (c * cInput->projBoardSqPxW + *borderCols);
			j < ((c + 1) * cInput->projBoardSqPxW + *borderCols); j++)
                    {
                        // B channel
                        data[i * step + j * 3] = chB;
                        // G channel
                        data[i * step + j * 3 + 1] = chG;
                        // R channel
                        data[i * step + j * 3 + 2] = chR;
                    }
    // Return without errors.
    return 0;
}


int CalibrateCamera(CvCapture *capture, RImageWindow *camWnd, T_CalibInput *cInput, T_CalibOutput *cOutput)
{
    // Set camera window to display it's label
    camWnd->SetDrawLabel(true);

    // Some additional variables
    int board_n = cInput->camBoardWidth * cInput->camBoardHeight;
    CvSize board_sz = cvSize(cInput->camBoardWidth, cInput->camBoardHeight);

    // Allocate storage
    CvMat* image_points = cvCreateMat(cInput->numberBoards * board_n, 2, CV_32FC1);
    CvMat* object_points = cvCreateMat(cInput->numberBoards * board_n, 3, CV_32FC1);
    CvMat* point_counts = cvCreateMat(cInput->numberBoards, 1, CV_32SC1);
    cOutput->camIntrinsic = cvCreateMat(3, 3, CV_32FC1);
    cOutput->camDistortion = cvCreateMat(5, 1, CV_32FC1);

    // Prepare some other variables
    CvPoint2D32f* corners = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f) * board_n);  // Board corners

    int corner_count;
    int successes = 0;
    int step, frame = 0;

    // Get image from the camera capture
    IplImage *image = cvQueryFrame(capture);
    IplImage *gray_image = cvCreateImage(cvGetSize(image), 8, 1);   // Subpixel

    // Capture corner views loop until we've got n_boards
    // Successful captures = all corners on the board are found
    while(successes < cInput->numberBoards)
    {
        // Skip every cInput->camSnapshotRate frames to allow user to move chessboard
        if(frame++ % 5 == 0)
        {
            // Find chessboard corners
            int found = cvFindChessboardCorners(image, board_sz, corners, &corner_count,
                    CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

            // Get subpixel accuracy on those corners
            cvCvtColor(image, gray_image, CV_BGR2GRAY);
            cvFindCornerSubPix(gray_image, corners, corner_count,
                    cvSize(11,11), cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS
                    + CV_TERMCRIT_ITER, 30, 0.1));

            // Draw chessboard corners
            cvDrawChessboardCorners(image, board_sz, corners, corner_count, found);
            //cvShowImage("Calibration", image);
            camWnd->ShowPattern(image);

            // If we got a good board, add it to our data
            if(corner_count == board_n)
            {
                step = successes * board_n;
                for(int i = step, j = 0; j < board_n; ++i, ++j)
                {
                    CV_MAT_ELEM(*image_points, float, i, 0) = corners[j].x;
                    CV_MAT_ELEM(*image_points, float, i, 1) = corners[j].y;
                    CV_MAT_ELEM(*object_points, float, i, 0) = cInput->camBoardSqMmW * (float)(j / cInput->camBoardWidth);
                    CV_MAT_ELEM(*object_points, float, i, 1) = cInput->camBoardSqMmH * (float)(j % cInput->camBoardHeight);
                    CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
                }
                CV_MAT_ELEM(*point_counts, int, successes, 0) = board_n;
                successes++;
                printf("+ Captured frame %d of %d.\n", successes, cInput->numberBoards);
                camWnd->SetLabelText(wxString::Format("+ Captured frame %d of %d.\n", successes, cInput->numberBoards));
            }
        } // End skip cInput->camSnapshotRate between chessboard capture

        // Handling Pause/Unpause and ESC
        int c = cvWaitKey(15);
        if(c == 'p')
        {
            c = 0;
            while(c != 'p' && c != 27)
                c = cvWaitKey(250);
        }
        if(c == 27)
            return 0;

        // Get next image from camera capture
        image = cvQueryFrame(capture);
    }  // End of capture collection while loop

    // Allocate matrices according to how many chessboards found
    CvMat* object_points2 = cvCreateMat(successes * board_n, 3, CV_32FC1);
    CvMat* image_points2 = cvCreateMat(successes * board_n, 2, CV_32FC1);
    CvMat* point_counts2 = cvCreateMat(successes, 1, CV_32SC1);
    // Transfer the points into the correct size matrices
    // Instead have written:
    // image_points->rows = object_points->rows = successes * board_n;
    // point_counts->rows = successes;
    for(int i = 0; i < successes * board_n; ++i)
    {
        CV_MAT_ELEM(*image_points2, float, i, 0) = CV_MAT_ELEM(*image_points, float, i, 0);
        CV_MAT_ELEM(*image_points2, float, i, 1) = CV_MAT_ELEM(*image_points, float, i, 1);
        CV_MAT_ELEM(*object_points2, float, i, 0) = CV_MAT_ELEM(*object_points, float, i, 0);
        CV_MAT_ELEM(*object_points2, float, i, 1) = CV_MAT_ELEM(*object_points, float, i, 1);
        CV_MAT_ELEM(*object_points2, float, i, 2) = CV_MAT_ELEM(*object_points, float, i, 2);
    }
    // These are all the same number
    for(int i = 0; i < successes; ++i)
        CV_MAT_ELEM(*point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);

    // Free memory allocated for old matrices
    cvReleaseMat(&object_points);
    cvReleaseMat(&image_points);
    cvReleaseMat(&point_counts);

    // At this point we have all of the chessboard corners we need
    // Initialize the intrinsic matrix such taht the two focal lengths have a ratio of 1.0
    CV_MAT_ELEM(*cOutput->camIntrinsic, float, 0, 0) = 1.0f;
    CV_MAT_ELEM(*cOutput->camIntrinsic, float, 1, 1) = 1.0f;

    // Calibrate the camera
    cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(image),
            cOutput->camIntrinsic, cOutput->camDistortion, NULL, NULL, 0);

    // Save the intrinsics and distortions
    cvSave("calib/camIntrinsics.xml", cOutput->camIntrinsic, NULL, NULL, cvAttrList(0, 0));
    cvSave("calib/camDistortion.xml", cOutput->camDistortion, NULL, NULL, cvAttrList(0, 0));

    printf("Calibration DONE!\n");

    /* Loading the matrices back in
    CvMat *intrinsic = (CvMat*)cvLoad("intrinsics.xml");
    CvMat *distortion = (CvMat*)cvLoad("distortion.xml");
    */

    /*
    // Build the undistort map that we will use for all subsequent frames
    IplImage* mapx = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
    IplImage* mapy = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
    cvInitUndistortMap(cOutput->camIntrinsic, cOutput->camDistortion, mapx, mapy);

    // Just run the camera to the screen, now showing the raw and the undistorted image
    cvNamedWindow("Undistort", CV_WINDOW_AUTOSIZE);
    while(image)
    {
        IplImage *t = cvCloneImage(image);
        cvShowImage("Calibration", image);  // Show raw image
        cvRemap(t, image, mapx, mapy, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));  // Undistort image
        cvReleaseImage(&t);
        cvShowImage("Undistort", image);    // Show corrected image

        // Handling Pause/Unpause and ESC
        int c = cvWaitKey(15);
        if(c == 'p')
        {
            c = 0;
            while(c != 'p' && c != 27)
                c = cvWaitKey(250);
        }
        if(c == 27)
            break;

        // Get next image from camera capture
        image = cvQueryFrame(capture);
    }
    */

    // Free memory allocated for old matrices
    cvReleaseMat(&object_points2);
    cvReleaseMat(&image_points2);
    cvReleaseMat(&point_counts2);
    free(corners);

    return 0;
}

int CalibrateCamProjSyst(CvCapture *capture, RImageWindow *camWnd, RImageWindow *projWnd, T_CalibInput *cInput, T_CalibOutput *cOutput)
{
    camWnd->SetDrawLabel(true);

    // Evaluate derived camera paramters and allocate storage
    int camBoardN = cInput->camBoardWidth * cInput->camBoardHeight;
    CvSize camBoardSize = cvSize(cInput->camBoardWidth, cInput->camBoardHeight);
    CvMat *camImagePoints = cvCreateMat(cInput->numberBoards * camBoardN, 2, CV_32FC1);
    CvMat *camObjectPoints = cvCreateMat(cInput->numberBoards * camBoardN, 3, CV_32FC1);
    CvMat *camPointCounts = cvCreateMat(cInput->numberBoards, 1, CV_32SC1);
    IplImage **camCalibImages = (IplImage**)malloc(sizeof(IplImage*) * cInput->numberBoards);

    // Evaluate derived projector parameters and allocate storage
    int projBoardN = cInput->projBoardWidth * cInput->projBoardHeight;
    CvSize projBoardSize = cvSize(cInput->projBoardWidth, cInput->projBoardHeight);
    CvMat *projImagePoints = cvCreateMat(cInput->numberBoards * projBoardN, 2, CV_32FC1);
    CvMat *projPointCounts = cvCreateMat(cInput->numberBoards, 1, CV_32SC1);
    IplImage **projCalibImages = (IplImage**)malloc(sizeof(IplImage*) * cInput->numberBoards);

    // Generate projector chessboard
    IplImage *projChessboard = cvCreateImage(cvSize(cInput->projResWidth, cInput->projResHeight), IPL_DEPTH_8U, 3);
    int projBorderCols, projBorderRows;
    if(GenerateProjChboardCol(cInput, projChessboard, &projBorderCols, &projBorderRows, 0, 255, 0) == -1)
    {
        printf("Cannot generate projector chessboard. Projector-camera calibration failed!");
        return -1;
    }

    // Initialize capture and allocate storage
    IplImage* camFrame = cvQueryFrame(capture);
    IplImage* camFrame1 = cvCreateImage(cvGetSize(camFrame), camFrame->depth, camFrame->nChannels);
    IplImage* camFrame2 = cvCreateImage(cvGetSize(camFrame), camFrame->depth, camFrame->nChannels);
    IplImage* camFrame3 = cvCreateImage(cvGetSize(camFrame), camFrame->depth, camFrame->nChannels);
    for(int i = 0; i < cInput->numberBoards; i++)
        camCalibImages[i] = cvCreateImage(cvGetSize(camFrame), camFrame->depth, camFrame->nChannels);
    for(int i = 0; i < cInput->numberBoards; i++)
        projCalibImages[i] = cvCreateImage(cvGetSize(camFrame), camFrame->depth, camFrame->nChannels);

    // Create a window to display capture frames
    //cvNamedWindow("CameraWindow", CV_WINDOW_AUTOSIZE);

    // Create a window to display projector images
    IplImage* projFrame = cvCreateImage(cvSize(projChessboard->width, projChessboard->height), IPL_DEPTH_8U, 3);
    cvSet(projFrame, cvScalarAll(255), NULL);
    //cvScale(projFrame, projFrame, 2.0 * cInput->projGain / 100.0, 0);
    //cvShowImage("ProjWindow", projFrame);
    projWnd->ShowPattern(projFrame);

    // Allocate storage for grayscale images
    IplImage *camFrame1Gray = cvCreateImage(cvGetSize(camFrame), IPL_DEPTH_8U, 1);
    IplImage *camFrame2Gray = cvCreateImage(cvGetSize(camFrame), IPL_DEPTH_8U, 1);

    // Prepare for calibration and press any key
    //printf("Prepare for calibration and press any key to start.\n");
    cvWaitKey(0);

    // Capture live image stream until "ESC" is pressed or calibration is completed
    int framesSkipped = 0;
    int successes = 0;
    short captureFrame = 1;
    int cvKey = -1;
    int cvKeyTmp = -1;
    while(successes < cInput->numberBoards)
    {
        framesSkipped++;
        // Get next available frame
        cvQueryFrame(capture);      // Makes another frame "safe" captured
        camFrame = cvQueryFrame(capture);
        //cvScale(camFrame, camFrame, 2.0 * cInput->camGain / 100.0, 0);
        cvCopyImage(camFrame, camFrame1);

        // Find camera chessboard corners
        CvPoint2D32f* camCorners = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f) * camBoardN);
        int camCornerCount;
        int camFound = cvFindChessboardCorners(camFrame, camBoardSize, camCorners, &camCornerCount,
                CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        // Refine chessboard corners
        IplImage* grayFrame = cvCreateImage(cvGetSize(camFrame), camFrame->depth, 1);
        if(camFrame->nChannels > 1)
            cvCvtColor(camFrame, grayFrame, CV_BGR2GRAY);
        else
            cvCopyImage(camFrame, grayFrame);
        // Find corners more accurately
        cvFindCornerSubPix(grayFrame, camCorners, camCornerCount, cvSize(11, 11), cvSize(-1, -1),
                cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        // Release resources
        cvReleaseImage(&grayFrame);

        // Camera chessboard not found, display current camera tracking results
        //cvDrawChessboardCorners(camFrame1, camBoardSize, camCorners, camCornerCount, camFound);
        //cvShowImage("CameraWindow", camFrame1);

        // If camera chessboard was found, attempt to detect projector chessboard
        if(camCornerCount == camBoardN)
        {
            // Dispaly projector chessboard
            cvCopy(projChessboard, projFrame, NULL);
            //cvScale(projFrame, projFrame, 2.0 * cInput->projGain / 100.0, 0);
            //cvShowImage("ProjWindow", projFrame);
            projWnd->ShowPattern(projFrame);

            // Get next available frame (after some delay)
            /*
            cvKeyTmp = cvWaitKey(cInput->frameDelay);
            if(cvKeyTmp != -1)
                cvKey = cvKeyTmp;
            */
            
            // Make safe frame capture
            cvQueryFrame(capture);
            camFrame = cvQueryFrame(capture);
            //cvScale(camFrame, camFrame, 2.0 * cInput->camGain / 100.0, 0);
            cvCopyImage(camFrame, camFrame2);
            cvCopyImage(camFrame, camFrame3);

            // Convert frames to grayscale and apply background subtraction
            cvCvtColor(camFrame1, camFrame1Gray, CV_BGR2GRAY);
            cvCvtColor(camFrame2, camFrame2Gray, CV_BGR2GRAY);
            cvSub(camFrame1Gray, camFrame2Gray, camFrame2Gray, NULL);
            // Invert chessboard image
            double minVal, maxVal;
            cvMinMaxLoc(camFrame2Gray, &minVal, &maxVal, NULL, NULL, NULL);
            cvSubRS(camFrame2Gray, cvScalarAll(maxVal), camFrame2Gray, NULL);
            //cvSaveImage("resImg.png", camFrame2Gray, NULL);

            // Find projector chessboard corners
            CvPoint2D32f* projCorners = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f) * projBoardN);
            int projCornerCount;
            int projFound = cvFindChessboardCorners(camFrame2Gray, projBoardSize, projCorners, &projCornerCount,
                CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

            // Display current projector tracking results
            cvDrawChessboardCorners(camFrame3, projBoardSize, projCorners, projCornerCount, projFound);
            //cvScale(projFrame, projFrame, 2.0 * cInput->projGain / 100.0, 0);
            // cvShowImage("CameraWindow", camFrame3);
            camWnd->ShowPattern(camFrame3);

            // If chessboard is detected, then update calibration lists
            if(captureFrame & (projCornerCount == projBoardN))
            {
                // Add camera calibration data
                for(int i = successes * camBoardN, j = 0; j < camBoardN; ++i, ++j)
                {
                    CV_MAT_ELEM(*camImagePoints,  float, i, 0) = camCorners[j].x;
                    CV_MAT_ELEM(*camImagePoints,  float, i, 1) = camCorners[j].y;
                    CV_MAT_ELEM(*camObjectPoints, float, i, 0) = cInput->camBoardSqMmW * (float)(j / cInput->camBoardWidth);
                    CV_MAT_ELEM(*camObjectPoints, float, i, 1) = cInput->camBoardSqMmH * (float)(j % cInput->camBoardHeight);
                    CV_MAT_ELEM(*camObjectPoints, float, i, 2) = 0.0f;
                }
                CV_MAT_ELEM(*camPointCounts, int, successes, 0) = camBoardN;
                cvCopyImage(camFrame1, camCalibImages[successes]);

                // Add projector calibration data
                for(int i = successes * projBoardN, j = 0; j < projBoardN; ++i, ++j)
                {
                    CV_MAT_ELEM(*projImagePoints, float, i, 0) = projCorners[j].x;
                    CV_MAT_ELEM(*projImagePoints, float, i, 1) = projCorners[j].y;
                }
                CV_MAT_ELEM(*projPointCounts, int, successes, 0) = projBoardN;
                cvCopyImage(camFrame2, projCalibImages[successes]);

                // Update display
                successes++;
                printf("+ Captured frame %d of %d.\n", successes, cInput->numberBoards);
                camWnd->SetLabelText(wxString::Format("+ Captured frame %d of %d.\n", successes, cInput->numberBoards));
                captureFrame = 0;

                // Black screen for a sec indicates that it was captured
                // Display black image for next camera capture
                cvSet(projFrame, cvScalarAll(0), NULL);
                //cvScale(projFrame, projFrame, 2.0 * cInput->projGain / 100.0, 0);
                //cvShowImage("ProjWindow", projFrame);
                projWnd->ShowPattern(projFrame);
                cvKeyTmp = cvWaitKey(cInput->frameDelay);
                if(cvKeyTmp != -1)
                    cvKey = cvKeyTmp;
            }
            // Free allocated resources
            free(projCorners);

            // Display white image for next camera capture
            cvSet(projFrame, cvScalarAll(255), NULL);
            //cvScale(projFrame, projFrame, 2.0 * cInput->projGain / 100.0, 0);
            //cvShowImage("ProjWindow", projFrame);
            projWnd->ShowPattern(projFrame);
            //cvKeyTmp = cvWaitKey(cInput->frameDelay);
            //if(cvKeyTmp != -1)
            //    cvKey = cvKeyTmp;
        }
        else
        {
            // Camera chessboard not found, display current camera tracking results
            cvDrawChessboardCorners(camFrame1, camBoardSize, camCorners, camCornerCount, camFound);
            //cvShowImage("CameraWindow", camFrame1);
            camWnd->ShowPattern(camFrame);

            // Display white image for next camera capture frame
            cvSet(projFrame, cvScalarAll(255), NULL);
            //cvShowImage("ProjWindow", projFrame);
            projWnd->ShowPattern(projFrame);
            //cvKeyTmp = cvWaitKey(1);
            //if(cvKeyTmp != -1)
            //    cvKey = cvKeyTmp;
        }


        // Free allocated memory
        free(camCorners);

        // Each 8th frame will be captured
        if(framesSkipped >= 6)
        {
            captureFrame = 1;
            framesSkipped = 0;
        }

        /*
        // Process user input
        cvKeyTmp = cvWaitKey(10);
        if(cvKeyTmp != -1)
            cvKey = cvKeyTmp;
        if(cvKey == 27)
            break;
        //else if(cvKey == 'n')
        //    captureFrame = 1;
        //captureFrame = 1;
        cvKeyTmp = -1;
        cvKey = -1;
        */
    }

    // Close the display window
    //cvDestroyWindow("CamWindow");

    printf("Data grabbed. Preparing for calibration!");

    // Calibrate projectro, if minimum of frames are available
    if(successes >= 2)
    {
        // Allocate calibration matrices
        CvMat* camObjectPoints2 = cvCreateMat(successes * camBoardN, 3, CV_32FC1);
        CvMat* camImagePoints2 = cvCreateMat(successes * camBoardN, 2, CV_32FC1);
        CvMat* camPointCounts2 = cvCreateMat(successes, 1, CV_32SC1);
        CvMat* camRotationVectors = cvCreateMat(successes, 3, CV_32FC1);
        CvMat* camTranslationVectors = cvCreateMat(successes, 3, CV_32FC1);
        CvMat* projObjectPoints2 = cvCreateMat(successes * projBoardN, 3, CV_32FC1);
        CvMat* projImagePoints2 = cvCreateMat(successes * projBoardN, 2, CV_32FC1);
        CvMat* projPointCounts2 = cvCreateMat(successes, 1, CV_32SC1);
        CvMat* projRotationVectors = cvCreateMat(successes, 3, CV_32FC1);
        CvMat* projTranslationVectors = cvCreateMat(successes, 3, CV_32FC1);

        // Transfer camera calibration data form capture values
        for(int i = 0; i < successes * camBoardN; ++i)
        {
            CV_MAT_ELEM(*camImagePoints2, float, i, 0) = CV_MAT_ELEM(*camImagePoints, float, i, 0);
            CV_MAT_ELEM(*camImagePoints2, float, i, 1) = CV_MAT_ELEM(*camImagePoints, float, i, 1);
            CV_MAT_ELEM(*camObjectPoints2, float, i, 0) = CV_MAT_ELEM(*camObjectPoints, float, i, 0);
            CV_MAT_ELEM(*camObjectPoints2, float, i, 1) = CV_MAT_ELEM(*camObjectPoints, float, i, 1);
            CV_MAT_ELEM(*camObjectPoints2, float, i, 2) = CV_MAT_ELEM(*camObjectPoints, float, i, 2);
        }
        for(int i = 0; i < successes; ++i)
            CV_MAT_ELEM(*camPointCounts2, int, i, 0) = CV_MAT_ELEM(*camPointCounts, int, i, 0);

        // Calibrate the camera and save calibration parameters
        printf("Calibrating camera...\n");
        int calibFlags = 0;
        calibFlags |= CV_CALIB_ZERO_TANGENT_DIST;
        //cvmSet(cOutput->camDistortion, 4, 0, 0);
        //calibFlags |= CV_CALIB_FIX_K3;
        cvCalibrateCamera2(camObjectPoints2, camImagePoints2, camPointCounts2,
                cvSize(cInput->camResWidth, cInput->camResHeight), cOutput->camIntrinsic,
                cOutput->camDistortion, camRotationVectors, camTranslationVectors, calibFlags);
        printf("Saving calibration images and parameters...\n");
        // Save camera calibration data
        char fName[128];
        ///*
        for(int i = 0; i < successes; ++i)
        {
            sprintf(fName, "calib/calibImg%d.png", i);
            cvSaveImage((char*)fName, camCalibImages[i], NULL);
        }
        //
        cvSave("calib/camIntrinsic.xml", cOutput->camIntrinsic, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/camDistortion.xml", cOutput->camDistortion, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/camRotVectors.xml", camRotationVectors, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/camTransVectors.xml", camTranslationVectors, NULL, NULL, cvAttrList(0, 0));

        // Transfer projector calibration data from captured values
        for(int i = 0; i < successes; ++i)
        {
            // Define image points corresponding to projector chessboard
            for(int j = 0; j < projBoardN; ++j)
            {
                CV_MAT_ELEM(*projImagePoints2, float, projBoardN * i + j, 0) =
                        cInput->projBoardSqPxW * (float)(j % cInput->projBoardWidth) +
                        (float)projBorderCols + (float)cInput->projBoardSqPxW - (float)0.5;
                CV_MAT_ELEM(*projImagePoints2, float, projBoardN * i + j, 1) =
                        cInput->projBoardSqPxH * (float)(j / cInput->projBoardWidth) +
                        (float)projBorderRows + (float)cInput->projBoardSqPxH - (float)0.5;
            }

            // Evaluate undistorted image pixels for both the camera and the projector chessboard corners
            CvMat* camDistImagePoints = cvCreateMat(camBoardN, 1, CV_32FC2);
            CvMat* camUndistImagePoints = cvCreateMat(camBoardN, 1, CV_32FC2);
            CvMat* projDistImagePoints = cvCreateMat(projBoardN, 1, CV_32FC2);
            CvMat* projUndistImagePoints = cvCreateMat(projBoardN, 1, CV_32FC2);
            for(int j = 0; j < camBoardN; ++j)
                cvSet1D(camDistImagePoints, j, cvScalar(CV_MAT_ELEM(*camImagePoints, float, camBoardN * i + j, 0),
                        CV_MAT_ELEM(*camImagePoints, float, camBoardN * i + j, 1), 0, 0));
            for(int j = 0; j < projBoardN; ++j)
                cvSet1D(projDistImagePoints, j, cvScalar(CV_MAT_ELEM(*projImagePoints, float, projBoardN * i + j, 0),
                        CV_MAT_ELEM(*projImagePoints, float, projBoardN * i + j, 1), 0, 0));
            cvUndistortPoints(camDistImagePoints, camUndistImagePoints, cOutput->camIntrinsic, cOutput->camDistortion, NULL, NULL);
            cvUndistortPoints(projDistImagePoints, projUndistImagePoints, cOutput->camIntrinsic, cOutput->camDistortion, NULL, NULL);
            cvReleaseMat(&camDistImagePoints);
            cvReleaseMat(&projDistImagePoints);

            // Estimate homography that maps undistorted image pixels to positions on the chessboard
            CvMat* homography = cvCreateMat(3, 3, CV_32FC1);
            CvMat* camSrc = cvCreateMat(camBoardN, 3, CV_32FC1);
            CvMat* camDst = cvCreateMat(camBoardN, 3, CV_32FC1);
            for(int j = 0; j < camBoardN; ++j)
            {
                CvScalar pd = cvGet1D(camUndistImagePoints, j);
                CV_MAT_ELEM(*camSrc, float, j, 0) = (float)pd.val[0];
                CV_MAT_ELEM(*camSrc, float, j, 1) = (float)pd.val[1];
                CV_MAT_ELEM(*camSrc, float, j, 2) = 1.0;
                CV_MAT_ELEM(*camDst, float, j, 0) = CV_MAT_ELEM(*camObjectPoints, float, camBoardN * i + j, 0);
                CV_MAT_ELEM(*camDst, float, j, 1) = CV_MAT_ELEM(*camObjectPoints, float, camBoardN * i + j, 1);
                CV_MAT_ELEM(*camDst, float, j, 2) = 1.0;
            }
            cvReleaseMat(&camUndistImagePoints);
            cvFindHomography(camSrc, camDst, homography, 0, 0, NULL);
            cvReleaseMat(&camSrc);
            cvReleaseMat(&camDst);

            // Map undistorted projector image corners to positions on the chessboard plane
            CvMat* projSrc = cvCreateMat(projBoardN, 1, CV_32FC2);
            CvMat* projDst = cvCreateMat(projBoardN, 1, CV_32FC2);
            for(int j = 0; j < projBoardN; j++)
                cvSet1D(projSrc, j, cvGet1D(projUndistImagePoints, j));
            cvReleaseMat(&projUndistImagePoints);
            cvPerspectiveTransform(projSrc, projDst, homography);
            cvReleaseMat(&homography);
            cvReleaseMat(&projSrc);

            // Define object points corresponding to projector chessboard
            for(int j = 0; j < projBoardN; j++)
            {
                CvScalar pd = cvGet1D(projDst, j);
                CV_MAT_ELEM(*projObjectPoints2, float, projBoardN * i + j, 0) = (float)pd.val[0];
                CV_MAT_ELEM(*projObjectPoints2, float, projBoardN * i + j, 1) = (float)pd.val[1];
                CV_MAT_ELEM(*projObjectPoints2, float, projBoardN * i + j, 2) = 0.0f;
            }
            cvReleaseMat(&projDst);
        }
        for(int i = 0; i < successes; ++i)
        CV_MAT_ELEM(*projPointCounts2, int, i, 0) = CV_MAT_ELEM(*projPointCounts, int, i, 0);

        // Calibrate the projector and save calibration parameters
        printf("Calibrating projector...\n");
        calibFlags = 0;
        calibFlags |= CV_CALIB_ZERO_TANGENT_DIST;
        //cvmSet(cOutput->projDistortion, 4, 0, 0);
        //calibFlags |= CV_CALIB_FIX_K3;
        // Calibrate the projector
        cvCalibrateCamera2(projObjectPoints2, projImagePoints2, projPointCounts2,
                cvSize(cInput->projResWidth, cInput->projResHeight), cOutput->projIntrinsic,
                cOutput->projDistortion, projRotationVectors, projTranslationVectors, calibFlags);
        // Save the results
        printf("Saving calibration images and parameters...\n");
        // Save projector calibration data
        ///*
        for(int i = 0; i < successes; ++i)
        {
            sprintf(fName, "calib/calibImg%dp.png", i);
            cvSaveImage((char*)fName, projCalibImages[i], NULL);
        }
        //
        cvSave("calib/projIntrinsic.xml", cOutput->projIntrinsic, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/projDistortion.xml", cOutput->projDistortion, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/projRotVectors.xml", projRotationVectors, NULL, NULL, cvAttrList(0, 0));
        cvSave("calib/projTransVectors.xml", projTranslationVectors, NULL, NULL, cvAttrList(0, 0));

        // Save extrinsic calibration of projector-camera system
        // (first calibration image is used to define extrinsic calibration
        CvMat* camObjectPoints00 = cvCreateMat(camBoardN, 3, CV_32FC1);
        CvMat* camImagePoints00 = cvCreateMat(camBoardN, 2, CV_32FC1);
        CvMat* camRotationVector00 = cvCreateMat(1, 3, CV_32FC1);
        CvMat* camTranslationVector00 = cvCreateMat(1, 3, CV_32FC1);
        for(int i = 0; i < camBoardN; ++i)
        {
            CV_MAT_ELEM(*camImagePoints00, float, i, 0) = CV_MAT_ELEM(*camImagePoints2, float, i, 0);
            CV_MAT_ELEM(*camImagePoints00, float, i, 1) = CV_MAT_ELEM(*camImagePoints2, float, i, 1);
            CV_MAT_ELEM(*camObjectPoints00, float, i, 0) = CV_MAT_ELEM(*camObjectPoints2, float, i, 0);
            CV_MAT_ELEM(*camObjectPoints00, float, i, 1) = CV_MAT_ELEM(*camObjectPoints2, float, i, 1);
            CV_MAT_ELEM(*camObjectPoints00, float, i, 2) = CV_MAT_ELEM(*camObjectPoints2, float, i, 2);
        }
        // Find camera extrinsics
        cvFindExtrinsicCameraParams2(camObjectPoints00, camImagePoints00, cOutput->camIntrinsic, cOutput->camDistortion,
                camRotationVector00, camTranslationVector00, 0);
        // Save camera extrinsics
        for(int i = 0; i < 3; i++)
            CV_MAT_ELEM(*cOutput->camExtrinsic, float, 0, i) = (float)cvmGet(camRotationVector00, 0, i);
        for(int i = 0; i < 3; i++)
            CV_MAT_ELEM(*cOutput->camExtrinsic, float, 1, i) = (float)cvmGet(camTranslationVector00, 0, i);
        cvSave("calib/camExtrinsic.xml", cOutput->camExtrinsic, NULL, NULL, cvAttrList(0, 0));
        // Save projector extrinsics
        for(int i = 0; i < 3; i++)
            CV_MAT_ELEM(*cOutput->projExtrinsic, float, 0, i) = (float)cvmGet(projRotationVectors, 0, i);
        for(int i = 0; i < 3; i++)
            CV_MAT_ELEM(*cOutput->projExtrinsic, float, 1, i) = (float)cvmGet(projTranslationVectors, 0, i);
        cvSave("calib/projExtrinsic.xml", cOutput->projExtrinsic, NULL, NULL, cvAttrList(0, 0));

        // Free allocated resources
        cvReleaseMat(&camObjectPoints2);
        cvReleaseMat(&camImagePoints2);
        cvReleaseMat(&camPointCounts2);
        cvReleaseMat(&camRotationVectors);
        cvReleaseMat(&camTranslationVectors);
        cvReleaseMat(&projObjectPoints2);
        cvReleaseMat(&projImagePoints2);
        cvReleaseMat(&projPointCounts2);
        cvReleaseMat(&projRotationVectors);
        cvReleaseMat(&projTranslationVectors);
        cvReleaseMat(&camObjectPoints00);
        cvReleaseMat(&camImagePoints00);
        cvReleaseMat(&camRotationVector00);
        cvReleaseMat(&camTranslationVector00);
    }
    else
    {
        printf("At least 2 detected chessboards are required to calibrate the camera!\n");
        return -1;
    }

    // Evalueate projector-camera geometry
    //EvalProjCamSystGeometry(cInput, cOutput);

    // Release rest of the resources
    cvReleaseMat(&camImagePoints);
    cvReleaseMat(&camObjectPoints);
    cvReleaseMat(&camPointCounts);
    cvReleaseMat(&projImagePoints);
    cvReleaseMat(&projPointCounts);
    cvReleaseImage(&projChessboard);
    cvReleaseImage(&camFrame1);
    cvReleaseImage(&camFrame2);
    cvReleaseImage(&camFrame3);
    cvReleaseImage(&projFrame);
    cvReleaseImage(&camFrame1Gray);
    cvReleaseImage(&camFrame2Gray);
    for(int i = 0; i < cInput->numberBoards; i++)
    {
        cvReleaseImage(&camCalibImages[i]);
        cvReleaseImage(&projCalibImages[i]);
    }
    free(camCalibImages);
    free(projCalibImages);

    // Return with success
    printf("Projector-camera calibration was successfull!\n");
    return 0;
}


int ComputePCGeometry(T_CalibInput *cInput, T_CalibOutput *cOutput)
{
    // Load projector and camera calibration matrices
    /*
     * Assume now we have the data  already loaded in cOutput structure
     */

    // Restore calibration parameters
    CvMat* r = cvCreateMat(1, 3, CV_32FC1);
    CvMat* camRotation = cvCreateMat(3, 3, CV_32FC1);
    CvMat* camTranslation = cvCreateMat(3, 1, CV_32FC1);
    CvMat* projRotation = cvCreateMat(3, 3, CV_32FC1);
    CvMat* projTranslation = cvCreateMat(3, 1, CV_32FC1);
    cvGetRow(cOutput->camExtrinsic, r, 0);
    cvRodrigues2(r, camRotation, NULL);
    for(int i = 0; i < 3; i++)
        cvmSet(camTranslation, i, 0, cvmGet(cOutput->camExtrinsic, 1, i));
    cvGetRow(cOutput->projExtrinsic, r, 0);
    cvRodrigues2(r, projRotation, NULL);
    for(int i = 0; i < 3; i++)
        cvmSet(projTranslation, i, 0, cvmGet(cOutput->projExtrinsic, 1, i));
    cvReleaseMat(&r);

    // Determine centers of projection
    // (all positions are in the coordinate system of the first camera)
    // Set camera center to origin
    cvSet(cOutput->camCenter, cvScalarAll(0), NULL);
    // Compute projection center
    cvGEMM(projRotation, projTranslation, -1, NULL, 0, cOutput->projCenter, CV_GEMM_A_T);
    cvGEMM(camRotation, cOutput->projCenter, 1, camTranslation, 1, cOutput->projCenter, 0);


    return 0;
}