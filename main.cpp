#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <string>

int main(int argc, char** argv) {

    CvCapture* capture = 0;
    IplImage *frame = 0;
    int keyPressed = 0;
    bool running = true;
    std::string WINDOW_NAME = "capture";

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

                        
        //IplImage *temp = cvCreateImage(cvSize(frame->width / 2, frame->height / 2), frame->depth, frame->nChannels); // A new Image half size
        //cvResize(frame, temp, CV_INTER_CUBIC); // Resize
        //cvSmooth(temp, temp, CV_GAUSSIAN, 7, 7, 0, 0); // Real time filter: Gaussian blur
        //cvSaveImage("test.jpg", temp, 0); // Save this image
        //cvSave("test.xml", temp, NULL, NULL, cvAttrList(0, 0)); // Save as xml
        cvShowImage(WINDOW_NAME.c_str(), frame); // Display the frame

        keyPressed = cvWaitKey(1); // wait 1 ms
        if (keyPressed == 27) // escape to quit
            running = false;
        
        //cvReleaseImage(&temp);

        //if (cvWaitKey(5000) == 27) // Escape key and wait, 5 sec per capture
        //    break;
    }
    
    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_NAME.c_str());
    return 0;
}
