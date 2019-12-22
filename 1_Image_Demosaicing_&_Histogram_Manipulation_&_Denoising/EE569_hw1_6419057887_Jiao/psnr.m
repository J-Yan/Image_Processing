function [p] = psnr(ori,out)
    d=abs(ori-out);
    e=d.*d;
    f=sum(sum(e))
    /256/256;
    p=10*log10(255*255./f);
end