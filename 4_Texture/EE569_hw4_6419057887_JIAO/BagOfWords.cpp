
// Submission date: 2019/03/15


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int width = 28;
int height = 28;
int channel = 1;



void GenerateSiftDescripter(Mat img, BOWKMeansTrainer& bag,vector<Mat> &histograms);



int main (int argc, char *argv[]){

	FILE *file;

	if (argc != 4 ){
		cout<<"Syntax Eoor - Incorrect Parameter Usage:" << endl;
		cout << "program_name ./zero_ ./one_ eight.raw" << endl;
		return 0;
	}
	// Make a bag of words trainer
	BOWKMeansTrainer codebook(2);
	vector<Mat> histograms;

	unsigned char zeros[5][width][height];
	unsigned char ones[5][width][height];
	unsigned char eight_img[width][height];
	double resize_effe = 10.0;
	// read training image
	for(int i=0; i<5; i++){
		// for 0 image sequence
		string filename = argv[1]+to_string(i+1)+".raw";
		char char_array[filename.length()+1];
		strcpy(char_array,filename.c_str());
		if (!(file=fopen(char_array,"rb"))) {
			cout << "Cannot open file: " <<endl;
			exit(1);
		}
		fread(zeros[i], sizeof(unsigned char), width*height, file);
		fclose(file);
		// transform to MAT
		Mat zeroMat(width,height,CV_8UC1),zeroResize;
		memcpy(zeroMat.data, zeros[i], width*height);
		// scaling up the image to make sure the descriptor is not empty
		resize(zeroMat,zeroResize,Size_<int>(),resize_effe,resize_effe);
		GenerateSiftDescripter(zeroResize,codebook,histograms);

		// for 1 image sequence
		filename = argv[2]+to_string(i+1)+".raw";
		char char_array2[filename.length()+1];
		strcpy(char_array2,filename.c_str());
		if (!(file=fopen(char_array2,"rb"))) {
			cout << "Cannot open file: " <<endl;
			exit(1);
		}
		fread(ones[i], sizeof(unsigned char), width*height, file);
		fclose(file);
		// trandform to MAT
		Mat oneMat(width,height,CV_8UC1),oneResize;
		memcpy(oneMat.data, ones[i], width*height);
		resize(oneMat,oneResize,Size_<int>(),resize_effe,resize_effe);
		GenerateSiftDescripter(oneResize,codebook,histograms);


	}



	//cluster
	Mat codewords = codebook.cluster();

	Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
	Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
	BOWImgDescriptorExtractor dextract = BOWImgDescriptorExtractor(extractor, matcher);
	dextract.setVocabulary(codewords);
	Mat codeBin = dextract.getVocabulary();


	// read test image
	if (!(file=fopen(argv[3],"rb"))) {
		cout << "Cannot open file: " << "river2.raw"<<endl;
		exit(1);
	}
	fread(eight_img, sizeof(unsigned char), width*height, file);
	fclose(file);
	Mat eightMat(width,height,CV_8UC1),eightResize;
	memcpy(eightMat.data, eight_img, width*height);
	resize(eightMat,eightResize,Size_<int>(),resize_effe,resize_effe);
	int minHessian = 40;
	Ptr<SIFT> test_detector = SIFT::create(minHessian);
	vector<KeyPoint> test_keypoints;
	Mat test_descriptor;
	test_detector->detectAndCompute(eightResize, noArray(), test_keypoints, test_descriptor);

	// plot histogram
	vector<int> histg(codeBin.rows,0);
	for (int i=0; i<test_descriptor.rows; i++){
		double dist[codeBin.rows];
		for (int j=0; j<codeBin.rows; j++){
			for (int feat=0; feat<test_descriptor.cols; feat++){
				dist[j] += pow(test_descriptor.at<float>(i,feat)-codeBin.at<float>(j,feat),2);
			}
		}
		//cout<<dist[0]<<" "<<dist[1]<<" "<<distance(dist,min_element(dist,dist+codeBin.rows))<<endl;
		histg[distance(dist,min_element(dist,dist+codeBin.rows))]++;

	}

	cout<<histg[0]<<" "<<histg[1]<<endl;

	for (int l=0; l<histograms.size();l++){
		vector<int> hist(codeBin.rows,0);
		for (int i=0; i<histograms[l].rows; i++){
			double dist[codeBin.rows];
			for (int j=0; j<codeBin.rows; j++){
				for (int feat=0; feat<test_descriptor.cols; feat++){
					dist[j] += pow(histograms[l].at<float>(i,feat)-codeBin.at<float>(j,feat),2);
				}
			}

			hist[distance(dist,min_element(dist,dist+codeBin.rows))]++;

		}
		cout<<hist[0]<<" "<<hist[1]<<endl;
	}






	return 0;


}


void GenerateSiftDescripter(Mat img, BOWKMeansTrainer& bag,vector<Mat> &histograms){
	int minHessian = 40;
	Ptr<SIFT> detector = SIFT::create(minHessian);
	vector<KeyPoint> keypoints;
	Mat descriptor;
	detector->detectAndCompute(img, noArray(), keypoints, descriptor);

	if(!descriptor.empty()){
		histograms.push_back(descriptor);
		bag.add(descriptor);
		// cout<<"1 ";
	}
}
