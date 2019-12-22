// Name: Yan Jiao
// USC ID: 6419057887
// USC Email: yanjiao@usc.edu
// Texture Segmentation

#include "stdafx.h"
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;

// padding function
vector<vector<double>> pad(vector<vector<double>> ori, int s, int padNum) {
	vector<vector<double>> after(s + padNum*2, vector<double>(s + padNum*2));
	for (int i = 0; i < s; i++) {
		// middle are the same
		for (int ii = 0; ii < s; ii++) {
			for (int jj = 0; jj < s; jj++) {
				after[ii + 2][jj + 2] = ori[ii][jj];
			}
		}
		// padding up and bottom
		for (int j = 0; j < padNum; j++) {
			for (int m = 0; m < s; m++) {
				after[j][m + 2] = ori[padNum - j][m];
				after[s - 1 - j + 4][m + 2] = ori[s - 1 - padNum + j][m];
			}
		}
		// padding left and right
		for (int jj = 0; jj < padNum; jj++) {
			for (int m = 0; m < s + 4; m++) {
				after[m][jj] = after[m][2 * padNum - jj];
				after[m][s + 4 - 1 - jj] = after[m][s + 4 - 1 - 2 * padNum + jj];
			}
		}
	}
	return after;
}
// histogram_equalization
vector<vector<double>> histogram_equalization(vector<vector<unsigned char>> img, int s) {
	vector<vector<double>> new_img(s, vector<double>(s));
	vector<vector<int>> new_order(256);
	for (int i = 0; i<s; i++) {
		for (int j = 0; j<s; j++) {
			double k = img[i][j];
			if (k >= 0 && k <= 255) {
				new_order[k].push_back(i*s + j);
			}
		}
	}
	double prob[256];
	double result[256];
	for (int i = 0; i<256; i++) {
		prob[i] = (double)new_order[i].size() / pow(s, 2);
	}
	result[0] = prob[0] * 255;
	for (int i = 1; i<256; i++) {
		prob[i] += prob[i - 1];
		result[i] = prob[i] * 255;
	}
	for (int i = 0; i<s; i++) {
		for (int j = 0; j<s; j++) {
			new_img[i][j] = result[(int)img[i][j]];
		}
	}

	return new_img;
}
// fliter the image
vector<vector<double>> filt(vector<vector<double>> img, int s, vector<vector<int>> f) {
	vector<vector<double>> filtered(s,vector<double>(s));
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			double temp = 0;
			for (int ii = -2; ii < 3; ii++) {
				for (int jj = -2; jj < 3; jj++) {
					temp += f[ii + 2][jj + 2] * img[i + ii + 2][j + jj + 2];
				}
			}
			filtered[i][j] = temp;
		}
	}
	return filtered;
}
// calculate each pixel's energy
vector<vector<double>> pixelEnergy(vector<vector<double>> img, int s, int w) {
	int padN = w / 2;
	vector<vector<double>> padded(s + 2*padN, vector<double>(s + 2*padN));
	vector<vector<double>> pEnergy(s, vector<double>(s));
	padded = pad(img, s, padN);
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			double temp = 0;
			for (int ii = -padN; ii <= padN; ii++) {
				for (int jj = -padN; jj <= padN; jj++) {
					temp += pow(padded[i + ii + padN][j + jj + padN], 2);
				}
			}
			pEnergy[i][j] = temp;
		}
	}
	return pEnergy;
}

int main(int argc, char *argv[])
{
	// 1D Kernel for 5*5 Laws Filters
	int Kernel[5][5] = {
		{ 1, 4, 6, 4, 1 },
		{ -1, -2, 0, 2, 1 },
		{ -1, 0, 2, 0, -1 },
		{ -1, 2, 0, -2, 1 },
		{ 1, -4, 6, -4, 1 },
	};
	// Define file pointer and variables
	FILE *file;
	const int BytesPerPixel = 1;
	const int Size = 510;
	const int NumF = 25;

	// Check for proper syntax
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Allocate image data array
	unsigned char ImageMattemp[Size][Size][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImageMattemp, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
	
	vector<vector<double>> imagePadIn(Size + 4, vector<double>(Size + 4));
	vector<vector<double>> imagePadOut(Size + 4, vector<double>(Size + 4));
	
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			imagePadIn[i][j] = ImageMattemp[i][j][0];
		}
	}
	// padding 2 lines each side
	imagePadOut = pad(imagePadIn, Size, 2);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	// total 25 filters
	int FilterIndex = 0;
	vector<vector<vector<int>>> Laws(25,vector<vector<int>>(5,vector<int>(5)));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					Laws[FilterIndex][m][n] = Kernel[i][m] * Kernel[j][n];
				}
			}
			FilterIndex++;
		}
	}

	cout << "*"<<endl;

	// filter 25 times
	vector<vector<vector<double>>> filtered_Out(25,vector<vector<double>>(Size,vector<double>(Size)));
	for (int i = 0; i < 25; i++) {
		filtered_Out[i] = filt(imagePadOut, Size, Laws[i]);
	}

	// pixel energy
	vector<vector<vector<double>>> pEnergy(25, vector<vector<double>>(Size, vector<double>(Size)));
	for (int i = 0; i < 25; i++) {
		pEnergy[i] = pixelEnergy(filtered_Out[i], Size, 9);
	}

	// leave the LL filter
	vector<vector<vector<double>>> pEner(24, vector<vector<double>>(Size, vector<double>(Size)));
	for (int i = 0; i < 24; i++) {
		pEner[i] = pEnergy[i+1];
	}

	// k means
	Mat data_pts(Size*Size, 24, CV_32F);
	int flag = 0;
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			for (int m = 0; m < 24; m++) {
				data_pts.at<float>(flag, m) = pEner[m][i][j];
			}
			flag++;
		}
	}
	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001);
	int k = 7;
	int numberOfAttempts = 3;
	Mat labels, centers;
	kmeans(data_pts, k, labels, criteria, numberOfAttempts, KMEANS_PP_CENTERS, centers);

	unsigned char OUT[Size][Size][1];
	flag = 0;
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			switch (labels.at<int>(flag)) {
			case 0:
				OUT[i][j][0] = 0;
				break;
			case 1:
				OUT[i][j][0] = 42;
				break;
			case 2:
				OUT[i][j][0] = 84;
				break;
			case 3:
				OUT[i][j][0] = 126;
				break;
			case 4:
				OUT[i][j][0] = 168;
				break;
			case 5:
				OUT[i][j][0] = 210;
				break;
			case 6:
				OUT[i][j][0] = 255;
				break;
			}
			flag++;
		}
	}

	











	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(OUT, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	//// hold the command window
	//string exitflag("\n");
	//cout << "Try New Quit Processing. Start Test..." << endl;
	//cout << "Press Q or q key to quit..." << endl;
	//getline(cin, exitflag);
	//while (exitflag.find('q') == string::npos && exitflag.find('Q') == string::npos)
	//{
	//	cout << "Press Q or q key to quit..." << endl;
	//	getline(cin, exitflag);
	//}


	return 0;
}