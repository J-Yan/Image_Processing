% Name: YAN JIAO
% USC ID Number: 6419057887
% USC Email: yanjiao@usc.edu
% Submission Date: 2/12/2019

% problem 1, a

% threshold
T = 0.25;
% read img
% Tig = imread('Pig.jpg');
row=321; col=481;
fin=fopen('Pig.raw','r');
I = fread(fin, col*row*3,'ubit24=>uint32');
I = reshape(I, col, row, []); 
R = uint8(bitand(bitshift(I,-00),uint32(255)));
G = uint8(bitand(bitshift(I,-08),uint32(255)));
B = uint8(bitand(bitshift(I,-16),uint32(255)));
I = cat(3,R,G,B);
Pig = flip(imrotate(I, -90),2);
imagesc(Pig);
fclose(fin);
% Tig_gray = rgb2gray(Tig);
% use own code to convert RGB to gray
Pig_gray = zeros(321,481);
for i = 1:321
    for j = 1:481
        Pig_gray(i,j) = (Pig(i,j,1)*0.2126 + Pig(i,j,2)*0.7152 + Pig(i,j,3)*0.0722);
    end
end
% sobel mask dir-x
Gx = [-1 0 1; -2 0 2; -1 0 1];
Xgrad = Pig_gray;
for i = 2:320
    for j = 2:480
        patch = double([Pig_gray(i-1,j-1) Pig_gray(i-1,j) Pig_gray(i-1,j+1);
            Pig_gray(i,j-1) Pig_gray(i,j) Pig_gray(i,j+1);
            Pig_gray(i+1,j-1) Pig_gray(i+1,j) Pig_gray(i+1,j+1)]);
        Xgrad(i,j) = sum(sum(patch .* Gx))/4;
    end
end
Xgrad = Xgrad(2:320,2:480);
maxX = max(max(abs(Xgrad)));
minX = min(min(abs(Xgrad)));
X = floor(abs(Xgrad-minX)  * 255/ (maxX-minX));
Xgrad = uint8(floor(abs(Xgrad-minX)  * 255/ (maxX-minX)));
% sobel mask dir-y
Gy = [1 2 1; 0 0 0; -1 -2 -1];
Ygrad = Pig_gray;
for i = 2:320
    for j = 2:480
        patch = double([Pig_gray(i-1,j-1) Pig_gray(i-1,j) Pig_gray(i-1,j+1);
            Pig_gray(i,j-1) Pig_gray(i,j) Pig_gray(i,j+1);
            Pig_gray(i+1,j-1) Pig_gray(i+1,j) Pig_gray(i+1,j+1)]);
        Ygrad(i,j) = sum(sum(patch .* Gy))/4;
    end
end
Ygrad = Ygrad(2:320,2:480);
maxY = max(max(abs(Ygrad)));
minY = min(min(abs(Ygrad)));
Y = floor(abs(Ygrad-minY)  * 255/ (maxY -minY));
Ygrad = uint8(floor(abs(Ygrad-minY)  * 255/ (maxY -minY)));
% Y = floor((Ygrad-minY) * 255/ (maxY-minY));
% Ygrad = uint8(floor((Ygrad-minY) * 255/ (maxY-minY)));
% gray value
Gray = floor(sqrt(X.*X + Y.*Y));
maxG = max(max(Gray));
E = 255*ones(319,479);
    for i = 1:319
        for j = 1:479
            if Gray(i,j) > maxG * T
            E(i,j) = 0;
            end
        end
    end
imshow(E);


