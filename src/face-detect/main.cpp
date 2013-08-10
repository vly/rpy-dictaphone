//
//  main.cpp
//  face-detect
//
//  Created by Neil Ang on 3/08/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#define NA_WINDOW_NAME        "Face detection test"
#define NA_CASCADE_CLASSIFIER "haarcascade_frontalface_alt.xml"
#define NA_FRAME_WAIT_TIME    33

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{

    // Load classifier or exit
    CascadeClassifier faceCascade;
    String face_cascade_name = NA_CASCADE_CLASSIFIER;

    if(!faceCascade.load(face_cascade_name)) {
        cout << "Couldn't load classifier: ";
        cout << NA_CASCADE_CLASSIFIER;
        cout << endl;
        return -1;
    }

    // Setup face detection vars
    double scaleFactor = 1.1;
    int minNeighbors   = 3;
    int flags          = CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE;
    Size minSize       = Size(30,30);

    vector<Rect> faces;

    // Setup video data vars
    VideoCapture captureDevice;
    captureDevice.open(-1);

    Mat captureFrame;
    Mat grayscaleFrame;

    // Load window
    namedWindow(NA_WINDOW_NAME, CV_WINDOW_AUTOSIZE);

    while(true){

        // Take a frame from the device
        captureDevice >> captureFrame;

        // If the device didn't return a frame we can't continue
        if(captureFrame.empty()){
            cout << "No frame captured. Exiting.";
            cout << endl;
            break;
        }

        // Convert frame to grayscale
        cvtColor(captureFrame, grayscaleFrame, CV_BGR2GRAY);

        // Equalise frame for better image contrast
        equalizeHist(grayscaleFrame, grayscaleFrame);

        // Detect faces
        faceCascade.detectMultiScale(grayscaleFrame, faces, scaleFactor, minNeighbors, flags, minSize);

        // Draw rect on face
        for(int i = 0; i < faces.size(); i++){
            Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
            Point pt2(faces[i].x, faces[i].y);
            rectangle(captureFrame, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
        }

        // Display frame in window
        imshow(NA_WINDOW_NAME, captureFrame);


        // Listen for escape key for x milliseconds
        if(cvWaitKey(NA_FRAME_WAIT_TIME) == 27){
          break;
        }
    }

    // Destroy window
    cvDestroyWindow(NA_WINDOW_NAME);

    return 0;
}

