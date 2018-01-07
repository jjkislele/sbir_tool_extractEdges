// Developed By:	Jiang Junkun
// Date:			06 Dec 2017
// Info:
//		Requires OpenCV
// ToDo:
//		Extract edges from a set of imgs, using method:
//		iterate {
//			smooth(img);
//			each_edge = canny(img);
//		}
//		edges = sum(each_edge)
//		which has more detail rather than simply using canny

#include <iostream>
#include <fstream>
#include <omp.h>

#include "myIO.h"
#include "imgHandler.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_contrib2413d.lib")
#pragma comment(lib, "opencv_features2d2413d.lib")
#pragma comment(lib, "opencv_flann2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_legacy2413d.lib")
#pragma comment(lib, "opencv_ml2413d.lib")
#pragma comment(lib, "opencv_nonfree2413d.lib")
#pragma comment(lib, "opencv_objdetect2413d.lib")
#pragma comment(lib, "opencv_photo2413d.lib")
#pragma comment(lib, "opencv_stitching2413d.lib")
#pragma comment(lib, "opencv_ts2413d.lib")
#pragma comment(lib, "opencv_video2413d.lib")
#pragma comment(lib, "opencv_videostab2413d.lib")
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_video2413.lib")
#pragma comment(lib, "opencv_ml2413.lib")
#pragma comment(lib, "opencv_legacy2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_features2d2413.lib")
#pragma comment(lib, "opencv_contrib2413.lib")
#pragma comment(lib, "opencv_flann2413.lib")
#pragma comment(lib, "opencv_objdetect2413.lib")
#pragma comment(lib, "opencv_nonfree2413.lib")
#endif
#endif

int main(int argc, const char* argv[])
{
	if (argc != 2) {
		printf("extractEdges.exe \"working directory\"[Default:'.']");
		return 0;
	}

	int setSize = 256;
	string wkDir = argv[1];
	vector<string> subFolders;
	string wkSubDir;
	string outSubDir;
	int numSubFolders;
	numSubFolders = myIO::getSubFolders(wkDir, subFolders);
	printf("working dir:	%s\n\n%d folders detected\n", wkDir.c_str(), numSubFolders);
	for (int i = 0; i < numSubFolders; i++) {
		wkSubDir = wkDir + "/" + subFolders[i] + '/';
		outSubDir = wkDir + "/../edges/" + subFolders[i] + '/';
		printf("---------------\nprocessing %s\n", wkSubDir.c_str());

		string imgNameWJPG = wkSubDir + "*.jpg";
		string imgNameWPNG = wkSubDir + "*.png";
		vector<string> names;
		string inDir, ext;
		int imgNum = myIO::getNamesNE(imgNameWJPG, names, inDir, ext);
		if (!imgNum) {
			imgNum = myIO::getNamesNE(imgNameWPNG, names, inDir, ext);
		}
		myIO::mkDir(outSubDir);
		printf("Load images `%s' and save its edges to `%s'\n", imgNameWJPG.c_str(), outSubDir.c_str());
		
		//--------------------------------------------
#pragma omp parallel for
		for (int i = 0; i < imgNum; i++) {
			string name = names[i] + ext;
			imgHandler imgHandler;
			string outPath = outSubDir + names[i] + ".jpg";

			// ignore if file exists
			if (myIO::fileExist(outPath))
				continue;

			printf("Processing %d/%dth image: %-70s\r", i, imgNum, (name).c_str());
			IplImage* img = cvLoadImage((inDir + name).c_str(), 0);
			if (!img) {
				std::cout << "Error Loading Image" << std::endl;
				return 0;
			}

			// resize to setSize
			IplImage* resizedImg = imgHandler.resize(img, setSize);

			// extract edges
			IplImage* edge = imgHandler.extract(resizedImg);

			// store
			//cvConvertScale(g, g8bit, 255); //Í¼ÏñÏßÐÔ±ä»»
			cvSaveImage(outPath.c_str(), edge);
		}
		//--------------------------------------------

		printf("---------------\n%d folders left\n", numSubFolders - i - 1);
	}
}