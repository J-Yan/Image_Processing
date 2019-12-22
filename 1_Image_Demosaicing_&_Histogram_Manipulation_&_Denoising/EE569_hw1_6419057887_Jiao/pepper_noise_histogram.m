fid=fopen('pepper - Copy.raw','rb');
[a,count]=fread(fid,[256,256]);
fclose(fid);

fid=fopen('pepper_uni - Copy.raw','rb');
[b,count]=fread(fid,[256,256]);
fclose(fid);

c=abs(a-b);

histUni=zeros(1,256);  % prealocate
s=size(c);
len=s(1)*s(2);
for n=1:len
    histUni(1,c(n)+1)=histUni(1,c(n)+1)+1; 
end
minc=255;
maxc=0;
for m=1:s(1)
    for k=1:s(2)
        if c(m,k) < minc
            minc=c(m,k);
        end
        if c(m,k) > maxc
            maxc=c(m,k);
        end
    end
end

figure,bar(histUni);
title('histogram of error');
xlabel('error');
ylabel('error Frequency');
xlim([minc maxc]);


