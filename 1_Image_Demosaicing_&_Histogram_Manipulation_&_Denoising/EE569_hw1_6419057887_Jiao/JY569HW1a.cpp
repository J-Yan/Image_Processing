// JY569HW1.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int SizeX = 300;
	int SizeY = 390;

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

	/////////////// extend the image data array, 1px outer /////////////////////////
  unsigned char Temp[SizeX+2][SizeY+2][BytesPerPixel];
  Temp[0][0][0] = Imagedata[1][1][0];
  Temp[0][SizeY+1][0] = Imagedata[1][SizeY-2][0];
  Temp[SizeX+1][0][0] = Imagedata[SizeX-2][1][0];
  Temp[SizeX+1][SizeY+1][0] = Imagedata[SizeX-2][SizeY-2][0];
  // top
  for (int i=1; i<=SizeY; i++) {
    Temp[0][i][0] = Imagedata[1][i-1][0];
  }
  // left
  for (int i=1; i<=SizeX; i++) {
    Temp[i][0][0] = Imagedata[i-1][1][0];
  }
  // right
  for (int i=1; i<=SizeX; i++) {
    Temp[i][SizeY+1][0] = Imagedata[i-1][SizeY-2][0];
  }
  // bottom
  for (int i=1; i<=SizeY; i++) {
    Temp[SizeX+1][i][0] = Imagedata[SizeX-2][i-1][0];
  }

  for (int i=1; i<=SizeX; i++) {
    for (int j=1; j<=SizeY; j++) {
      Temp[i][j][0] = Imagedata[i-1][j-1][0];
    }
  }

	/////////////////// output image data array //////////////////////////////
	unsigned char ImagedataOut[SizeX][SizeY][BytesPerPixel*3];
	for (int i=1; i<=SizeX; i++) {
    for (int j=1; j<=SizeY; j++) {
        if ((i+j)%2 == 0) {  // already has green
					if (i%2 == 1) {
						ImagedataOut[i-1][j-1][0] = (Temp[i][j-1][0] + Temp[i][j+1][0]) / 2;
						ImagedataOut[i-1][j-1][1] = Temp[i][j][0];
						ImagedataOut[i-1][j-1][2] = (Temp[i-1][j][0] + Temp[i+1][j][0]) / 2;
					}
        	else {
						ImagedataOut[i-1][j-1][2] = (Temp[i][j-1][0] + Temp[i][j+1][0]) / 2;
						ImagedataOut[i-1][j-1][1] = Temp[i][j][0];
						ImagedataOut[i-1][j-1][0] = (Temp[i-1][j][0] + Temp[i+1][j][0]) / 2;
					}
        }
        else if (i%2 == 1) { // red
          ImagedataOut[i-1][j-1][0] = Temp[i][j][0];
          ImagedataOut[i-1][j-1][1] = (Temp[i][j-1][0] + Temp[i][j+1][0] + Temp[i-1][j][0] + Temp[i+1][j][0]) / 4;
          ImagedataOut[i-1][j-1][2] = (Temp[i-1][j-1][0] + Temp[i+1][j+1][0] + Temp[i-1][j+1][0] + Temp[i+1][j-1][0]) / 4;
        }
        else { // blue
          ImagedataOut[i-1][j-1][0] = (Temp[i-1][j-1][0] + Temp[i+1][j+1][0] + Temp[i-1][j+1][0] + Temp[i+1][j-1][0]) / 4;
          ImagedataOut[i-1][j-1][1] = (Temp[i-1][j][0] + Temp[i+1][j][0] + Temp[i][j-1][0] + Temp[i][j+1][0]) / 4;
          ImagedataOut[i-1][j-1][2] = Temp[i][j][0];
        }
    }
  }

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ImagedataOut, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel*3, file);
	fclose(file);

    return 0;
}
