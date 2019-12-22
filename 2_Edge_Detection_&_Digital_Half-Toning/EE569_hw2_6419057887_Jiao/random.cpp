// Name: Yan Jiao
// USC ID Number: 6419057887
// USC e-mail: yanjiao@usc.edu
// Submission date: 2/12/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
	// declear random number
	int R = 0;
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++) {
	  	R = rand() % 256;
			//cout << R << "  ";
			if (Imagedata[i][j][0] < R) {
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
