fid=fopen('rose_dark.raw','rb');
[a,count]=fread(fid,[400,400]);
fclose(fid);

figure,imshow(a',[0,255]);
title('rose\_dark.raw');

histArray=zeros(1,256);  % prealocate
x=1:1:256;
s=size(a);
len=s(1)*s(2);
for n=1:len
    histArray(1,a(n)+1)=histArray(1,a(n)+1)+1; % every time you meet the particular value, you add 1 into to corresponding bin
end

figure,bar(histArray);
title('histogram of rose\_dark.raw');
xlabel('Pixel Intensity');
ylabel('Pixel Frequency');


cdfInput=zeros(1,256);
histP=histArray/len;
for  i=1:256
    cdfInput(i)=sum(histP(1:i));
end
% figure,plot([0:1:255],cdfInput);
% title('transfer function of rose_dark.raw');
% xlabel('Pixel Intensity');
% ylabel('cdf');

cdfOutput=zeros(1,256);
for x=1:256
    temp=floor(cdfInput(1,x)*255)+1;
    cdfOutput(1,temp)=cdfInput(1,x);
end
figure,plot(cdfOutput);
title('cdf of rose\_dark.raw enhanced');
xlabel('Pixel Intensity');
ylabel('cdf');


b=255*cdfInput(a+1);
tf=255*cdfInput;
figure,plot(1:1:256,floor(tf));
title('transfer function');
xlabel('Input Pixel Intensity');
ylabel('Output Pixel Intensity');
axis([0 255 0 255]);
figure,imshow(b',[0,255]);
title('rose\_dark.raw enhanced');

histArray2=zeros(1,256);  % prealocate
x=1:1:256;
ss=size(b);
lenb=ss(1)*ss(2);
for n=1:lenb
    histArray2(1,floor(b(n))+1)=histArray2(1,floor(b(n))+1)+1; 
end

figure,bar(histArray2);
title('histogram of rose\_dark.raw enhanced');
xlabel('Pixel Intensity');
ylabel('Pixel Frequency');