/**
 * face_detect sub system
 * Created by Alfred Yang
 * 13-09-2013
 */

#include "singleton.h"
#include "base_service.h"
#include <string>
#include <opencv2/opencv.hpp>
using std::string;
using namespace cv;

struct face_detect_arg;
class Message;

class face_detect 
	: public Singleton<face_detect>
	, public base_service
{
	const string strWindowName;
	const string strCascadeClassifier;
	const int    nFrameWaitTime;
	const int    nFrameWidth;
	const int    nFrameHeight;

	bool isRunning;

	face_detect_arg* fd;

public:
	pthread_t face_main_thread;
	pthread_t face_detect_thread;

public:
	bool isMainThreadRunning() const;
	const string& getWindowName() const;
	face_detect_arg* getDetectArg();
	const int getFrameWaitTime() const;
	const int getFrameWidth() const;
	const int getFrameHeight() const;

	pthread_t getMainThread() const;
	pthread_t getDetectThread() const;

public:
	face_detect();
	~face_detect(); 

	virtual bool setUpService();
	virtual void startMainLoop();
	virtual void stopMainLoop();
	virtual void receiveMessage(const Message& m);

public:
	void loadCascadeClassifier(const string& strPath,CascadeClassifier* pc);
	CvCapture* createCaptureFrameCamera(int nCaptureIndex = -1);
	IplImage* captureFrame(CvCapture* pCapture);

};

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

void* faceCaptureFunction(void* arg);
void* faceDetectFunction(void* arg);
CvRect detectFaceInImage(IplImage* inputImg, CascadeClassifier* cascade);
