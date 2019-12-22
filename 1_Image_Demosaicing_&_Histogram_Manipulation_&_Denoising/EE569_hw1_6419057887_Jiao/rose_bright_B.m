fid=fopen('rose_bright.raw','rb');
[a,count]=fread(fid,[400,400]);
fclose(fid);

figure,imshow(a',[0,255]);
title('rose\_bright.raw');

histArray=zeros(1,256);  % prealocate
s=size(a);
len=s(1)*s(2);
for n=1:len
    histArray(1,a(n)+1)=histArray(1,a(n)+1)+1; 
end

figure,bar(histArray);
title('histogram of rose\_bright.raw');
xlabel('Pixel Intensity');
ylabel('Pixel Frequency');

cdfInput=zeros(1,256);
histP=histArray/len;
for  i=1:256
    cdfInput(i)=sum(histP(1:i));
end

figure,bar(cdfInput*len);
title('cumulative histogram of rose\_bright.raw');
xlabel('Pixel Intensity');
ylabel('cumulative Pixel Frequency');

countB=zeros(1,256);
bucNum=256;
bucVol=len/bucNum; %625
b=a;

for i=1:400
    for j=1:400
        countB(a(i,j)+1)=countB(a(i,j)+1)+1;
        if a(i,j)+1~=1
            b(i,j)=floor((cdfInput(1,a(i,j))*len+countB(a(i,j)+1)-1)/bucVol);
        else
            b(i,j)=floor((countB(a(i,j)+1)-1)/bucVol);
        end
    end
end

figure,imshow(b',[0,255]);
title('rose\_bright.raw Method B enhanced');

histArray2=zeros(1,256);  % prealocate
ss=size(b);
lenB=ss(1)*ss(2);
for n=1:lenB
    histArray2(1,b(n)+1)=histArray2(1,b(n)+1)+1; 
end

figure,bar(histArray2);
title('histogram of rose\_bright.raw enhanced Method B');
xlabel('Pixel Intensity');
ylabel('Pixel Frequency');

cdfOutput=zeros(1,256);
histP2=histArray2/len;
for  i=1:256
    cdfOutput(i)=sum(histP2(1:i));
end

figure,bar(cdfOutput*len);
title('cumulative histogram of rose\_bright.raw method B enhanced');
xlabel('Pixel Intensity');
ylabel('cumulative Pixel Frequency');