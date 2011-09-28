#include <cv.h>
#include <stdio.h>
#include <highgui.h>
#include <iostream>
#include <string>

static const double FROM_R = 100;
static const double FROM_G = 50;
static const double FROM_B = 100;
static const double TO_R = 255;
static const double TO_G = 255;
static const double TO_B = 255;

IplImage* GetThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);
    // smooth it
    cvSmooth(imgHSV, imgHSV, CV_GAUSSIAN, 11, 11 );
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    // Values 20,100,100 to 30,255,255 working perfect for yellow at around 6pm
    cvInRangeS(imgHSV, cvScalar(FROM_R, FROM_G, FROM_B), cvScalar(TO_R, TO_G, TO_B), imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

int main(int argc, char *argv[])
{
    CvCapture* capture = 0;
    IplImage *frame = 0;
    int keyPressed = 0;
    bool running = true;
    std::string WINDOW_NAME = "capture";
    std::string THRESH_WIN = "thresh";
    int threshold = 100;
    int camera_index = 0;

    capture = cvCaptureFromCAM(camera_index);
    if (! capture)
    {
        std::cerr << "Cannot initialize camera" << std::endl;;
        return 1;
    }
    cvNamedWindow(WINDOW_NAME.c_str(), CV_WINDOW_AUTOSIZE);
    cvNamedWindow(THRESH_WIN.c_str(), CV_WINDOW_AUTOSIZE);

    // This image holds the "scribble" data...
    // the tracked positions of the ball
    IplImage* imgScribble = NULL;

    // Holding the last and current ball positions
    int posX = 0;
    int posY = 0;

    while (running)
    {
        frame = cvQueryFrame(capture);

        // always check
        if (! frame)
        {
            std::cout << "Bad frame." << std::endl;
            break;
        }

        // If this is the first frame, we need to initialize it
        if(imgScribble == NULL)
        {
            imgScribble = cvCreateImage(cvGetSize(frame), 8, 3);
        }
        // Holds the yellow thresholded image (yellow = white, rest = black)
        IplImage* imgYellowThresh = GetThresholdedImage(frame);

        // Calculate the moments to estimate the position of the ball
        CvMoments *moments = new CvMoments;
        cvMoments(imgYellowThresh, moments, 1);

        // The actual moment values
        double moment10 = cvGetSpatialMoment(moments, 1, 0);
        double moment01 = cvGetSpatialMoment(moments, 0, 1);
        double area = cvGetCentralMoment(moments, 0, 0);
        delete moments;

        int lastX = posX;
        int lastY = posY;

        posX = moment10 / area;
        posY = moment01 / area;

        // Print it out for debugging purposes
        printf("position (%d,%d)\n", posX, posY);

        // We want to draw a line only if its a valid position
        if(lastX > 0 && lastY > 0 && posX > 0 && posY > 0)
        {
            // Draw a yellow line from the previous point to the current point
            cvLine(imgScribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0, 255, 255), 5);
        }

        // Add the scribbling image and the frame... and we get a combination of the two
        cvAdd(frame, imgScribble, frame);
        cvShowImage(THRESH_WIN.c_str(), imgYellowThresh);
        cvShowImage(WINDOW_NAME.c_str(), frame);

        // Release the thresholded image... we need no memory leaks.. please
        cvReleaseImage(&imgYellowThresh);

        keyPressed = cvWaitKey(1); // wait 1 ms
        if (keyPressed == 27) // escape to quit
            running = false;
    }
    
    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_NAME.c_str());
    return 0;
}
