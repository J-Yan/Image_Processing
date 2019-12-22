// Name: Yan Jiao
// USC ID: 6419057887
// USC Email: yanjiao@usc.edu

//#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
// #include "eigen-eigen-323c052e1731\eigen-eigen-323c052e1731\Eigen\Dense"

using namespace std;
// using namespace Eigen;


#define N 6

int forwardElim(double mat[N][N+1]);
vector<double> backSub(double mat[N][N+1]);
vector<double> gaussianElimination(double mat[N][N+1])
{

    int singular_flag = forwardElim(mat);
vector<double> v(N);
    if (singular_flag != -1)
    {
        printf("Singular Matrix.\n");

        if (mat[singular_flag][N])
            printf("Inconsistent System.");
        else
            {printf("May have infinitely many "
                   "solutions.");}
				return v;
    }


		v=backSub(mat);
		cout<<"###";
		return v;
}
void swap_row(double mat[N][N+1], int i, int j)
{
    for (int k=0; k<=N; k++)
    {
        double temp = mat[i][k];
        mat[i][k] = mat[j][k];
        mat[j][k] = temp;
    }
}

void print(double mat[N][N+1])
{
    for (int i=0; i<N; i++, printf("\n"))
        for (int j=0; j<=N; j++)
            printf("%lf ", mat[i][j]);

    printf("\n");
}

int forwardElim(double mat[N][N+1])
{
    for (int k=0; k<N; k++)
    {
        int i_max = k;
        int v_max = mat[i_max][k];
        for (int i = k+1; i < N; i++)
            if (abs(mat[i][k]) > v_max)
                v_max = mat[i][k], i_max = i;
        if (!mat[k][i_max])
            return k;
        if (i_max != k)
            swap_row(mat, k, i_max);
        for (int i=k+1; i<N; i++)
        {
            double f = mat[i][k]/mat[k][k];
            for (int j=k+1; j<=N; j++)
                mat[i][j] -= mat[k][j]*f;
            mat[i][k] = 0;
        }

    }

    return -1;
}

vector<double> backSub(double mat[N][N+1])
{
    double x[N];
    for (int i = N-1; i >= 0; i--)
    {
        x[i] = mat[i][N];
        for (int j=i+1; j<N; j++)
        {
            x[i] -= mat[i][j]*x[j];
        }
        x[i] = x[i]/mat[i][i];
    }
    printf("\nSolution for the system:\n");
		vector<double> v(N);
    for (int i=0; i<N; i++) {
			printf("%lf\n", x[i]);

		}
		for (int i=0; i<N; i++) {
			v[i]=x[i];

		}
		return v;
}


int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int Size = 512;

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
  unsigned char ImageHat[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file = fopen(argv[1], "rb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fread(ImageHat, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	unsigned char ImageHatOut[Size][Size][BytesPerPixel];
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++){
			ImageHatOut[i][j][0] = 100;
		}
	}
	// radius of the circle where the arc lies
	double R = 377.68;
	vector<vector<double>> uv;
	double x0 = 0, x1 = (Size-1)/4 ,x2 = (Size-1)/2, x3 = (Size-1)/4*3, x4 = 511, x5 = (Size-1)/2, y0 = 0, y1 = (Size-1)/4, y2 = (Size-1)/2, y3 = (Size-1)/4, y4 = 0, y5 = 100;
	double u0 = 0, u1 = (Size-1)/4 ,u2 = (Size-1)/2, u3 = (Size-1)/4*3, u4 = 511, u5 = (Size-1)/2, v0 = 0, v1 = (Size-1)/4, v2 = (Size-1)/2, v3 = (Size-1)/4, v4 = 0, v5 = 0;
	double mat1[N][N+1] = {{1.0, x0, y0, x0*x0, x0*y0, y0*y0,u0},
											{1.0, x1, y1, x1*x1, x1*y1, y1*y1,u1},
											{1.0, x2, y2, x2*x2, x2*y2, y2*y2,u2},
											{1.0, x3, y3, x3*x3, x3*y3, y3*y3,u3},
											{1.0, x4, y4, x4*x4, x4*y4, y4*y4,u4},
											{1.0, x5, y5, x5*x5, x5*y5, y5*y5,u5}
											};
	double mat2[N][N+1] = {{1.0, x0, y0, x0*x0, x0*y0, y0*y0,v0},
											{1.0, x1, y1, x1*x1, x1*y1, y1*y1,v1},
											{1.0, x2, y2, x2*x2, x2*y2, y2*y2,v2},
											{1.0, x3, y3, x3*x3, x3*y3, y3*y3,v3},
											{1.0, x4, y4, x4*x4, x4*y4, y4*y4,v4},
											{1.0, x5, y5, x5*x5, x5*y5, y5*y5,v5}
										};
	vector<double> a;
	vector<double> b;
	a=gaussianElimination(mat1);
 	b=gaussianElimination(mat2);

	for (int i = 0; i < Size/2+1; i++) {
		//cout <<i<<"   ";
		for (int j = 0; j <= i; j++) {
			// decide the pixel is out of the shape or not
			if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
				double X, Y,xx,yy;
				X = a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j;
				Y = b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j;
				//cout<<i<<" "<<j<<" "<<X<<" "<<Y<<" "<<endl;
				int temp;

					temp = (int)(ImageHat[(int)X][(int)floor(Y)][0]*(ceil(Y)-Y)
														+ ImageHat[(int)X][(int)ceil(Y)][0]*(Y-floor(Y)));

				ImageHatOut[i][j][0] = temp;

			}
			else {
				ImageHatOut[i][j][0] = 50;
			}
		}
	}

	for (int i = Size/2; i < Size; i++) {
		for (int j = 0; j <= Size-i; j++) {

			if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
				double X, Y;
				X = a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j;
				Y = b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j;
				int temp;
				temp = (int)(ImageHat[(int)X][(int)floor(Y)][0]*(ceil(Y)-Y)
														+ ImageHat[(int)X][(int)ceil(Y)][0]*(Y-floor(Y)));
				ImageHatOut[i][j][0] = temp;
			}
			else {
				ImageHatOut[i][j][0] = 50;
			}
		}
	}

	// for (int i = 0; i < Size/2; i++) {
	// 	//cout <<i<<"   ";
	// 	for (int j = 0; j <= i; j++) {
	// 		// decide the pixel is out of the shape or not
	// 		if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
	// 			double X, Y;
	// 			X = a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j;
	// 			Y = b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j;
	// 			//cout<<i<<" "<<j<<" "<<X<<" "<<Y<<" "<<endl;
	// 			int temp;
  //
	// 				temp = (int)(ImageHat[(int)floor(Y)][(int)X][0]*(ceil(Y)-Y)
	// 													+ ImageHat[(int)ceil(Y)][(int)X][0]*(Y-floor(Y)));
	// 			ImageHatOut[j][i][0] = temp;
	// 		}
	// 		else {
	// 			ImageHatOut[j][i][0] = 50;
	// 		}
	// 	}
	// }
  //
	// for (int i = Size/2; i < Size; i++) {
	// 	for (int j = 0; j <= Size-i; j++) {
  //
	// 		if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
	// 			double X, Y;
	// 			X = a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j;
	// 			Y = b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j;
	// 			int temp;
	// 			temp = (int)(ImageHat[(int)floor(Y)][(int)X][0]*(ceil(Y)-Y)
	// 													+ ImageHat[(int)ceil(Y)][(int)X][0]*(Y-floor(Y)));
	// 			ImageHatOut[j][i][0] = temp;
	// 		}
	// 		else {
	// 			ImageHatOut[j][i][0] = 50;
	// 		}
	// 	}
	// }
  //
	// for (int i = 0; i < Size/2+1; i++) {
	// 	//cout <<i<<"   ";
	// 	for (int j = 0; j <= i; j++) {
	// 		// decide the pixel is out of the shape or not
	// 		if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
	// 			double X, Y,xx,yy;
	// 			X =511- (a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j);
	// 			Y =511- (b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j);
	// 			//cout<<i<<" "<<j<<" "<<X<<" "<<Y<<" "<<endl;
	// 			int temp;
	// 				temp = (int)(ImageHat[(int)X][(int)floor(Y)][0]*(ceil(Y)-Y)
	// 													+ ImageHat[(int)X][(int)ceil(Y)][0]*(Y-floor(Y)));
  //
	// 			ImageHatOut[511-i][511-j][0] = temp;
  //
	// 		}
	// 		else {
	// 			ImageHatOut[511-i][511-j][0] = 50;
	// 		}
	// 	}
	// }
  //
	// for (int i = Size/2; i < Size; i++) {
	// 	for (int j = 0; j <= Size-i; j++) {
  //
	// 		if (pow(j+(R-100),2)+pow(i-Size/2,2) > R*R) {
	// 			double X, Y;
	// 			X = 511-(a[0]*1.0 + a[1]*i+ a[2]*j+ a[3]*i*i+ a[4]*i*j+ a[5]*j*j);
	// 			Y = 511-(b[0]*1.0 + b[1]*i+ b[2]*j+ b[3]*i*i+ b[4]*i*j+ b[5]*j*j);
	// 			int temp;
	// 			temp = (int)(ImageHat[(int)X][(int)floor(Y)][0]*(ceil(Y)-Y)
	// 													+ ImageHat[(int)X][(int)ceil(Y)][0]*(Y-floor(Y)));
	// 			ImageHatOut[511-i][511-j][0] = temp;
	// 		}
	// 		else {
	// 			ImageHatOut[511-i][511-j][0] = 50;
	// 		}
	// 	}
	// }

	if (!(file = fopen(argv[2], "wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ImageHatOut, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

    return 0;
}
