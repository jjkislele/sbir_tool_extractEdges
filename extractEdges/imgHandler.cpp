#include "imgHandler.h"

IplImage* imgHandler::extract(IplImage* Image) {
	IplImage* runImg = cvCloneImage(Image);
	//彩色变灰度图
	if (Image->nChannels > 1) {
		IplImage* gimg = cvCreateImage(cvGetSize(runImg), Image->depth, 1);
		cvCvtColor(runImg, gimg, CV_BGR2GRAY);
		cvReleaseImage(&runImg);
		runImg = gimg;
	}
	IplImage* edge = cvCreateImage(cvGetSize(runImg), runImg->depth, 1);
	for (int s = 19; s >= 1; s -= 2) {
		cvSmooth(runImg, edge, CV_GAUSSIAN, s); //cvSmooth(src, dst, GAUSSIAN, size of kernel)
#ifdef I_WANT_TO_WATCH
		cvShowImage("edge", edge);
		printf("s = %d\n", s);
		cvWaitKey(0);
#endif
		cvCanny(edge, edge, 100, 200);
#ifdef I_WANT_TO_WATCH
		cvShowImage("edge", edge);
		cvWaitKey(0);
#endif
		CvScalar sc = cvSum(edge); //边缘图像的各通道的像素值和
		float area = sc.val[0] / (float)(255 * runImg->width*runImg->height);
		if (area>0.02)
			break;
	}
	cvNot(edge, edge); //边缘图，白底黑线
	return edge;
}

IplImage* imgHandler::resize(IplImage* Image, int maxdim) {
	CvSize s;
	if (Image->width > maxdim) {
		float r = (float)maxdim / Image->width;
		s.width = maxdim;
		s.height = (float)(Image->height) * r;
	}
	else {
		float r = (float)maxdim / Image->height;
		s.height = maxdim;
		s.width = (float)(Image->width) * r;
	}
	IplImage* resize = cvCreateImage(s, Image->depth, Image->nChannels);
	cvResize(Image, resize);
	return  resize;
}