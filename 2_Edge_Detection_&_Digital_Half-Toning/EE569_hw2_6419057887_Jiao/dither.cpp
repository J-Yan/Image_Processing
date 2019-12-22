// Name: Yan Jiao
// USC ID Number: 6419057887
// USC e-mail: yanjiao@usc.edu
// Submission date: 2/12/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
//#include <malloc.h>

using namespace std;

int I2[2][2] = {
	1, 2,
	3, 0
};
// ex.: if the x size of dithering matrix is 8, LoopNum should be 2
int DitherMat(int *Dither, int LoopNum) {
	int XLengthCurr = 2;
	int *DitherSmall;
	DitherSmall=(int*)malloc(sizeof(int) * 1024);
	for (int m = 0; m < 2; m++) {
		for (int n = 0; n < 2; n++) {
			DitherSmall[m*2+n] = I2[m][n];
			Dither[m*2+n] = I2[m][n];
		}
	}

	for (int i = 1; i <= LoopNum; i++) {
		XLengthCurr = pow(2, i+1);
					for (int m = 0; m < XLengthCurr/2; m++) {
						for (int n = 0; n < XLengthCurr/2; n++) {
							Dither[m*XLengthCurr+n] = DitherSmall[m*(XLengthCurr/2)+n]*4+1;
						}
					}
					for (int m = 0; m < XLengthCurr/2; m++) {
						for (int n = XLengthCurr/2; n < XLengthCurr; n++) {
							Dither[m*XLengthCurr+n] = DitherSmall[m*(XLengthCurr/2)+n-(XLengthCurr/2)]*4+2;
						}
					}
					for (int m = XLengthCurr/2; m < XLengthCurr; m++) {
						for (int n = 0; n < XLengthCurr/2; n++) {
							Dither[m*XLengthCurr+n] = DitherSmall[(m-(XLengthCurr/2))*(XLengthCurr/2)+n]*4+3;
						}
					}
					for (int m = XLengthCurr/2; m < XLengthCurr; m++) {
						for (int n = XLengthCurr/2; n < XLengthCurr; n++) {
							Dither[m*XLengthCurr+n] = DitherSmall[(m-(XLengthCurr/2))*(XLengthCurr/2)+n-(XLengthCurr/2)]*4;
						}
					}
					for (int m = 0; m < XLengthCurr; m++) {
						for (int n = 0; n < XLengthCurr; n++) {
							DitherSmall[m*XLengthCurr+n] = Dither[m*XLengthCurr+n];
						}
					}
				}
				for (int m = 0; m < XLengthCurr; m++) {
					for (int n = 0; n < XLengthCurr; n++) {
						Dither[m*XLengthCurr+n] = (Dither[m*XLengthCurr+n]+0.5)/XLengthCurr/XLengthCurr*255;
					}
				}
				free(DitherSmall);
				return 0;
	}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int SizeX = 400;
	int SizeY = 600;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	}
	// else {
	// 	BytesPerPixel = atoi(argv[3]);
	// 	// Check if size is specified
	// 	if (argc >= 5){
	// 		SizeX = atoi(argv[4]);
	// 	}
	// }

	// Allocate image data array
	unsigned char Imagedata[SizeX][SizeY][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	// new a output matrix
  unsigned char ImagedataOut[SizeX][SizeY][BytesPerPixel];
	// generate dithering matrix
	int power = 5; // means dithering matrix size is 2^3 * 2^3
	int DSize = pow(2,power);
	int D[DSize][DSize];

	DitherMat(&D[0][0], power-1);
	for (int i = 0; i < DSize; i++) {
		for (int j = 0; j < DSize; j++) {
			cout << D[i][j] << " ";
		}
		cout << endl;
	}

	//dithering begin
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++){
			if (Imagedata[i][j][0] <= D[i%DSize][j%DSize]) {
				ImagedataOut[i][j][0] = 0;
			}
			else {
				ImagedataOut[i][j][0] = 255;
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ImagedataOut, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

	return 0;
}
