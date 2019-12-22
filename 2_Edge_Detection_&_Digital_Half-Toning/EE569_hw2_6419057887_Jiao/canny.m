low = [0.1 0.2 0.3 0.4 0.1 0.2 0.3 0.4];
high = [0.15 0.25 0.35 0.45 0.2 0.3 0.4 0.5];
for i = 1:1:8
    TigCan = edge(Tig_gray,"Canny",[low(i),high(i)]);
%     r=floor(i/0.5)+1;
%     c=mod(i/0.1,5);
    subplot(3,4,i);
    imshow(TigCan);
    title([num2str(low(i)*100),'% ',num2str(high(i)*100),'%']);
    
end
low = [0.1 0.2 0.3 0.4];
high = [0.25 0.35 0.45 0.55];
for i = 1:1:4
    TigCan = edge(Tig_gray,"Canny",[low(i),high(i)]);
%     r=floor(i/0.5)+1;
%     c=mod(i/0.1,5);
    subplot(3,4,i+8);
    imshow(TigCan);
    title([num2str(low(i)*100),'% ',num2str(high(i)*100),'%']);
    
end
