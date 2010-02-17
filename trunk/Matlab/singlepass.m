clear all;
format compact;

numphases =1;

rotate = 0;
gamma = 1/2.2;
%[t,angles1,unwrapped_angles1,p1,p2,p3,pd]=structuredlight('Default-0001.jpg','Default-0002.jpg','Default-0003.jpg',gamma,rotate);
%[t,angles1,unwrapped_angles1,p1,p2,p3,pd]=structuredlight('Default-0837.jpg','Default-0838.jpg','Default-0839.jpg',rotate);
%[t,angles1,unwrapped_angles1,p1,p2,p3,pd]=structuredlight('../Processing/SimThreePhase/ref1001.jpg','../Processing/SimThreePhase/ref1002.jpg','../Processing/SimThreePhase/ref1003.jpg',1)
[t,angles1,unwrapped_angles1,p1,p2,p3,pd]=structuredlight(...
        '../Processing/ThreePhase/img/phase1.jpg',...
        '../Processing/ThreePhase/img/phase2.jpg',...
        '../Processing/ThreePhase/img/phase3.jpg',...
            gamma,1)



len1 = length(angles1);
angles = zeros(numphases,len1);
angles(1,:) = angles1;
unwrapped_angles = zeros(numphases,len1);
uangles = unwrapped_angles;
unwrapped_angles(1,:) = unwrapped_angles1;
unwrapped_angles_norm = unwrapped_angles;


%%
% increasing skew will tend to tilt the unwrapped angles to the left,
% decreasing will tilt to the right
skew = -28; %28.36;% %0.9465;
scale = 1;

flat_angles = [1:len1]/len1;
flat_angles = flat_angles - flat_angles(end/2);

skewp = polyfit(flat_angles(end:end-end/2),(unwrapped_angles(end:end-end/2)),1)
flat_angles_skewed = flat_angles*skew; %skewp(1) + skewp(2);%refunwrapped_angles/div;

maxi = size(angles,1);
for i = [1:maxi]
    uangles(i,:) = scale*(unwrapped_angles(i,:)/(2^(i-1)) - flat_angles_skewed);
    uangles(i,:) = uangles(i,:) - uangles(i,end/2);
    unwrapped_angles_norm(i,:) = unwrapped_angles(i,:)/(2^(i-1));
    
    staggered_angles(i,:) = angles(i,:)+i*1.2;
end

median_uangles = median(uangles,1);
% true_scale = polyfit(median_uangles(ind), pd(ind),1);

figure(1);
subplot(4,1,1), plot(t,p1,t,p2,t,p3);
ylabel('source three phases');
% hold on;
% plot(t, sin(t*0.31)*75+170,'k.');
% hold off;
subplot(4,1,2), plot(t,sqrt(3) * (p1 - p3), t, (2*p2 - p1 - p3) );
ylabel('atan2 parameters');
subplot(4,1,3),plot(t,staggered_angles);
ylabel('wrapped angles');
% subplot(4,1,3),plot(t,unwrapped_angles); %, t, refunwrapped_angles );
% ylabel('unwrapped angles');
subplot(4,1,4),plot(t,uangles); %, t, refunwrapped_angles );
ylabel('unwrapped angles');

% figure(2);
% subplot(2,1,1),plot(t, unwrapped_angles_norm, t,flat_angles_skewed ); %t, refuangle/div,t, refunwrapped_angles/div,
% subplot(2,1,2), plot(t, median(unwrapped_angles_norm,1),t,flat_angles_skewed);%t , flat_angles*skewp(1) + skewp(2) );

% figure(3);
% ind = length(t)/4:length(t)-length(t)/4;
% tm = t(ind);
% subplot(2,1,1),
% plot(tm,uangles(:,ind),tm,pd(ind));
% axis([1*size(angles,2)/4 3*size(angles,2)/4 -100 max(pd(ind))]); %min(pd(end/4:end-end/4))
% %subplot(2,1,2),plot( tm , refunwrapped_angles(end/4:end-end/4));
% 
% subplot(2,1,2),plot(tm,median_uangles(:,ind), tm, pd(ind) );
% axis([1*size(angles,2)/4 3*size(angles,2)/4 -100 max(pd(ind))]);
% %axis([1*size(angles,2)/4 3*size(angles,2)/4 -10 max(pd(end/4:end-end/4))]); %min(pd(end/4:end-end/4))
