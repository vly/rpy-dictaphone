#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include "maestro.h"
#include <math.h>
using std::string;  using std::cout;
using std::endl;  using std::vector;
using std::cerr;
using namespace cv;


#ifdef __APPLE__
#define NA_DEVICE "/dev/cu.usbmodem00065291"
#else
#define NA_DEVICE "/dev/ttyACM0"
#endif

#define HAAR 0

#define NA_FACE_MOVE_THRESHOLD 400

#ifdef __APPLE__
#define NA_CAMERA 1
#else
#define NA_CAMERA 0
#endif

#define GUI_MODE 1
#define NA_MIN_FACE_SIZE 50

const string strWindowName = "Face detection test";
#if HAAR
const string strCascadeClassifier = "haarcascade_frontalface_alt.xml";
#else
const string strCascadeClassifier = "lbpcascade_frontalface.xml";
#endif
const int    nFrameWaitTime = 33;
const int    nFrameWidth = 320;
const int    nFrameHeight = 240;

struct face_detect_arg
{
#if HAAR
  CvHaarClassifierCascade* pCvHaar;
#else
  CascadeClassifier* pCvHaar;
#endif
  IplImage* pImage;
  bool bDetected;
  CvRect rc;
  bool threadRunning;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define FAILURE_CHECK(p, x) \
assert(p);  \
if(p == NULL)  \
{    \
std::cerr << x << std::endl;  \
exit(EXIT_FAILURE);    \
}

#if HAAR
CvHaarClassifierCascade* loadCascadeClassifier(const string& strPath)
{
  CvHaarClassifierCascade* p = NULL;
  p = (CvHaarClassifierCascade*)cvLoad(strPath.c_str(), 0, 0, 0);

  FAILURE_CHECK(p, "Conldn't load classifier");
  return p;
}
#else
void loadCascadeClassifier(const string& strPath,CascadeClassifier* pc)
{
  FAILURE_CHECK(pc, "Conldn't load classifier");
  if(!pc->load(strPath))
  {
    std::cerr << "Conldn't load classifier" << endl;
    exit(EXIT_FAILURE);
  }
}
#endif

CvCapture* createCaptureFrameCamera(int nCaptureIndex = -1)
{
  CvCapture* p = cvCaptureFromCAM(nCaptureIndex);
  FAILURE_CHECK(p, "Init Capture Failed");
  if(!cvGrabFrame(p))
  {
    std::cerr << "Conldn't GrabFrame" << std::endl;
    exit(EXIT_FAILURE);
  }

#if 1
  cvSetCaptureProperty(p, CV_CAP_PROP_FRAME_WIDTH, nFrameWidth);
  cvSetCaptureProperty(p, CV_CAP_PROP_FRAME_HEIGHT, nFrameHeight);
  cvSetCaptureProperty(p, CV_CAP_PROP_FPS, 5);
#endif
  return p;
}

IplImage* captureFrame(CvCapture* pCapture)
{
  IplImage* p = cvQueryFrame(pCapture);
  FAILURE_CHECK(p, "Error: cvQueryFrame failed");

  return p;
}

#if HAAR
CvRect detectFaceInImage(IplImage* inputImg, CvHaarClassifierCascade* cascade)
#else
CvRect detectFaceInImage(IplImage* inputImg, CascadeClassifier* cascade)
#endif
{
  // Smallest face size
  CvSize minFeatureSize = cvSize(NA_MIN_FACE_SIZE,NA_MIN_FACE_SIZE);
  // Only search for 1 face
  int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;
  // How detailed should the search be
  float search_scale_factor = 1.1f;

  CvMemStorage* storage = cvCreateMemStorage(0);
  cvClearMemStorage(storage);

  // if the detect image is color, use the greycopy
  IplImage* detectImg = inputImg;
  if(detectImg->nChannels > 1)
  {
    CvSize size = cvSize(detectImg->width, detectImg->height);
    IplImage* greyImg = cvCreateImage(size, IPL_DEPTH_8U, 1);
    cvCvtColor(detectImg, greyImg, CV_BGR2GRAY);
    detectImg = greyImg;
  }

  // count detect time, measure performance
    #if 0
  double time = (double)cvGetTickCount();
#endif

  // get all the faces in rects
#if HAAR
  CvSeq* rects = cvHaarDetectObjects(detectImg, cascade, storage, search_scale_factor, 3, flags, minFeatureSize);
#else
  vector<Rect> rects;
  cascade->detectMultiScale(detectImg, rects, search_scale_factor, 3, flags, minFeatureSize);
#endif
#if 0
  time = (double)cvGetTickCount() - time;
  // change CPU time to ms


  int ms = cvRound(time / ((double)cvGetTickFrequency() * 1000.0));
#endif

  // How many face detected
#if HAAR
  int nFaces = rects->total;
#else
  int nFaces = (int)rects.size();
#endif

#if 0
  cout << "Face detection took "
    << ms << " ms and found "
    << nFaces << " objects" << endl;
#endif

  // get the bigget one
  CvRect rc = cvRect(-1,-1,-1,-1);
  if(nFaces > 0)
  {
#if HAAR
    rc = *(CvRect*)cvGetSeqElem(rects, 0);
#else
    rc = cvRect(rects[0].x, rects[0].y, rects[0].width, rects[0].height);
#endif
  }

  if(inputImg->nChannels > 1)
    cvReleaseImage(&detectImg);
  cvReleaseMemStorage(&storage);

  return rc;
}

void* faceDetectFunction(void* arg)
{
  face_detect_arg* fd = (face_detect_arg*)arg;

  while(fd->threadRunning)
  {
    if(fd->pImage == NULL)
    {
      //sleep(200);
      continue;
    }

    pthread_mutex_lock(&mutex);
    IplImage* pImg = fd->pImage;
    pthread_mutex_unlock(&mutex);

    CvRect rc = detectFaceInImage(pImg, fd->pCvHaar);
    if(rc.x != -1)
    {
      pthread_mutex_lock(&mutex);
      fd->bDetected = true;
      fd->rc = rc;
      pthread_mutex_unlock(&mutex);
    }
    else
    {
      pthread_mutex_lock(&mutex);
      fd->bDetected = false;
      pthread_mutex_unlock(&mutex);
    }
  }

  return NULL;
}

double euclideanDistance(CvPoint pt1, CvPoint pt2) {
    double x = pt1.x - pt2.x;
    double y = pt1.y - pt2.y;
    double dist;

    dist = pow(x,2)+pow(y,2);
    return dist;
}

int main(int argc, char** argv)
{

    Maestro maestro(NA_DEVICE);
    Servo horizontalServo(0, 3968, 9216, 6104);
    Servo verticalServo(1, 3968, 9216, 8220);

  face_detect_arg* fd = (face_detect_arg*)malloc(sizeof(face_detect_arg));

  // initialization
#if HAAR
  fd->pCvHaar = loadCascadeClassifier(strCascadeClassifier);
#else
  fd->pCvHaar = new CascadeClassifier();
  loadCascadeClassifier(strCascadeClassifier, fd->pCvHaar);
#endif
  fd->pImage = NULL;
  fd->threadRunning = true;
  fd->bDetected = false;

  CvCapture* pCapture = createCaptureFrameCamera(NA_CAMERA);

#if GUI_MODE
  cvNamedWindow(strWindowName.c_str(), CV_WINDOW_AUTOSIZE);
#endif

#if GUI_MODE

  IplImage* src =  cvQueryFrame(pCapture);

    IplImage* bck1 = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
  FAILURE_CHECK(bck1, "cvCreateImage Error");
#endif
  // Create thread
  pthread_t face_detect_thread;
  pthread_create(&face_detect_thread, NULL, faceDetectFunction, (void*)fd);

#if GUI_MODE
    // Debug
    CvScalar blue  = cvScalar(255, 0, 0, 0);
    CvScalar red   = cvScalar(0, 0, 255, 0);
#endif

    float screenDivisions = 2.6;
#if GUI_MODE
    Point rightPt1  = Point(0, 0);
    Point leftPt2   = Point(nFrameWidth-1, nFrameHeight-1);
    Point topPt1    = Point(0, 0);
    Point bottomPt2 = Point(nFrameWidth-1, nFrameHeight-1);
#endif

    Point rightPt2  = Point(nFrameWidth/screenDivisions, nFrameHeight-1);
    Point leftPt1   = Point(nFrameWidth - (nFrameWidth/screenDivisions), 0);
    Point topPt2    = Point(nFrameWidth-1, nFrameHeight/screenDivisions);
    Point bottomPt1 = Point(0, nFrameHeight - (nFrameHeight/screenDivisions));

    maestro.goHome(&horizontalServo);
    maestro.goHome(&verticalServo);
    sleep(1);

    CvPoint lastFace;
    lastFace.x = 0;
    lastFace.y = 0;

  CvRect rc;
  bool bDetected = false;
  bool isRunning = true;
  while(isRunning)
  {
    IplImage* pImg = cvQueryFrame(pCapture);
    if(!pImg)
    {
      std::cerr << "Query Frame Failed" << endl;
      sleep(1);
      continue;
    }

    assert(pImg);
    pthread_mutex_lock(&mutex);
    fd->pImage = pImg;
    bDetected = fd->bDetected;
    rc = fd->rc;
    pthread_mutex_unlock(&mutex);

        CvPoint facePoint;
        facePoint.x = 0;
        facePoint.y = 0;

#if GUI_MODE
        // Show regions
        cvRectangle(pImg, rightPt1, rightPt2, red, 1, 8, 0);
        cvRectangle(pImg, leftPt1, leftPt2, red, 1, 8, 0);
        cvRectangle(pImg, topPt1, topPt2, red, 1, 8, 0);
        cvRectangle(pImg, bottomPt1, bottomPt2, red, 1, 8, 0);
#endif

    if(bDetected) {
#if GUI_MODE
      CvPoint p1 = cvPoint(rc.x, rc.y);
      CvPoint p2 = cvPoint(rc.x + rc.width, rc.y + rc.height);
            cvRectangle(pImg, p1, p2, CV_RGB(0, 255, 0), 5, 8);
#endif

            facePoint.x = rc.x + (rc.width / 2.0f);
            facePoint.y = rc.y + (rc.height / 2.0f);
#if GUI_MODE
            cvCircle(pImg, facePoint, 2, blue, 2, 8, 0);
#endif
    }

#if GUI_MODE
    cvResize(src, bck1, CV_INTER_LINEAR);
    cvShowImage(strWindowName.c_str(), bck1);
#endif
        if (facePoint.x > 0 && facePoint.y > 0) {

            // Euclid
            double dist = euclideanDistance(facePoint, lastFace);

            if(dist > 0 && dist < NA_FACE_MOVE_THRESHOLD){

                // Move to the stage right.
                if (facePoint.x < rightPt2.x) {
                    maestro.stepUp(&horizontalServo);
                } else if (facePoint.x > leftPt1.x){
                    maestro.stepDown(&horizontalServo);
                }

                // Move to the stage top
                if (facePoint.y < topPt2.y) {
                    maestro.stepUp(&verticalServo);
                }
                else if (facePoint.y > bottomPt1.y){
                    maestro.stepDown(&verticalServo);
                }
            }

            lastFace.x = facePoint.x;
            lastFace.y = facePoint.y;

        }

    if(cvWaitKey(nFrameWaitTime) == 27) {
            isRunning = false;
            fd->threadRunning = false;
        }

    usleep(1000);
  }

    maestro.goHome(&horizontalServo);
    maestro.goHome(&verticalServo);
    sleep(1);


  // release
#if GUI_MODE
  cvReleaseImage(&bck1);
#endif
    //  cvReleaseHaarClassifierCascade(&pFaceCascade);
  cvReleaseCapture(&pCapture);
#if GUI_MODE
  cvDestroyWindow(strWindowName.c_str());
#endif
  return 0;
}
