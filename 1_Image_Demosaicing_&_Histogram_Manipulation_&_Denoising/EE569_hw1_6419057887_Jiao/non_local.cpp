// JY569HW1.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;
void Extend(unsigned char *in, unsigned char *out, int extNum, int row, int col){ // row, col -> ori
	// top
	for (int i = 1; i <= extNum; i++) {
		for (int j = 0; j < col; j++) {
			out[(extNum-i)*(col+2*extNum) + j + extNum]=(in[i*col + j]);
			//out[(extNum-i)*(col+2*extNum) + j + extNum]=0;
		}
	}
	// bottom
	for (int i = 1; i <= extNum; i++) {
		for (int j = 0; j < col; j++) {
			out[(row+extNum+i-1)*(col+2*extNum) + j + extNum]=(in[(row-i-1)*col + j]);
		}
	}
	// middle
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			out[(extNum+i)*(col+2*extNum) + (j + extNum)]=in[i*col + j];
		}
	}
	// left
	for (int i = 1; i <= extNum; i++) {
		for (int j = 0; j < col; j++) {
			out[j*(col + 2*extNum) + (extNum - i)]=out[j*(col+extNum*2) + (i + extNum)];
		}
	}
	// right
	for (int i = 1; i <= extNum; i++) {
		for (int j = 0; j < col; j++) {
			out[j*(col + 2*extNum) + (col + extNum- 1 + i)]=out[j*(col+extNum*2) + (col + extNum- 1 - i)];
		}
	}
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int SizeX = 256;
	int SizeY = 256;

	// Check for proper syntax
	if (argc < 3) {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4) {
		BytesPerPixel = 1; // default is grey image
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5) {
			SizeX = atoi(argv[4]);
		}
	}

	// Allocate image data array

  unsigned char Imagedata[SizeX][SizeY][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	/////////////// extend the image data array, 2px outer /////////////////////////
	int SizeF = 5; // <-- change filter size, odd!
	int Re = 21; // search region size
	int PadNum = (Re - 1) / 2;
  unsigned char Temp[SizeX + Re - 1][SizeY + Re - 1][BytesPerPixel]; // Extended image
	Extend(&Imagedata[0][0][0], &Temp[0][0][0], PadNum, SizeX, SizeY);

	//----------------- output image data array
	unsigned char ImagedataOut[SizeX][SizeY][BytesPerPixel];
	//int grey;
	float sig = 50;
	float h = 10;
	//float w;

  for (int i = 0; i < SizeX; i++) {

		for (int j = 0; j < SizeY; j++) {

			float total = 0;
			float fTotal = 0;
			float f = 0;
			for (int m = i+(SizeF-1)/2; m < i+Re-(SizeF+1)/2; m++) {
				for (int n = j+(SizeF-1)/2; n < j+Re-(SizeF+1)/2; n++) {
					int n1;
					int n2;
					float g;
					float gsum=0;
					for (int w = m-(SizeF-1)/2; w < m+(SizeF-1)/2; w++) {
						for (int v = n-(SizeF-1)/2; v < n+(SizeF-1)/2; v++) {
							 n1 = (w-m);
							 n2 = (v-n);
							 g = 1/sqrt(2*3.14)/sig*exp(-(n1*n1+n2*n2)/2/sig/sig);
							 gsum += g*pow(Temp[i+(SizeF-1)/2-n1][j+(SizeF-1)/2-n2][0]-Temp[w][v][0],2);
						}
					}
					f = exp(-gsum/h/h);
					fTotal += f;
					//cout << w<<" "<<wTotal<<" ";
					total += Temp[m][n][0] * f;
					//cout << total<<endl;
				}
			}
			ImagedataOut[i-5][j-5][0] = floor(total / fTotal);
			//cout<<(int)ImagedataOut[i][j][0]<<endl;
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ImagedataOut, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	//fwrite(Temp, sizeof(unsigned char), (SizeX+2*PadNum)*(SizeY+2*PadNum)*BytesPerPixel, file);
	fclose(file);

    return 0;
}
