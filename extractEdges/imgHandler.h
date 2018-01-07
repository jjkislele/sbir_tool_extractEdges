#pragma once
#include <vector>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//#define I_WANT_TO_WATCH

class imgHandler
{
public:
	IplImage* extract(IplImage* Image);
	IplImage* resize(IplImage* Image,int maxdim);
};