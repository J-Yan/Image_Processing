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
	unsigned char DataTemp[SizeX][SizeY][BytesPerPixel];
	for (int i = 0; i < SizeX; i++) {
		for (int j = 0; j < SizeY; j++) {
			for (int c = 0; c < 3; c++) {
				DataTemp[i][j][c] = Imagedata[i][j][c];
			}
		}
	}
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
  // mbvq mark, 1-6
	int mbvq = 1;


		for (int i = 0; i < SizeX; i++) {
			f = abs(1-f); // odd line and even line
			//cout<<i<<endl;
			for (int j = 0; j < SizeY; j++) {








				//---------------------------------------------------




				if (f == 0) {
					// decide which part
					if (DataTemp[i][j][0]+DataTemp[i][j][1]>255) {
						if (DataTemp[i][j][2]+DataTemp[i][j][1]>255) {
							if (DataTemp[i][j][0]+DataTemp[i][j][1]+DataTemp[i][j][2]>510) {
								mbvq = 1;
							}
							else {
								mbvq = 2;
							}
						}
						else {
							mbvq = 3;
						}
					}
					else {
						if (DataTemp[i][j][2]+DataTemp[i][j][1]<=255) {
							if (DataTemp[i][j][0]+DataTemp[i][j][1]+DataTemp[i][j][2]<=255) {
								mbvq = 4;
							}
							else {
								mbvq = 5;
							}
						}
						else {
							mbvq = 6;
						}
					}







					// closest point
					// 1: CMYW--------------------------------------------------------
					if (mbvq == 1) {
						// vertex = 'white'
						ImagedataOut[i][j][0] = 255;
						ImagedataOut[i][j][1] = 255;
						ImagedataOut[i][j][2] = 255;
						if (Imagedata[i][j][2] < 127) {
							if (Imagedata[i][j][2] <= Imagedata[i][j][0]) {
								if (Imagedata[i][j][2] <= Imagedata[i][j][1]) {
									// Y
									ImagedataOut[i][j][0] = 255;
									ImagedataOut[i][j][1] = 255;
									ImagedataOut[i][j][2] = 0;
								}
							}
						}
						if (Imagedata[i][j][1] < 127) {
								if (Imagedata[i][j][1] <= Imagedata[i][j][2]) {
										if (Imagedata[i][j][1] <= Imagedata[i][j][0]) {
											// M
											ImagedataOut[i][j][0] = 255;
											ImagedataOut[i][j][1] = 0;
											ImagedataOut[i][j][2] = 255;
										}
								}
						}
						if (Imagedata[i][j][0] < 127) {
								if (Imagedata[i][j][0] <= Imagedata[i][j][2]) {
										if (Imagedata[i][j][0] <= Imagedata[i][j][1]) {
											// C
											ImagedataOut[i][j][0] = 0;
											ImagedataOut[i][j][1] = 255;
											ImagedataOut[i][j][2] = 255;
										}
								}
						}
					}
					// 2: MYGC--------------------------------------------------------
					if (mbvq == 2) {
						//vertex = 'magenta';
						ImagedataOut[i][j][0] = 255;
						ImagedataOut[i][j][1] = 0;
						ImagedataOut[i][j][2] = 255;
		        if (Imagedata[i][j][1] >= Imagedata[i][j][2]) {
		            if (Imagedata[i][j][0] >= Imagedata[i][j][2]) {
		                if (Imagedata[i][j][0] >= 127) {
		                    // vertex = 'yellow';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
										}
		                else{
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
		            }
		        }
		        if (Imagedata[i][j][1] >= Imagedata[i][j][0]) {
		            if (Imagedata[i][j][2] >= Imagedata[i][j][0]) {
		                if (Imagedata[i][j][2] >= 127) {
		                    //vertex = 'cyan';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 255;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
		            }
		        }
					}
	        // 3: RGMY--------------------------------------------------------
					if (mbvq == 3) {
						if (Imagedata[i][j][2] > 127) {
		            if (Imagedata[i][j][0] > 127) {
		                if (Imagedata[i][j][2] >= Imagedata[i][j][1]) {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
											}
										else {
		                    //vertex = 'yellow';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][j][1] > Imagedata[i][j][2] + Imagedata[i][j][0]) {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
											}
										else  {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
							}
		        else {
		            if (Imagedata[i][j][0] >= 127) {
		                if (Imagedata[i][j][1] >= 127) {
		                    //vertex = 'yellow';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
											}
										else {
		                    //vertex = 'red';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][j][0] >= Imagedata[i][j][1]) {
		                    //vertex = 'red';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 0;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
		            }
		        }
					}
					// 4: KRGB--------------------------------------------------------
					if (mbvq == 4) {
						//vertex = 'black';
						ImagedataOut[i][j][0] = 0;
						ImagedataOut[i][j][1] = 0;
						ImagedataOut[i][j][2] = 0;
		        if (Imagedata[i][j][2] > 127) {
		            if (Imagedata[i][j][2] >= Imagedata[i][j][0]) {
		                if (Imagedata[i][j][2] >= Imagedata[i][j][1]) {
		                    //vertex = 'blue';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
		        }
		        if (Imagedata[i][j][1] > 127) {
		            if (Imagedata[i][j][1] >= Imagedata[i][j][2]) {
		                if (Imagedata[i][j][1] >= Imagedata[i][j][0]) {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
		            }
		        }
		        if (Imagedata[i][j][0] > 127) {
		            if (Imagedata[i][j][0] >= Imagedata[i][j][2]) {
		                if (Imagedata[i][j][0] >= Imagedata[i][j][1]) {
		                    //vertex = 'red';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 0;
		                }
		            }
		        }
					}
					// 5: RGBM--------------------------------------------------------
					if (mbvq == 5) {
						//vertex = 'green';
						ImagedataOut[i][j][0] = 0;
						ImagedataOut[i][j][1] = 255;
						ImagedataOut[i][j][2] = 0;
		        if (Imagedata[i][j][0] > Imagedata[i][j][1]) {
		            if (Imagedata[i][j][0] >= Imagedata[i][j][2]) {
		                if (Imagedata[i][j][2] < 127) {
		                    //vertex = 'red';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 0;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
		        }
		        if (Imagedata[i][j][2] > Imagedata[i][j][1]) {
		            if (Imagedata[i][j][2] >= Imagedata[i][j][0]) {
		                if (Imagedata[i][j][0] < 127) {
		                    //vertex = 'blue';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
		        }
					}
					// 6: CMGB--------------------------------------------------------
					if (mbvq == 6) {
						if (Imagedata[i][j][2] > 127) {
		            if ( Imagedata[i][j][0] > 127) {
		                if (Imagedata[i][j][1] >= Imagedata[i][j][0]) {
		                    //vertex = 'cyan';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 255;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
									}
		            else {
		                if (Imagedata[i][j][1] > 127) {
		                    //vertex = 'cyan';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 255;
											}
		                else {
		                    //vertex = 'blue';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
							}
		        else {
		            if ( Imagedata[i][j][0] > 127) {
		                if (Imagedata[i][j][0] - Imagedata[i][j][1] + Imagedata[i][j][2] >= 127) {
		                    //vertex = 'magenta';
												ImagedataOut[i][j][0] = 255;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][j][1] >= Imagedata[i][j][2]) {
		                    //vertex = 'green';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 255;
												ImagedataOut[i][j][2] = 0;
											}
		                else {
		                    //vertex = 'blue';
												ImagedataOut[i][j][0] = 0;
												ImagedataOut[i][j][1] = 0;
												ImagedataOut[i][j][2] = 255;
		                }
		            }
		        }
					}


					for (int c = 0; c < 3; c++) {
						e = Imagedata[i][j][c] - ImagedataOut[i][j][c];
						if (j+1<SizeY) {
							Imagedata[i][j+1][c] += e * ED[1][2] / DivCo;
						}
						if ((i+1<SizeX)&&(j-1>=0)) {
							Imagedata[i+1][j-1][c] += e * ED[2][0] / DivCo;
						}
						if (i+1<SizeX) {
							Imagedata[i+1][j][c] += e * ED[2][1] / DivCo;
						}
						if ((i+1<SizeX)&&(j+1<SizeY)) {
							Imagedata[i+1][j+1][c] += e * ED[2][2] / DivCo;
						}
					}


				}
				else {
					if (DataTemp[i][SizeY-j-1][0]+DataTemp[i][SizeY-j-1][1]>255) {
						if (DataTemp[i][SizeY-j-1][2]+DataTemp[i][SizeY-j-1][1]>255) {
							if (DataTemp[i][SizeY-j-1][0]+DataTemp[i][SizeY-j-1][1]+DataTemp[i][SizeY-j-1][2]>510) {
								mbvq = 1;
							}
							else {
								mbvq = 2;
							}
						}
						else {
							mbvq = 3;
						}
					}
					else {
						if (DataTemp[i][SizeY-j-1][2]+DataTemp[i][SizeY-j-1][1]<=255) {
							if (DataTemp[i][SizeY-j-1][0]+DataTemp[i][SizeY-j-1][1]+DataTemp[i][SizeY-j-1][2]<=255) {
								mbvq = 4;
							}
							else {
								mbvq = 5;
							}
						}
						else {
							mbvq = 6;
						}
					}







					// closest point
					// 1: CMYW--------------------------------------------------------
					if (mbvq == 1) {
						// vertex = 'white'
						ImagedataOut[i][SizeY-j-1][0] = 255;
						ImagedataOut[i][SizeY-j-1][1] = 255;
						ImagedataOut[i][SizeY-j-1][2] = 255;
						if (Imagedata[i][SizeY-j-1][2] < 127) {
							if (Imagedata[i][SizeY-j-1][2] <= Imagedata[i][SizeY-j-1][0]) {
								if (Imagedata[i][SizeY-j-1][2] <= Imagedata[i][SizeY-j-1][1]) {
									// Y
									ImagedataOut[i][SizeY-j-1][0] = 255;
									ImagedataOut[i][SizeY-j-1][1] = 255;
									ImagedataOut[i][SizeY-j-1][2] = 0;
								}
							}
						}
						if (Imagedata[i][SizeY-j-1][1] < 127) {
								if (Imagedata[i][SizeY-j-1][1] <= Imagedata[i][SizeY-j-1][2]) {
										if (Imagedata[i][SizeY-j-1][1] <= Imagedata[i][SizeY-j-1][0]) {
											// M
											ImagedataOut[i][SizeY-j-1][0] = 255;
											ImagedataOut[i][SizeY-j-1][1] = 0;
											ImagedataOut[i][SizeY-j-1][2] = 255;
										}
								}
						}
						if (Imagedata[i][SizeY-j-1][0] < 127) {
								if (Imagedata[i][SizeY-j-1][0] <= Imagedata[i][SizeY-j-1][2]) {
										if (Imagedata[i][SizeY-j-1][0] <= Imagedata[i][SizeY-j-1][1]) {
											// C
											ImagedataOut[i][SizeY-j-1][0] = 0;
											ImagedataOut[i][SizeY-j-1][1] = 255;
											ImagedataOut[i][SizeY-j-1][2] = 255;
										}
								}
						}
					}
					// 2: MYGC--------------------------------------------------------
					if (mbvq == 2) {
						//vertex = 'magenta';
						ImagedataOut[i][SizeY-j-1][0] = 255;
						ImagedataOut[i][SizeY-j-1][1] = 0;
						ImagedataOut[i][SizeY-j-1][2] = 255;
		        if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][2]) {
		            if (Imagedata[i][SizeY-j-1][0] >= Imagedata[i][SizeY-j-1][2]) {
		                if (Imagedata[i][SizeY-j-1][0] >= 127) {
		                    // vertex = 'yellow';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
										}
		                else{
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
		            }
		        }
		        if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][0]) {
		            if (Imagedata[i][SizeY-j-1][2] >= Imagedata[i][SizeY-j-1][0]) {
		                if (Imagedata[i][SizeY-j-1][2] >= 127) {
		                    //vertex = 'cyan';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
		            }
		        }
					}
	        // 3: RGMY--------------------------------------------------------
					if (mbvq == 3) {
						if (Imagedata[i][SizeY-j-1][2] > 127) {
		            if (Imagedata[i][SizeY-j-1][0] > 127) {
		                if (Imagedata[i][SizeY-j-1][2] >= Imagedata[i][SizeY-j-1][1]) {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
										else {
		                    //vertex = 'yellow';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][SizeY-j-1][1] > Imagedata[i][SizeY-j-1][2] + Imagedata[i][SizeY-j-1][0]) {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
											}
										else  {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
							}
		        else {
		            if (Imagedata[i][SizeY-j-1][0] >= 127) {
		                if (Imagedata[i][SizeY-j-1][1] >= 127) {
		                    //vertex = 'yellow';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
											}
										else {
		                    //vertex = 'red';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][SizeY-j-1][0] >= Imagedata[i][SizeY-j-1][1]) {
		                    //vertex = 'red';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 0;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
		            }
		        }
					}
					// 4: KRGB--------------------------------------------------------
					if (mbvq == 4) {
						//vertex = 'black';
						ImagedataOut[i][SizeY-j-1][0] = 0;
						ImagedataOut[i][SizeY-j-1][1] = 0;
						ImagedataOut[i][SizeY-j-1][2] = 0;
		        if (Imagedata[i][SizeY-j-1][2] > 127) {
		            if (Imagedata[i][SizeY-j-1][2] >= Imagedata[i][SizeY-j-1][0]) {
		                if (Imagedata[i][SizeY-j-1][2] >= Imagedata[i][SizeY-j-1][1]) {
		                    //vertex = 'blue';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
		        }
		        if (Imagedata[i][SizeY-j-1][1] > 127) {
		            if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][2]) {
		                if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][0]) {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
		            }
		        }
		        if (Imagedata[i][SizeY-j-1][0] > 127) {
		            if (Imagedata[i][SizeY-j-1][0] >= Imagedata[i][SizeY-j-1][2]) {
		                if (Imagedata[i][SizeY-j-1][0] >= Imagedata[i][SizeY-j-1][1]) {
		                    //vertex = 'red';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
		            }
		        }
					}
					// 5: RGBM--------------------------------------------------------
					if (mbvq == 5) {
						//vertex = 'green';
						ImagedataOut[i][SizeY-j-1][0] = 0;
						ImagedataOut[i][SizeY-j-1][1] = 255;
						ImagedataOut[i][SizeY-j-1][2] = 0;
		        if (Imagedata[i][SizeY-j-1][0] > Imagedata[i][SizeY-j-1][1]) {
		            if (Imagedata[i][SizeY-j-1][0] >= Imagedata[i][SizeY-j-1][2]) {
		                if (Imagedata[i][SizeY-j-1][2] < 127) {
		                    //vertex = 'red';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 0;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
		        }
		        if (Imagedata[i][SizeY-j-1][2] > Imagedata[i][SizeY-j-1][1]) {
		            if (Imagedata[i][SizeY-j-1][2] >= Imagedata[i][SizeY-j-1][0]) {
		                if (Imagedata[i][SizeY-j-1][0] < 127) {
		                    //vertex = 'blue';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
		        }
					}
					// 6: CMGB--------------------------------------------------------
					if (mbvq == 6) {
						if (Imagedata[i][SizeY-j-1][2] > 127) {
		            if ( Imagedata[i][SizeY-j-1][0] > 127) {
		                if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][0]) {
		                    //vertex = 'cyan';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
		                else {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
									}
		            else {
		                if (Imagedata[i][SizeY-j-1][1] > 127) {
		                    //vertex = 'cyan';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
		                else {
		                    //vertex = 'blue';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
							}
		        else {
		            if ( Imagedata[i][SizeY-j-1][0] > 127) {
		                if (Imagedata[i][SizeY-j-1][0] - Imagedata[i][SizeY-j-1][1] + Imagedata[i][SizeY-j-1][2] >= 127) {
		                    //vertex = 'magenta';
												ImagedataOut[i][SizeY-j-1][0] = 255;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
											}
		                else {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
		                }
									}
		            else {
		                if (Imagedata[i][SizeY-j-1][1] >= Imagedata[i][SizeY-j-1][2]) {
		                    //vertex = 'green';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 255;
												ImagedataOut[i][SizeY-j-1][2] = 0;
											}
		                else {
		                    //vertex = 'blue';
												ImagedataOut[i][SizeY-j-1][0] = 0;
												ImagedataOut[i][SizeY-j-1][1] = 0;
												ImagedataOut[i][SizeY-j-1][2] = 255;
		                }
		            }
		        }
					}
					//cout<<SizeY-j-1<<" ";
					for (int c = 0; c < 3; c++) {
						e = Imagedata[i][SizeY-j-1][c] - ImagedataOut[i][SizeY-j-1][c];
						if (SizeY-j-2>=0){
							Imagedata[i][SizeY-j-2][c] += e * ED[1][2] / DivCo;
						}
						if ((i+1<SizeX)&&(SizeY-j<SizeY)){
							Imagedata[i+1][SizeY-j][c] += e * ED[2][0] / DivCo;
						}
						if (i+1<SizeX){
							Imagedata[i+1][SizeY-j-1][c] += e * ED[2][1] / DivCo;
						}
						if ((i+1<SizeX)&&(SizeY-j-2>=0)){
							Imagedata[i+1][SizeY-j-2][c] += e * ED[2][2] / DivCo;
						}
					}
				}



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
