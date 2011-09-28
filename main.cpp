#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    CvCapture* capture = 0;
    IplImage *frame = 0;
    int keyPressed = 0;
    bool running = true;
    std::string WINDOW_NAME = "capture";
    int threshold = 100;

    capture = cvCaptureFromCAM(0);
    if (! capture)
    {
        std::cerr << "Cannot initialize camera" << std::endl;;
        return 1;
    }
    cvNamedWindow(WINDOW_NAME.c_str(), CV_WINDOW_AUTOSIZE);

    while (running)
    {
        frame = cvQueryFrame(capture);

        // always check
        if (! frame)
        {
            std::cout << "Bad frame." << std::endl;
            break;
        }
        IplImage *tmp = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
        cvCvtColor(frame, tmp, CV_RGB2GRAY );
        cvSmooth(tmp, tmp, CV_GAUSSIAN, 11, 11 );
        cvThreshold(tmp, tmp, threshold, 255, CV_THRESH_BINARY);
        //CvMemStorage* storage = cvCreateMemStorage(0);
        //CvSeq* cvHoughCircles(tmp, storage, int method, double dp, double min_dist, double param1=100, double param2=100, int min_radius=0, int max_radius=0 );

        //cvResize(frame, temp, CV_INTER_CUBIC); // Resize
        //cvSmooth(temp, temp, CV_GAUSSIAN, 7, 7, 0, 0); // Real time filter: Gaussian blur
        //cvSaveImage("test.jpg", temp, 0); // Save this image
        //cvSave("test.xml", temp, NULL, NULL, cvAttrList(0, 0)); // Save as xml
        //cvShowImage(WINDOW_NAME.c_str(), frame); // Display the frame
        cvShowImage(WINDOW_NAME.c_str(), tmp); // Display the frame

        keyPressed = cvWaitKey(1); // wait 1 ms
        if (keyPressed == 27) // escape to quit
            running = false;
        
    }
    
    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_NAME.c_str());
    return 0;
}
