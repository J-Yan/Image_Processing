// // Name: Yan Jiao
// // USC ID: 6419057887
// // USC Email: yanjiao@usc.edu
// // SIFT

// #include "opencv2/core.hpp"
// #include "opencv2/highgui.hpp"
// #include <stdio.h>
// #include <iostream>
// #include <stdlib.h>
// #include <vector>
// #include <math.h>
// #include <string>

// #include "opencv2/features2d.hpp"
// //#include "opencv2/xfeatures2d.hpp"
// #include "opencv2/xfeatures2d/nonfree.hpp"

// //#include "opencv2/imgproc/imgproc.hpp"
// //#include <opencv2/nonfree/nonfree.hpp>
// //initModule_nonfree();
// using namespace cv;
// using namespace cv::xfeatures2d;
// using namespace std;

// void readme();

// /** @function main */
// int main(int argc, char** argv)
// {
// 	FILE *file;
// 	const int SizeX = 1024;
// 	const int SizeY = 768;
// 	const int bi = 3;

// 	if (argc != 3)
// 	{
// 		readme(); return -1;
// 	}

// 	unsigned char img1[SizeX][SizeY][bi];
// 	unsigned char img2[SizeX][SizeY][bi];

// 	// read the ".raw" file
// 	if (!(file = fopen(argv[1], "rb"))) {
// 		cout << "Cannot open file: " << argv[1] << endl;
// 		exit(1);
// 	}
// 	fread(img1, sizeof(unsigned char), SizeX*SizeY*bi, file);
// 	fclose(file);

// 	// read 2nd image
// 	if (!(file = fopen(argv[2], "rb"))) {
// 		cout << "Cannot open file: " << argv[2] << endl;
// 		exit(1);
// 	}
// 	fread(img2, sizeof(unsigned char), SizeX*SizeY*bi, file);
// 	fclose(file);

// 	// transform to Mat
// 	Mat img_1;
// 	img_1.create(SizeX, SizeY, CV_8UC3);
// 	memcpy(img_1.data, img1, SizeX*SizeY*3);
// 	//free(img1);

// 	Mat img_2;
// 	img_2.create(SizeX, SizeY, CV_8UC3);
// 	memcpy(img_2.data, img2, SizeX*SizeY*3);
// 	//free(img2);

// 	// RGB to BGR
// 	Mat Image1(SizeX, SizeY, CV_8UC3), Image2(SizeX, SizeY, CV_8UC3);
// 	int from_to[] = { 0,2,1,1,2,0 };
// 	mixChannels(&img_1, 1, &Image1, 1, from_to, 3);
// 	mixChannels(&img_2, 1, &Image2, 1, from_to, 3);

// 	/*Mat img_1 = imread(argv[1], IMREAD_GRAYSCALE);
// 	Mat img_2 = imread(argv[2], IMREAD_GRAYSCALE);*/

// 	// to gray
// 	//cvtColor(Image1, Image1, COLOR_BGR2GRAY);


// 	if (!Image1.data || !Image2.data)
// 	{
// 		std::cout << " --(!) Error reading images " << std::endl; return -1;
// 	}

// 	//-- Step 1: Detect the keypoints using SURF Detector
// 	int minHessian = 400;

// 	Ptr<SIFT> detector = SIFT::create(minHessian);

// 	std::vector<KeyPoint> keypoints_1, keypoints_2;

// 	detector->detect(Image1, keypoints_1);
// 	detector->detect(Image2, keypoints_2);

// 	//-- Draw keypoints
// 	Mat img_keypoints_1; Mat img_keypoints_2;

// 	drawKeypoints(Image1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
// 	drawKeypoints(Image2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

// 	//-- Show detected (drawn) keypoints
// 	imshow("Keypoints 1", img_keypoints_1);
// 	imshow("Keypoints 2", img_keypoints_2);


// 	//SiftDescriptorExtractor siftDescriptor;
// 		Mat imageDesc1, imageDesc2;
// 		//siftDescriptor.compute(Image1, keypoints_1, imageDesc1);
// 		//siftDescriptor.compute(Image2, keypoints_2, imageDesc2);

// detector->compute(Image1, keypoints_1, imageDesc1 ); 
// detector->compute(Image2, keypoints_2, imageDesc2);


// 		// keypoints match
// 		BFMatcher matcher(NORM_L2);
// 		vector<DMatch> matchePoints;
// 		matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
// 		vector<DMatch> good_scale_Points;
// 		//matchePoints = sorted(matchePoints, key = lambda x:x.distance);
// 		float min=1000;
// 		int count=0;
// 		for (int i = 0; i < imageDesc1.rows; i++) {
// 			// cout<<i<<" "<<matchePoints[i].distance<<" "<<matchePoints[i].imgIdx<<endl;
// 			if(matchePoints[i].distance<min){
// 				min=matchePoints[i].distance;
// 				count=i;
// 			}
			
// 		}
// 		//cout<<maxDiameter;

// 		good_scale_Points.push_back(matchePoints[count]);
// float s=5;
// 		for (int i = 0; i < imageDesc1.rows; i++) {
// 			// cout<<i<<" "<<matchePoints[i].distance<<" "<<matchePoints[i].imgIdx<<endl;
// 			if(matchePoints[i].distance<min*s){
// 				good_scale_Points.push_back(matchePoints[i]);
// 			}
			
// 		}


// 		Mat imageOutput;
// 		drawMatches(Image1, keypoints_1, Image2, keypoints_2, good_scale_Points, imageOutput, Scalar::all(-1), Scalar::all(-1),
//                  vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
// 		// DEFAULT 
// 		// DRAW_OVER_OUTIMG 
// 		// NOT_DRAW_SINGLE_POINTS 
// 		// DRAW_RICH_KEYPOINTS 
// 		//cout<<imageDesc1.rows<<" "<<imageDesc2.rows<<" "<<keypoints_1.size();
		

// 		namedWindow("Mathch Points", 0);
// 		imwrite("Mathch Points_2.jpg", imageOutput);
// 		// waitKey(0);

// 	return 0;
// }

// /** @function readme */
// void readme()
// {
// 	std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl;
// }


//======================================== following: largest diameter =====================



// Name: Yan Jiao
// USC ID: 6419057887
// USC Email: yanjiao@usc.edu
// SIFT

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <string>

#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

//#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv2/nonfree/nonfree.hpp>
//initModule_nonfree();
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

void readme();

/** @function main */
int main(int argc, char** argv)
{
	FILE *file;
	const int SizeX = 1024;
	const int SizeY = 768;
	const int bi = 3;

	if (argc != 3)
	{
		readme(); return -1;
	}

	unsigned char img1[SizeX][SizeY][bi];
	unsigned char img2[SizeX][SizeY][bi];

	// read the ".raw" file
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(img1, sizeof(unsigned char), SizeX*SizeY*bi, file);
	fclose(file);

	// read 2nd image
	if (!(file = fopen(argv[2], "rb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fread(img2, sizeof(unsigned char), SizeX*SizeY*bi, file);
	fclose(file);

	// transform to Mat
	Mat img_1;
	img_1.create(SizeX, SizeY, CV_8UC3);
	memcpy(img_1.data, img1, SizeX*SizeY*3);
	//free(img1);

	Mat img_2;
	img_2.create(SizeX, SizeY, CV_8UC3);
	memcpy(img_2.data, img2, SizeX*SizeY*3);
	//free(img2);

	// RGB to BGR
	Mat Image1(SizeX, SizeY, CV_8UC3), Image2(SizeX, SizeY, CV_8UC3);
	int from_to[] = { 0,2,1,1,2,0 };
	mixChannels(&img_1, 1, &Image1, 1, from_to, 3);
	mixChannels(&img_2, 1, &Image2, 1, from_to, 3);

	/*Mat img_1 = imread(argv[1], IMREAD_GRAYSCALE);
	Mat img_2 = imread(argv[2], IMREAD_GRAYSCALE);*/

	// to gray
	//cvtColor(Image1, Image1, COLOR_BGR2GRAY);


	if (!Image1.data || !Image2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	Ptr<SIFT> detector = SIFT::create(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector->detect(Image1, keypoints_1);
	detector->detect(Image2, keypoints_2);

	//-- Draw keypoints
	Mat img_keypoints_1; Mat img_keypoints_2;

	drawKeypoints(Image1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(Image2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1", img_keypoints_1);
	imshow("Keypoints 2", img_keypoints_2);


	//SiftDescriptorExtractor siftDescriptor;
		Mat imageDesc1, imageDesc2;
		//siftDescriptor.compute(Image1, keypoints_1, imageDesc1);
		//siftDescriptor.compute(Image2, keypoints_2, imageDesc2);

detector->compute(Image1, keypoints_1, imageDesc1 ); 
detector->compute(Image2, keypoints_2, imageDesc2);


		// keypoints match
		BFMatcher matcher(NORM_L2);
		vector<DMatch> matchePoints;
		matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
		vector<DMatch> largest_scale_Points;
		//matchePoints = sorted(matchePoints, key = lambda x:x.distance);
		float maxDiameter=0;
		int count=0;
		for (int i = 0; i < imageDesc1.rows; i++) {
			// cout<<i<<" "<<matchePoints[i].distance<<" "<<matchePoints[i].imgIdx<<endl;
			if(keypoints_1[i].size>maxDiameter){
				maxDiameter=keypoints_1[i].size;
				count=i;
			}
			
		}
		cout<<maxDiameter;

		largest_scale_Points.push_back(matchePoints[count]);


		Mat imageOutput;
		drawMatches(Image1, keypoints_1, Image2, keypoints_2, largest_scale_Points, imageOutput, Scalar::all(-1), Scalar::all(-1), 
					vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		//cout<<imageDesc1.rows<<" "<<imageDesc2.rows<<" "<<keypoints_1.size();
		

		namedWindow("Mathch Points", 0);
		imwrite("Mathch Points_diameter.jpg", imageOutput);
		// waitKey(0);

	return 0;
}

/** @function readme */
void readme()
{
	std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl;
}


