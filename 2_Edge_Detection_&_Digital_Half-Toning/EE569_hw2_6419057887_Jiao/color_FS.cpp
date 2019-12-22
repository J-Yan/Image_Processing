// Name: Yan Jiao
// USC ID Number: 6419057887
// USC e-mail: yanjiao@usc.edu
// Submission date: 2/12/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int SizeX = 375;
	int SizeY = 500;

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
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			SizeX = atoi(argv[4]);
		}
	}

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
	// set threshold
	int T = 128;
	// diffusion matrix
	//-----Floyd-Steinberg-----
	int ED[3][3] = {
		0, 0, 0,
		0, 0, 7,
		3, 5, 1
	};
	int DivCo = 16;

	// error diffusion begin
	// serpentine flag
	int f = 1;
	// Error
	int e = 0;
	// additive to subtractive
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++) {
			for (int c = 0; c < 3; c++) {
				Imagedata[i][j][c] = 255 - Imagedata[i][j][c];
			}
		}
	}



	for (int c = 0; c < 3; c++){
		for (int i = 0; i < SizeX; i++) {
			f = abs(1-f);
			//cout<< i <<endl;
			for (int j = 0; j < SizeY; j++) {

				//if(i==399)cout<< j <<" ";
				if (f == 0) {
					//cout <<f<<endl;
					if (Imagedata[i][j][c] <= T) {
						ImagedataOut[i][j][c] = 0;
						//cout<<Imagedata[i][j][0]<<" ";
					}
					else {
						ImagedataOut[i][j][c] = 255;
					}
					e = Imagedata[i][j][c] - ImagedataOut[i][j][c];
					if (j+1<SizeY) {
						//cout<<Imagedata[i][j+1][0]<<" ";
						Imagedata[i][j+1][c] = Imagedata[i][j+1][c] + e * ED[1][2] / DivCo;
						//cout<<e*ED[1][2]/DivCo<<" ";
						//cout<<Imagedata[i][j+1][0]<<endl;
					}
					if ((i+1<SizeX)&&(j-1>=0)) {
						Imagedata[i+1][j-1][c] = Imagedata[i+1][j-1][c] + e * ED[2][0] / DivCo;
					}
					if (i+1<SizeX) {
						Imagedata[i+1][j][c] = Imagedata[i+1][j][c] + e * ED[2][1] / DivCo;
					}
					if ((i+1<SizeX)&&(j+1<SizeY)) {
						Imagedata[i+1][j+1][c] = Imagedata[i+1][j+1][c] + e * ED[2][2] / DivCo;
					}
				}


				else {
					// if(i==399)cout<< j <<" ";
					//cout <<f<<endl;
					if ((int)Imagedata[i][SizeY-j-1][c] <= T) {
						ImagedataOut[i][SizeY-j-1][c] = 0;
					}
					else {
						ImagedataOut[i][SizeY-j-1][c] = 255;
					}
	e = Imagedata[i][SizeY-j-1][c] - ImagedataOut[i][SizeY-j-1][c];
					if (SizeY-j-2>=0){
						Imagedata[i][SizeY-j-2][c] = Imagedata[i][SizeY-j-2][c] + e * ED[1][2] / DivCo;
					}
					if ((i+1<SizeX)&&(SizeY-j<SizeY)){
						Imagedata[i+1][SizeY-j][c] = Imagedata[i+1][SizeY-j][c] + e * ED[2][0] / DivCo;
					}
					if (i+1<SizeX){
						Imagedata[i+1][SizeY-j-1][c] = Imagedata[i+1][SizeY-j-1][c] + e * ED[2][1] / DivCo;
					}
					if ((i+1<SizeX)&&(SizeY-j-2>=0)){
						Imagedata[i+1][SizeY-j-2][c] = Imagedata[i+1][SizeY-j-2][c] + e * ED[2][2] / DivCo;
					}

				}
			}
		}
	}

cout <<"!";

for (int i = 0; i < SizeX; i++) {
	for (int j = 0; j < SizeY; j++) {
		for (int c = 0; c < 3; c++) {
			ImagedataOut[i][j][c] = 255 - ImagedataOut[i][j][c];
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
