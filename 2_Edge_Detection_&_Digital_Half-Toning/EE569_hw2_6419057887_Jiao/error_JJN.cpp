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
	// set threshold
	int T = 128;
	// diffusion matrix
	//-----Floyd-Steinberg-----
	// int ED[3][3] = {
	// 	0, 0, 0,
	// 	0, 0, 7,
	// 	3, 5, 1
	// };
	// int DivCo = 16;
	//-----JJN-----
	// int ED[5][5] = {
	// 	0, 0, 0, 0, 0,
	// 	0, 0, 0, 0, 0,
	// 	0, 0, 0, 7, 5,
	// 	3, 5, 7, 5, 3,
	// 	1, 3, 5, 3, 1
	// };
	// int DivCo = 48;
	//-----Stucki-----
	int ED[5][5] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 8, 4,
		2, 4, 8, 4, 2,
		1, 2, 4, 2, 1
	};
	int DivCo = 42;


	// error diffusion begin
	// serpentine flag
	int f = 1;
	// Error
	int e = 0;
	//int  ii =0;
	for (int i = 0; i < SizeX; i++) {
		f = abs(1-f);
		//cout<< i <<endl;
		for (int j = 0; j < SizeY; j++) {

			//if(i==399)cout<< j <<" ";
			if (f == 0) {
				//cout <<f<<endl;
				if (Imagedata[i][j][0] <= T) {
					ImagedataOut[i][j][0] = 0;
					//cout<<Imagedata[i][j][0]<<" ";
				}
				else {
					ImagedataOut[i][j][0] = 255;
				}
				e = Imagedata[i][j][0] - ImagedataOut[i][j][0];
				if (j+1<SizeY) {
					Imagedata[i][j+1][0] = Imagedata[i][j+1][0] + e * ED[2][3] / DivCo;
				}
				if (j+2<SizeY) {
					Imagedata[i][j+2][0] = Imagedata[i][j+2][0] + e * ED[2][4] / DivCo;
				}
				for (int m = i+1; m <= i+2; m++) {
					for (int n = j-2; n <= j+2; n++){
						if (m<SizeX&&n>=0&&n<SizeY) Imagedata[m][n][0] += e * ED[2+m-i][2+n-j] / DivCo;
					}
				}
			} // if end


			else {
				if ((int)Imagedata[i][SizeY-j-1][0] <= T) {
					ImagedataOut[i][SizeY-j-1][0] = 0;
				}
				else {
					ImagedataOut[i][SizeY-j-1][0] = 255;
				}
				e = Imagedata[i][SizeY-j-1][0] - ImagedataOut[i][SizeY-j-1][0];
				if (SizeY-j-2>=0){
					Imagedata[i][SizeY-j-2][0] += e * ED[2][3] / DivCo;
				}
				if (SizeY-j-3>=0){
					Imagedata[i][SizeY-j-3][0] += e * ED[2][4] / DivCo;
				}
				for (int m = i+1; m <= i+2; m++) {
					for (int n = (SizeY-j-1)-2; n <= (SizeY-j-1)+2; n++){
						if (m<SizeX&&n>=0&&n<SizeY) Imagedata[m][n][0] += e * ED[2+m-i][2+n-(SizeY-j-1)] / DivCo;
					}
				}
			} // else end
		}
	}
cout <<"!";

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ImagedataOut, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

	return 0;
}
