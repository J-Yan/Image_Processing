// Name: Yan Jiao
// USC ID: 6419057887
// USC Email: yanjiao@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
// #include "eigen-eigen-323c052e1731\eigen-eigen-323c052e1731\Eigen\Dense"

using namespace std;
// using namespace Eigen;
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
	// else {
	// 	BytesPerPixel = atoi(argv[3]);
	// 	// Check if size is specified
	// 	if (argc >= 5) {
	// 		SizeX = atoi(argv[4]);
	// 	}
	//}

	// Allocate image data array

  unsigned char ImagePatch1[SizeX][SizeY][BytesPerPixel];
  unsigned char ImagePatch2[SizeX][SizeY][BytesPerPixel];
  unsigned char ImagePatch3[SizeX][SizeY][BytesPerPixel];
  unsigned char ImageLHmiss[SizeX*2][SizeY*2][BytesPerPixel];

	//cout<<ImagePatch1[1][1][0]<<"hgfdsdfghj"<<endl;
	// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImagePatch1, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);
  // Read other Images
  if (!(file = fopen(argv[2], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImagePatch2, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);
  //
  if (!(file = fopen(argv[3], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImagePatch3, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel, file);
	fclose(file);

  if (!(file = fopen(argv[4], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImageLHmiss, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel*4, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
  int a1x = 0, a1y = 0, a2x = 0, a2y = 0, a3x = 0, a3y = 0, a4x = 0, a4y = 0,
      b1x, b1y, b2x, b2y, b3x, b3y, b4x, b4y,
      c1x, c1y, c2x, c2y, c3x, c3y, c4x, c4y;
  int xmin, xmax, ymin, ymax;
  // ImagePatch1
  xmin = SizeX-1;
  xmax = 0;
  ymin = SizeY-1;
  ymax = 0;
	for (int i = 0; i < SizeX; i++) {
    for (int j = 0; j < SizeY; j++){
      if (ImagePatch1[i][j][0] < 245) {
        if ((i == xmin && ImagePatch1[i][j][0] < ImagePatch1[xmin][a1y][0]) || (i < xmin)) {
          a1x = i;
          a1y = j;
          xmin = i;
        }
        if ((i == xmax && ImagePatch1[i][j][0] < ImagePatch1[xmax][a3y][0]) || (i > xmax)) {
          a3x = i;
          a3y = j;
          xmax = i;
        }
        if ((j == ymin && ImagePatch1[i][j][0] < ImagePatch1[a2x][ymin][0]) || (j < ymin)) {
          a2x = i;
          a2y = j;
          ymin = j;
        }
        if ((j == ymax && ImagePatch1[i][j][0] < ImagePatch1[a4x][ymax][0]) || (j > ymax)) {
          a4x = i;
          a4y = j;
          ymax = j;
        }
      }
    }
  }

  // test
  // ImagePatch1[a1x][a1y][0] = 0;
  // ImagePatch1[a2x][a2y][0] = 0;
  // ImagePatch1[a3x][a3y][0] = 0;
  // ImagePatch1[a4x][a4y][0] = 0;
  //
  xmin = SizeX-1;
  xmax = 0;
  ymin = SizeY-1;
  ymax = 0;
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++){
			if (ImagePatch2[i][j][0] < 245) {
				if ((i == xmin && ImagePatch2[i][j][0] < ImagePatch2[xmin][b1y][0]) || (i < xmin)) {
					b1x = i;
					b1y = j;
					xmin = i;
				}
				if ((i == xmax && ImagePatch2[i][j][0] < ImagePatch2[xmax][b3y][0]) || (i > xmax)) {
					b3x = i;
					b3y = j;
					xmax = i;
				}
				if ((j == ymin && ImagePatch2[i][j][0] < ImagePatch2[b2x][ymin][0]) || (j < ymin)) {
					b2x = i;
					b2y = j;
					ymin = j;
				}
				if ((j == ymax && ImagePatch2[i][j][0] < ImagePatch2[b4x][ymax][0]) || (j > ymax)) {
					b4x = i;
					b4y = j;
					ymax = j;
				}
			}
		}
	}
	// test 2
  // ImagePatch2[b1x][b1y][0] = 0;
  // ImagePatch2[b2x][b2y][0] = 0;
  // ImagePatch2[b3x][b3y][0] = 0;
  // ImagePatch2[b4x][b4y][0] = 0;
  //
  xmin = SizeX-1;
  xmax = 0;
  ymin = SizeY-1;
  ymax = 0;
	for (int i = 0; i < SizeX; i++) {
    for (int j = 0; j < SizeY; j++){
      if (ImagePatch3[i][j][0] < 245) {
        if ((i == xmin && ImagePatch3[i][j][0] < ImagePatch3[xmin][c1y][0]) || (i < xmin)) {
          c1x = i;
          c1y = j;
          xmin = i;
        }
        if ((i == xmax && ImagePatch3[i][j][0] < ImagePatch3[xmax][c3y][0]) || (i > xmax)) {
          c3x = i;
          c3y = j;
          xmax = i;
        }
        if ((j == ymin && ImagePatch3[i][j][0] < ImagePatch3[c2x][ymin][0]) || (j < ymin)) {
          c2x = i;
          c2y = j;
          ymin = j;
        }
        if ((j == ymax && ImagePatch3[i][j][0] < ImagePatch3[c4x][ymax][0]) || (j > ymax)) {
          c4x = i;
          c4y = j;
          ymax = j;
        }
      }
    }
  }
	// test 3
	// ImagePatch3[c1x][c1y][0] = 0;
	// ImagePatch3[c2x][c2y][0] = 0;
	// ImagePatch3[c3x][c3y][0] = 0;
	// ImagePatch3[c4x][c4y][0] = 0;
	//
	int flag = 1;
	int wa1x, wa2x, wa3x, wa4x,
			wb1x, wb2x, wb3x, wb4x,
			wc1x, wc2x, wc3x, wc4x,
			wa1y, wa2y, wa3y, wa4y,
			wb1y, wb2y, wb3y, wb4y,
			wc1y, wc2y, wc3y, wc4y;
	for (int i = 0; i < SizeX*2; i++) {
		for (int j = 0; j < SizeY*2; j++) {
			// find the UL white corner of the hole
			if (ImageLHmiss[i][j][0] == 255 && ImageLHmiss[i-1][j][0] !=255 && ImageLHmiss[i][j-1][0] != 255 && ImageLHmiss[i-1][j-1][0] != 255 && ImageLHmiss[i+1][j][0] == 255 && ImageLHmiss[i][j+1][0] == 255) {
				if (flag == 1) {
					wb1x = i;
					wb4x = i;
					wb1y = j;
					wb2y = j;
					int k = i;
					while (ImageLHmiss[k][j][0] == 255) {
						k++;
					}
					wb2x = k-1;
					wb3x = k-1;
					k = j;
					while (ImageLHmiss[i][k][0] == 255) {
						k++;
					}
					wb4y = k-1;
					wb3y = k-1;
					flag++;
					break;
				}
				if (flag == 2) {
					wa1x = i;
					wa4x = i;
					wa1y = j;
					wa2y = j;
					int k = i;
					while (ImageLHmiss[k][j][0] == 255) {
						k++;
					}
					wa2x = k-1;
					wa3x = k-1;
					k = j;
					while (ImageLHmiss[i][k][0] == 255) {
						k++;
					}
					wa4y = k-1;
					wa3y = k-1;
					flag++;
					break;
				}
				if (flag == 3) {
					wc1x = i;
					wc4x = i;
					wc1y = j;
					wc2y = j;
					int k = i;
					while (ImageLHmiss[k][j][0] == 255) {
						k++;
					}
					wc2x = k-1;
					wc3x = k-1;
					k = j;
					while (ImageLHmiss[i][k][0] == 255) {
						k++;
					}
					wc4y = k-1;
					wc3y = k-1;
					//cout<<wc1x<<" "<<wc1y<<" "<<wc2x<<" "<<wc2y<<endl;
				}
			}
		}
	}
	// test 4
	// ImageLHmiss[wa1x][wa1y][0]=0;
	// ImageLHmiss[wa2x][wa2y][0]=0;
  // ImageLHmiss[wa3x][wa3y][0]=0;
	// ImageLHmiss[wa4x][wa4y][0]=0;
	// ImageLHmiss[wb1x][wb1y][0]=0;
	// ImageLHmiss[wb2x][wb2y][0]=0;
  // ImageLHmiss[wb3x][wb3y][0]=0;
	// ImageLHmiss[wb4x][wb4y][0]=0;
	// ImageLHmiss[wc1x][wc1y][0]=0;
	// ImageLHmiss[wc2x][wc2y][0]=0;
	// ImageLHmiss[wc3x][wc3y][0]=0;
	// ImageLHmiss[wc4x][wc4y][0]=0;

	// // 1
	// // for hole a
	// int relaX, relaY, tx, ty;
	// double s = 1, sinTheta, cosTheta, IPx, IPy;
	// unsigned char ImageOut[SizeX*2][SizeY*2][BytesPerPixel];
	// for (int i = 0; i < SizeX*2; i++) {
	// 	for (int j = 0; j < SizeY*2; j++){
	// 		ImageOut[i][j][0] = ImageLHmiss[i][j][0];
	// 	}
	// }
	// for (int i = wa1x; i <= wa2x; i++) {
	// 	for (int j = wa1y; j <=wa4y; j++) {
	// 		relaX = wa2x - i;
	// 		relaY = j - wa2y;
	// 		double L = sqrt(pow(a4y-a3y,2)+pow(a4x-a3x,2));
	// 		s = L/(wa2x-wa1x);
	// 		sinTheta = abs(a3x-a4x)/L;
	// 		cosTheta = abs(a3y-a4y)/L;
	// 		tx = a3x;
	// 		ty = a3y;
	// 		IPy = s*cosTheta*relaY - s*sinTheta*relaX + ty;
	// 		IPx = - s*sinTheta*relaY - s*cosTheta*relaX + tx;
	// 		if (ceil(IPx)-IPx == 0) {
	// 			IPx += 0.001;
	// 		}
	// 		if (IPy-floor(IPy) == 0) {
	// 			IPy += 0.001;
	// 		}
	// 		int temp = (int)(ImagePatch1[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
	// 											+ ImagePatch1[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
	// 											+ ImagePatch1[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
	// 											+ ImagePatch1[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
	// 		if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
	// 	}
	// }
	//
	// // for hole b
	// for (int i = wb1x; i <= wb2x; i++) {
	// 	for (int j = wb1y; j <=wb4y; j++) {
	// 		relaX = wb2x - i;
	// 		relaY = j - wb2y;
	// 		double L = sqrt(pow(b4y-b3y,2)+pow(b4x-b3x,2));
	// 		s = L/(wb2x-wb1x);
	// 		sinTheta = abs(b3x-b4x)/L;
	// 		cosTheta = abs(b3y-b4y)/L;
	// 		tx = b4x;
	// 		ty = b4y;
	// 		IPy = -s*sinTheta*relaY - s*cosTheta*relaX + ty;
	// 		IPx = - s*cosTheta*relaY + s*sinTheta*relaX + tx;
	// 		if (ceil(IPx)-IPx == 0) {
	// 			IPx += 0.001;
	// 		}
	// 		if (IPy-floor(IPy) == 0) {
	// 			IPy += 0.001;
	// 		}
	// 		int temp = (int)(ImagePatch2[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
	// 											+ ImagePatch2[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
	// 											+ ImagePatch2[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
	// 											+ ImagePatch2[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
	// 		if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
	// 	}
	// }
	//
	// // for hole c
	// for (int i = wc1x; i <= wc2x; i++) {
	// 	for (int j = wc1y; j <=wc4y; j++) {
	// 		relaX = wc2x - i;
	// 		relaY = j - wc2y;
	// 		double L = sqrt(pow(c4y-c3y,2)+pow(c4x-c3x,2));
	// 		s = L/(wc2x-wc1x);
	// 		sinTheta = abs(c3x-c4x)/L;
	// 		cosTheta = abs(c3y-c4y)/L;
	// 		tx = c2x;
	// 		ty = c2y;
	// 		IPy = s*sinTheta*relaY + s*cosTheta*relaX + ty;
	// 		IPx = s*cosTheta*relaY - s*sinTheta*relaX + tx;
	// 		if (ceil(IPx)-IPx == 0) {
	// 			IPx += 0.001;
	// 		}
	// 		if (IPy-floor(IPy) == 0) {
	// 			IPy += 0.001;
	// 		}
	// 		int temp = (int)(ImagePatch3[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
	// 											+ ImagePatch3[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
	// 											+ ImagePatch3[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
	// 											+ ImagePatch3[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
	// 		if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
	// 	}
	// }

	// 2
	// for hole a
	int relaX, relaY, tx, ty;
	double s = 1, sinTheta, cosTheta, IPx, IPy;
	unsigned char ImageOut[SizeX*2][SizeY*2][BytesPerPixel];
	for (int i = 0; i < SizeX*2; i++) {
		for (int j = 0; j < SizeY*2; j++){
			ImageOut[i][j][0] = ImageLHmiss[i][j][0];
		}
	}

	wa1x-=1; wa2x+=1; wa3x+=1; wa4x-=1;
	wb1x-=1; wb2x+=1; wb3x+=1; wb4x-=1;
			wc1x-=1; wc2x+=1; wc3x+=1; wc4x-=1;
			wa1y-=1; wa2y-=1; wa3y+=1; wa4y+=1;
			wb1y; wb2y; wb3y+=1; wb4y+=1;
			wc1y-=1; wc2y-=1; wc3y+=1; wc4y+=1;

	for (int i = wa1x+1; i <= wa2x-1; i++) {
		for (int j = wa1y+1; j <=wa4y-1; j++) {
			relaX = wa2x - i;
			relaY = j - wa2y;
			double L = sqrt(pow(a4y-a3y,2)+pow(a4x-a3x,2));
			s = L/(wa2x-wa1x);
			sinTheta = abs(a3x-a4x)/L;
			cosTheta = abs(a3y-a4y)/L;
			tx = a3x;
			ty = a3y;
			IPy = s*cosTheta*relaY - s*sinTheta*relaX + ty;
			IPx = - s*sinTheta*relaY - s*cosTheta*relaX + tx;
			if (ceil(IPx)-IPx == 0) {
				IPx += 0.001;
			}
			if (IPy-floor(IPy) == 0) {
				IPy += 0.001;
			}
			int temp = (int)(ImagePatch1[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
												+ ImagePatch1[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
												+ ImagePatch1[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
												+ ImagePatch1[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
			if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
		}
	}

	// for hole b
	for (int i = wb1x+1; i <= wb2x-1; i++) {
		for (int j = wb1y+1; j <=wb4y-1; j++) {
			relaX = wb2x - i;
			relaY = j - wb2y;
			double L = sqrt(pow(b4y-b3y,2)+pow(b4x-b3x,2));
			s = L/(wb2x-wb1x);
			sinTheta = abs(b3x-b4x)/L;
			cosTheta = abs(b3y-b4y)/L;
			tx = b4x;
			ty = b4y;
			IPy = -s*sinTheta*relaY - s*cosTheta*relaX + ty;
			IPx = - s*cosTheta*relaY + s*sinTheta*relaX + tx;
			if (ceil(IPx)-IPx == 0) {
				IPx += 0.001;
			}
			if (IPy-floor(IPy) == 0) {
				IPy += 0.001;
			}
			int temp = (int)(ImagePatch2[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
												+ ImagePatch2[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
												+ ImagePatch2[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
												+ ImagePatch2[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
			if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
		}
	}

	// for hole c
	for (int i = wc1x+1; i <= wc2x-1; i++) {
		for (int j = wc1y+1; j <=wc4y-1; j++) {
			relaX = wc2x - i;
			relaY = j - wc2y;
			double L = sqrt(pow(c4y-c3y,2)+pow(c4x-c3x,2));
			s = L/(wc2x-wc1x);
			sinTheta = abs(c3x-c4x)/L;
			cosTheta = abs(c3y-c4y)/L;
			tx = c2x;
			ty = c2y;
			IPy = s*sinTheta*relaY + s*cosTheta*relaX + ty;
			IPx = s*cosTheta*relaY - s*sinTheta*relaX + tx;
			if (ceil(IPx)-IPx == 0) {
				IPx += 0.001;
			}
			if (IPy-floor(IPy) == 0) {
				IPy += 0.001;
			}
			int temp = (int)(ImagePatch3[(int)floor(IPx)][(int)floor(IPy)][0]*(ceil(IPx)-IPx)*(ceil(IPy)-IPy)
												+ ImagePatch3[(int)floor(IPx)][(int)ceil(IPy)][0]*(ceil(IPx)-IPx)*(IPy-floor(IPy))
												+ ImagePatch3[(int)ceil(IPx)][(int)floor(IPy)][0]*(IPx-floor(IPx))*(ceil(IPy)-IPy)
												+ ImagePatch3[(int)ceil(IPx)][(int)ceil(IPy)][0]*(IPx-floor(IPx))*(IPy-floor(IPy)));
			if (temp>=0 && temp<=255) {ImageOut[i][j][0] = temp;}
		}
	}

 // remove white line
 // for a
 for (int i=wa1x+1; i<wa2x; i++) {
	 if (abs(ImageOut[i][wa1y+1][0]-(ImageOut[i][wa1y][0]+ImageOut[i][wa1y+2][0])/2)>10) {
		 ImageOut[i][wa1y+1][0] = (ImageOut[i][wa1y][0]+ImageOut[i][wa1y+2][0])/2;
	 }
 }
 for (int j=wa2y+1; j<wa3y; j++) {
	 if (abs(ImageOut[wa2x-1][j][0]-(ImageOut[wa2x][j][0]+ImageOut[wa2x-2][j][0])/2)>10) {
		 ImageOut[wa2x-1][j][0] = (ImageOut[wa2x][j][0]+ImageOut[wa2x-2][j][0])/2;
	 }
 }
 // for b
 for (int i=wb1x+1; i<wb2x; i++) {
 	if (abs(ImageOut[i][wb1y][0]-(ImageOut[i][wb1y-1][0]+ImageOut[i][wb1y+1][0])/2)>10) {
 		ImageOut[i][wb1y][0] = (ImageOut[i][wb1y-1][0]+ImageOut[i][wb1y+1][0])/2;
 	}
 }
 for (int j=wb1y; j<wb4y; j++) {
 	if (abs(ImageOut[wb4x+1][j][0]-(ImageOut[wb4x+2][j][0]+ImageOut[wb4x][j][0])/2)>3) {
 		ImageOut[wb4x+1][j][0] = (ImageOut[wb4x+2][j][0]+ImageOut[wb4x][j][0])/2;
 	}
 }

	// Matrix2d a;
 // a << 1, 2,
	// 		3, 4;
 // MatrixXd b(2,2);
 // b << 2, 3,
	// 		1, 4;
 // std::cout << "a + b =\n" << a + b << std::endl;
 // std::cout << "a - b =\n" << a - b << std::endl;
 // std::cout << "Doing a += b;" << std::endl;
 // a += b;
 // std::cout << "Now a =\n" << a << std::endl;
 // Vector3d v(1,2,3);
 // Vector3d w(1,0,0);
 // std::cout << "-v + w - v =\n" << -v + w - v << std::endl;


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file = fopen(argv[5], "wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(ImageOut, sizeof(unsigned char), SizeX*SizeY*BytesPerPixel*4, file);
	fclose(file);

    return 0;
}
