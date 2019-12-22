% Name: YAN JIAO
% USC ID Number: 6419057887
% USC Email: yanjiao@usc.edu
% Submission Date: 2/12/2019

% read raw rgb image
row=321; col=481;
fin=fopen('Tiger.raw','r');
I = fread(fin, col*row*3,'ubit24=>uint32');
I = reshape(I, col, row, []); 
R = uint8(bitand(bitshift(I,-00),uint32(255)));
G = uint8(bitand(bitshift(I,-08),uint32(255)));
B = uint8(bitand(bitshift(I,-16),uint32(255)));
I = cat(3,R,G,B);
Ifinal = flip(imrotate(I, -90),2);
imagesc(Ifinal);
fclose(fin);
imshow(Ifinal);