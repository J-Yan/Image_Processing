fid=fopen('pepper - Copy.raw','rb');
[a,count]=fread(fid,[256,256]);
fclose(fid);

fid=fopen('p3uni - Copy.raw','rb');
[b1,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p5uni - Copy.raw','rb');
[b2,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p7uni - Copy.raw','rb');
[b3,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p9uni - Copy.raw','rb');
[b4,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p11uni - Copy.raw','rb');
[b5,count]=fread(fid,[256,256]);
fclose(fid);

fid=fopen('p3Gaus2 - Copy.raw','rb');
[c1,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p5Gaus2 - Copy.raw','rb');
[c2,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p7Gaus2 - Copy.raw','rb');
[c3,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p9Gaus2 - Copy.raw','rb');
[c4,count]=fread(fid,[256,256]);
fclose(fid);
fid=fopen('p11Gaus2 - Copy.raw','rb');
[c5,count]=fread(fid,[256,256]);
fclose(fid);

ps=[3 psnr(a,b1);5 psnr(a,b2);7 psnr(a,b3);9 psnr(a,b4);11 psnr(a,b5)];
ps=ps';
plot(ps(1,:),ps(2,:),'*');
hold on;
ps2=[3 psnr(a,c1);5 psnr(a,c2);7 psnr(a,c3);9 psnr(a,c4);11 psnr(a,c5)];
ps2=ps2';
plot(ps2(1,:),ps2(2,:),'+');
title('PSNR-N');
xlabel('N (filter size)');
ylabel('PSNR');