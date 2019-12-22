// Name: Yan Jiao
// USC ID: 6419057887
// USC Email: yanjiao@usc.edu
// S

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

static int SizeX = 691;
static int SizeY = 550;

// the value which represents black in the binary image
static const unsigned char I = 255;

// Digital Image Processing, 3rd Edition, Table 14.3-1, pp.413,414
// Shrink, Thin, and Skeletonize Conditional Mark Patterns [M = 1 if hit]
static const unsigned char k_conditional_patterns[][9] = {
// S 1
{0,0,I,
0,I,0,
0,0,0},
{I,0,0,
0,I,0,
0,0,0},
{0,0,0,
0,I,0,
I,0,0},
{0,0,0,
0,I,0,
0,0,I},

// S 2
{0,0,0,
0,I,I,
0,0,0},
{0,I,0,
0,I,0,
0,0,0},
{0,0,0,
I,I,0,
0,0,0},
{0,0,0,
0,I,0,
0,I,0},

// S 3
{0,0,I,
0,I,I,
0,0,0},
{0,I,I,
0,I,0,
0,0,0},
{I,I,0,
0,I,0,
0,0,0},
{I,0,0,
I,I,0,
0,0,0},
{0,0,0,
I,I,0,
I,0,0},
{0,0,0,
0,I,0,
I,I,0},
{0,0,0,
0,I,0,
0,I,I},
{0,0,0,
0,I,I,
0,0,I},
/*
// TK 4
{0,I,0,
0,I,I,
0,0,0},
{0,I,0,
I,I,0,
0,0,0},
{0,0,0,
I,I,0,
0,I,0},
{0,0,0,
0,I,I,
0,I,0},
*/
// STK 4
{0,0,I,
0,I,I,
0,0,I},
{I,I,I,
0,I,0,
0,0,0},
{I,0,0,
I,I,0,
I,0,0},
{0,0,0,
0,I,0,
I,I,I},

// ST 5
{I,I,0,
0,I,I,
0,0,0},
{0,I,0,
0,I,I,
0,0,I},
{0,I,I,
I,I,0,
0,0,0},
{0,0,I,
0,I,I,
0,I,0},

// ST 5
{0,I,I,
0,I,I,
0,0,0},
{I,I,0,
I,I,0,
0,0,0},
{0,0,0,
I,I,0,
I,I,0},
{0,0,0,
0,I,I,
0,I,I},

// ST 6
{I,I,0,
0,I,I,
0,0,I},
{0,I,I,
I,I,0,
I,0,0},

// STK 6
{I,I,I,
0,I,I,
0,0,0},
{0,I,I,
0,I,I,
0,0,I},
{I,I,I,
I,I,0,
0,0,0},
{I,I,0,
I,I,0,
I,0,0},
{I,0,0,
I,I,0,
I,I,0},
{0,0,0,
I,I,0,
I,I,I},
{0,0,0,
0,I,I,
I,I,I},
{0,0,I,
0,I,I,
0,I,I},

// STK 7
{I,I,I,
0,I,I,
0,0,I},
{I,I,I,
I,I,0,
I,0,0},
{I,0,0,
I,I,0,
I,I,I},
{0,0,I,
0,I,I,
I,I,I},

// STK 8
{0,I,I,
0,I,I,
0,I,I},
{I,I,I,
I,I,I,
0,0,0},
{I,I,0,
I,I,0,
I,I,0},
{0,0,0,
I,I,I,
I,I,I},

// STK 9
{I,I,I,
0,I,I,
0,I,I},
{0,I,I,
0,I,I,
I,I,I},
{I,I,I,
I,I,I,
I,0,0},
{I,I,I,
I,I,I,
0,0,I},
{I,I,I,
I,I,0,
I,I,0},
{I,I,0,
I,I,0,
I,I,I},
{I,0,0,
I,I,I,
I,I,I},
{0,0,I,
I,I,I,
I,I,I},

// STK 10
{I,I,I,
0,I,I,
I,I,I},
{I,I,I,
I,I,I,
I,0,I},
{I,I,I,
I,I,0,
I,I,I},
{I,0,I,
I,I,I,
I,I,I},
/*
// K 11
{I,I,I,
I,I,I,
0,I,I},
{I,I,I,
I,I,I,
I,I,0},
{I,I,0,
I,I,I,
I,I,I},
{0,I,I,
I,I,I,
I,I,I},
*/
};

// Digital Image Processing, 3rd Edition, Table 14.3-I, p. 414
// Shrink, Thin, and Skeletonize Unconditional Mark Patterns
// P(M,M0,M1,M2,M3,M4,M5,M6,M7) = 1 if hit]
// A ∪ B ∪ C = 1
// A ∪ B = 1
// D = 0 ∪ 1
static const unsigned char M = 1;
static const unsigned char A = 2;
static const unsigned char B = 3;
static const unsigned char C = 4;
static const unsigned char D = 5;

static const unsigned char k_unconditional_patterns[][9] = {
// spur
{0,0,M,
0,M,0,
0,0,0},
{M,0,0,
0,M,0,
0,0,0},
{0,0,0,
0,M,0,
0,M,0},
{0,0,0,
0,M,M,
0,0,0},

// L Cluster
{0,0,M,
0,M,M,
0,0,0},
{0,M,M,
0,M,0,
0,0,0},
{M,M,0,
0,M,0,
0,0,0},
{M,0,0,
M,M,0,
0,0,0},
{0,0,0,
M,M,0,
M,0,0},
{0,0,0,
0,M,0,
M,M,0},
{0,0,0,
0,M,0,
0,M,M},
{0,0,0,
0,M,M,
0,0,M},

// offset
{0,M,M,
M,M,0,
0,0,0},
{M,M,0,
0,M,M,
0,0,0},
{0,M,0,
0,M,M,
0,0,M},
{0,0,M,
0,M,M,
0,M,0},

// spur corner
{0,A,M,
0,M,B,
M,0,0},
{M,B,0,
A,M,0,
0,0,M},
{0,0,M,
A,M,0,
M,B,0},
{M,0,0,
0,M,B,
0,A,M},

// corner clutter
{M,M,D,
M,M,D,
D,D,D},

// tee branch
{D,M,0,
M,M,M,
D,0,0},
{0,M,D,
M,M,M,
0,0,D},
{0,0,D,
M,M,M,
0,M,D},
{D,0,0,
M,M,M,
D,M,0},
{D,M,D,
M,M,0,
0,M,0},
{0,M,0,
M,M,0,
D,M,D},
{0,M,0,
0,M,M,
D,M,D},
{D,M,D,
0,M,M,
0,M,0},

// vee branch
{M,D,M,
D,M,D,
A,B,C},
{M,D,C,
D,M,B,
M,D,A},
{C,B,A,
D,M,D,
M,D,M},
{A,D,M,
B,M,D,
C,D,M},

// diagonal branch
{D,M,0,
0,M,M,
M,0,D},
{0,M,D,
M,M,0,
D,0,M},
{D,0,M,
M,M,0,
0,M,D},
{M,0,D,
0,M,M,
D,M,0},
};

// count each set's number of masks
int NumCond = sizeof(k_conditional_patterns)/sizeof(k_conditional_patterns[0]);
int NumUnCond = sizeof(k_unconditional_patterns)/sizeof(k_unconditional_patterns[0]);

vector<vector<unsigned char>> Shrink(vector<vector<unsigned char>> IP) {
	unsigned char StageOne[SizeX][SizeY];
	unsigned char StageTwo[SizeX][SizeY];
	int loopflag = 1;
	int count = 0;
	while (loopflag == 1) {
		count++;
		if (count > 12) {
			break;
		}
		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++){
				StageOne[i][j] = 0;
			}
		}
		unsigned char nine[9];
		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++){
				if (IP[i][j] == 255) {
					nine[0] = IP[i-1][j-1];
					nine[1] = IP[i-1][j];
					nine[2] = IP[i-1][j+1];
					nine[3] = IP[i][j-1];
					nine[4] = IP[i][j];
					nine[5] = IP[i][j+1];
					nine[6] = IP[i+1][j-1];
					nine[7] = IP[i+1][j];
					nine[8] = IP[i+1][j+1];
					unsigned char flag = 0;
					for (int k = 0; k < NumCond; k++){
						flag = M;
						for (int m = 0; m < 9; m++) {
							if (k_conditional_patterns[k][m] != nine[m]) {
								flag = 0;
								break;
							}
						}
						StageOne[i][j] = flag;
						if (flag == M) {
							break;
						}
					}
				}
			}
		}

		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++){
				StageTwo[i][j] = StageOne[i][j];
			}
		}
		// stage two
		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++){
				if (StageOne[i][j] == M) {
					// loop all masks
					nine[0] = StageOne[i-1][j-1];
					nine[1] = StageOne[i-1][j];
					nine[2] = StageOne[i-1][j+1];
					nine[3] = StageOne[i][j-1];
					nine[4] = StageOne[i][j];
					nine[5] = StageOne[i][j+1];
					nine[6] = StageOne[i+1][j-1];
					nine[7] = StageOne[i+1][j];
					nine[8] = StageOne[i+1][j+1];
					unsigned char flag = 0;
					for (int k = 0; k < NumUnCond; k++){
						flag = M;
						unsigned char ABC = 0;
						unsigned char t = 0;
						for (int m = 0; m < 9; m++) {
							if (k_unconditional_patterns[k][m] > 1 && k_unconditional_patterns[k][m] < 5) {
								ABC = 1;
								if (nine[m] == M) {
									t = 1;
								}
							}
							else if (k_unconditional_patterns[k][m] < 2) {
								if (k_unconditional_patterns[k][m] != nine[m]) {
									flag = 0;
								}
							}
						}
						if (ABC!=t || flag != M) {
							StageTwo[i][j] = 0;
						}
						else {
							StageTwo[i][j] = M;
							break;
						}
					}
				}
			}
		}
	loopflag = 0;
		for (int i = 0; i < SizeX; i++) {
			for (int j = 0; j < SizeY; j++){
				if (StageOne[i][j] == M && StageTwo[i][j] == 0) {
					IP[i][j] = 0;
					loopflag = 1;
				}
			}
		}
	}
return IP;
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;

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

	// Allocate image data array
  unsigned char Image1[SizeX][SizeY][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(Image1, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
int flag = 1;
while (flag ==1) {
	flag =0;
	for (int i = 1; i < SizeX-1; i++) {
		for (int j = 1; j < SizeY-1; j++){
			int c = 0;
			if (Image1[i][j][0] == 0 && Image1[i-1][j][0] == 255 && Image1[i+1][j][0] == 255 && Image1[i][j-1][0] == 255 && Image1[i][j+1][0] == 255) {
				
				if (Image1[i-1][j-1][0] == 0) {

					c++;
				}
				if (Image1[i-1][j+1][0] == 0) {

					c++;
				}
				if (Image1[i+1][j-1][0] == 0) {

					c++;
				}
				if (Image1[i+1][j+1][0] == 0) {

					c++;
				}
				if (c==1 || c==0) {
					Image1[i][j][0] = 255;
					flag =1;
				}
			}
		}
	}
}


	//////////////////////////////////////////////////////////////////////////////////////
	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image1, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

    return 0;
}
