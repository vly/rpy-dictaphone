#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include "face_detect.h"
#include "message.h"
using std::string;	using std::cout;
using std::endl;	using std::vector;
using std::cerr;
using namespace cv;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define FAILURE_CHECK(p, x) \
	assert(p);	\
	if(p == NULL)	\
	{		\
		std::cerr << x << std::endl;	\
		exit(EXIT_FAILURE);		\
	}
	
face_detect::face_detect()
	: strWindowName("Face detection test")
	, strCascadeClassifier("lbpcascade_frontalface.xml")
	, nFrameWaitTime(33)
	, nFrameWidth(320)
	, nFrameHeight(240)
{

}

face_detect::~face_detect()
{

}

bool face_detect::setUpService()
{
	fd = (face_detect_arg*)malloc(sizeof(face_detect_arg));
	fd->pCvHaar = new CascadeClassifier();
	loadCascadeClassifier(strCascadeClassifier, fd->pCvHaar);

	fd->pImage = NULL;
	fd->threadRunning = true;
	fd->bDetected = false;
}

void face_detect::startMainLoop()
{
	isRunning = true;
	pthread_create(&face_main_thread, NULL, faceCaptureFunction, (void*)this);
}

void face_detect::stopMainLoop()
{
	isRunning = false;
	fd->threadRunning = false;
}
	
void face_detect::receiveMessage(const Message& m)
{
	// TODO: handle the message ...
}
	
void face_detect::loadCascadeClassifier(const string& strPath,CascadeClassifier* pc)
{
	FAILURE_CHECK(pc, "Conldn't load classifier");
	if(!pc->load(strPath))
	{
		std::cerr << "Conldn't load classifier" << endl;
		exit(EXIT_FAILURE);
	}

}
	
CvCapture* face_detect::createCaptureFrameCamera(int nCaptureIndex)
{
	CvCapture* p = cvCaptureFromCAM(nCaptureIndex);
	FAILURE_CHECK(p, "Init Capture Failed");
	if(!cvGrabFrame(p))
	{
		std::cerr << "Conldn't GrabFrame" << std::endl;
		exit(EXIT_FAILURE);
	}

	cvSetCaptureProperty(p, CV_CAP_PROP_FRAME_WIDTH, nFrameWidth);
	cvSetCaptureProperty(p, CV_CAP_PROP_FRAME_HEIGHT, nFrameHeight);
	cvSetCaptureProperty(p, CV_CAP_PROP_FPS, 5);
	
	return p;
}
	
IplImage* face_detect::captureFrame(CvCapture* pCapture)
{
	IplImage* p = cvQueryFrame(pCapture);
	FAILURE_CHECK(p, "Error: cvQueryFrame failed");

	return p;
}
	
CvRect detectFaceInImage(IplImage* inputImg, CascadeClassifier* cascade)
{
	// Smallest face size
	CvSize minFeatureSize = cvSize(80,80);
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
	double time = (double)cvGetTickCount();

	// get all the faces in rects
	vector<Rect> rects;
	cascade->detectMultiScale(detectImg, rects, search_scale_factor, 3, flags, minFeatureSize);
	time = (double)cvGetTickCount() - time;

#if 0
	// change CPU time to ms
	int ms = cvRound(time / ((double)cvGetTickFrequency() * 1000.0));

	cout << "Face detection took " 
		<< ms << " ms and found " 
		<< nFaces << " objects" << endl; 
#endif
	// How many face detected
	int nFaces = rects.size();

	// get the bigget one
	CvRect rc = cvRect(-1,-1,-1,-1);
	if(nFaces > 0)
	{
		rc = cvRect(rects[0].x, rects[0].y, rects[0].width, rects[0].height);
	}

	if(detectImg->nChannels > 1)
		cvReleaseImage(&detectImg);
	cvReleaseMemStorage(&storage);

	return rc;

}

void* faceCaptureFunction(void* arg)
{
	face_detect* fdc = (face_detect*)arg;
	face_detect_arg* fd = fdc->getDetectArg();
	
	CvCapture* pCapture = fdc->createCaptureFrameCamera();

	cvNamedWindow(fdc->getWindowName().c_str(), CV_WINDOW_AUTOSIZE);

	IplImage* src =  cvQueryFrame(pCapture);
	IplImage* bck1 = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	FAILURE_CHECK(bck1, "cvCreateImage Error");

	// Create thread
	pthread_create(&(fdc->face_detect_thread), NULL, faceDetectFunction, (void*)fd);

	CvRect rc;
	bool bDetected = false;
	while(fdc->isMainThreadRunning())
	{
		IplImage* pImg = cvQueryFrame(pCapture);
		if(!pImg)
		{
			std::cerr << "Query Frame Failed" << endl;
			sleep(1000);
			continue;
		}

		assert(pImg);
		pthread_mutex_lock(&mutex);
		fd->pImage = pImg;
		bDetected = fd->bDetected;
		rc = fd->rc;
		pthread_mutex_unlock(&mutex);

		if(bDetected)
		{
			CvPoint p1 = cvPoint(rc.x, rc.y);
			CvPoint p2 = cvPoint(rc.x + rc.width, rc.y + rc.height);
			cvRectangle(pImg, p1, p2, CV_RGB(0, 255, 0), 5, 8);
		}

		cvResize(src, bck1, CV_INTER_LINEAR);

		cvShowImage(fdc->getWindowName().c_str(), bck1);

//		if(cvWaitKey(fdc->getFrameWaitTime()) == 27)
//			isRunning = false;

		sleep(1);
	}
	
	// release
	cvReleaseImage(&bck1);
	cvReleaseCapture(&pCapture);
	cvDestroyWindow(fdc->getWindowName().c_str());

	return NULL;
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

bool face_detect::isMainThreadRunning() const
{
	return isRunning;
}

const string& face_detect::getWindowName() const
{
	return strWindowName;
}

face_detect_arg* face_detect::getDetectArg()
{
	return fd;
}

pthread_t face_detect::getMainThread() const
{
	return face_main_thread;
}

pthread_t face_detect::getDetectThread() const
{
	return face_detect_thread;
}

const int face_detect::getFrameWaitTime() const
{
	return nFrameWaitTime;
}

const int face_detect::getFrameWidth() const
{
	return nFrameWidth;
}

const int face_detect::getFrameHeight() const
{
	return nFrameHeight;
}
