#include "opencv2\opencv.hpp"

void MouseDraw(int event,int x,int y,int flags,void*param);
struct MouseArgs{
	IplImage* img;
	CvPoint p_start;
	CvPoint p_end;
	CvSeq* seq;
	CvMemStorage* storage;
	int points;
	// init
	MouseArgs():img(0),points(0){
		p_start = cvPoint(-1,-1);
		p_end = cvPoint(-1,-1);
		storage = cvCreateMemStorage(0);
		seq = cvCreateSeq( CV_32SC2,sizeof(CvSeq),sizeof(CvPoint), storage );
	}
	// destroy
	void Destroy(){
		if(!img)
			cvReleaseImage(&img);
		cvReleaseMemStorage(&storage );
		seq = NULL;
		img = NULL;
	}
};
