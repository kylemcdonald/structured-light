clear all;
format compact;


% p1 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase3.jpg'))));

numphases = 5;
div = (2^(numphases-1));

% this will get the period of the highest frequency phase
[skew,scale, period,refuangle]=skewfromref('../Processing/SimThreePhase/ref1001.jpg',1.0,div);

period
skew
scale

[t,refangles,refunwrapped_angles,pd]=structuredlight('../Processing/SimThreePhase/ref1001.jpg','../Processing/SimThreePhase/ref1002.jpg','../Processing/SimThreePhase/ref1003.jpg', '../Processing/SimThreePhase/heightdata.dat');

%figure(40),
%plot(t,refangles)

[t,angles1,unwrapped_angles1,pd]=structuredlight('../Processing/SimThreePhase/phase1001.jpg','../Processing/SimThreePhase/phase1002.jpg','../Processing/SimThreePhase/phase1003.jpg', '../Processing/SimThreePhase/heightdata.dat');

len1 = length(angles1);
angles = zeros(numphases,len1);
angles(1,:) = angles1;
unwrapped_angles = zeros(numphases,len1);
uangles = unwrapped_angles;
unwrapped_angles(1,:) = unwrapped_angles1;
unwrapped_angles_norm = unwrapped_angles;


[t,angles(2,:),unwrapped_angles(2,:),pd]=structuredlight('../Processing/SimThreePhase/phase1004.jpg','../Processing/SimThreePhase/phase1005.jpg','../Processing/SimThreePhase/phase1006.jpg', '../Processing/SimThreePhase/heightdata.dat');
[t,angles(3,:),unwrapped_angles(3,:),pd]=structuredlight('../Processing/SimThreePhase/phase1007.jpg','../Processing/SimThreePhase/phase1008.jpg','../Processing/SimThreePhase/phase1009.jpg', '../Processing/SimThreePhase/heightdata.dat');
[t,angles(4,:),unwrapped_angles(4,:),pd]=structuredlight('../Processing/SimThreePhase/phase1010.jpg','../Processing/SimThreePhase/phase1011.jpg','../Processing/SimThreePhase/phase1012.jpg', '../Processing/SimThreePhase/heightdata.dat');
[t,angles(5,:),unwrapped_angles(5,:),pd]=structuredlight('../Processing/SimThreePhase/phase1013.jpg','../Processing/SimThreePhase/phase1014.jpg','../Processing/SimThreePhase/phase1015.jpg', '../Processing/SimThreePhase/heightdata.dat');

%%
% increasing skew will tend to tilt the unwrapped angles to the left,
% decreasing will tilt to the right
skew = 1.57; %0.9465;
scale = 1.5804e+003;

flat_angles = [1:len1]/len1;
flat_angles = flat_angles - flat_angles(end/2);

skewp = polyfit(flat_angles(end/4:end-end/4),(refunwrapped_angles(end/4:end-end/4)/div),1);
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
subplot(2,1,1),plot(t,staggered_angles);
subplot(2,1,2),plot(t,unwrapped_angles, t, refunwrapped_angles );

figure(2);
subplot(2,1,1),plot(t, unwrapped_angles_norm, t, refuangle/div, t, refunwrapped_angles/div,t,flat_angles_skewed );
subplot(2,1,2), plot(t, median(unwrapped_angles_norm,1),t,flat_angles_skewed, t , flat_angles*skewp(1) + skewp(2) );

figure(3);
ind = length(t)/4:length(t)-length(t)/4;
tm = t(ind);
subplot(2,1,1),
plot(tm,uangles(:,ind),tm,pd(ind));
axis([1*size(angles,2)/4 3*size(angles,2)/4 -100 max(pd(ind))]); %min(pd(end/4:end-end/4))
%subplot(2,1,2),plot( tm , refunwrapped_angles(end/4:end-end/4));



subplot(2,1,2),plot(tm,median_uangles(:,ind), tm, pd(ind) );
axis([1*size(angles,2)/4 3*size(angles,2)/4 -100 max(pd(ind))]);
%axis([1*size(angles,2)/4 3*size(angles,2)/4 -10 max(pd(end/4:end-end/4))]); %min(pd(end/4:end-end/4))
