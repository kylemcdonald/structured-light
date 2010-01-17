
% p1 = rot90(double( rgb2gray(imread('data/00003phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('data/00003phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('data/00003phase3.jpg'))));

% p1 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase3.jpg'))));

[t,angles1,unwrapped_angles1,pd]=structuredlight('../Processing/SimThreePhase/phase1001.jpg','../Processing/SimThreePhase/phase1002.jpg','../Processing/SimThreePhase/phase1003.jpg', '../Processing/SimThreePhase/heightdata.dat');

len1 = length(angles1);
angles = zeros(5,len1);
angles(1,:) = angles1;
unwrapped_angles = zeros(5,len1);
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
skew = 0.9465;
scale = 2900;

flat_angles = [1:len1]/len1;
flat_angles = flat_angles - flat_angles(end/2);
flat_angles = flat_angles*skew;

maxi = size(angles,1);
for i = [1:maxi]
    uangles(i,:) = scale*(unwrapped_angles(i,:)/(2^(i-1)) - flat_angles);
    uangles(i,:) = uangles(i,:) - uangles(i,end/2);
    unwrapped_angles_norm(i,:) = unwrapped_angles(i,:)/(2^(i-1));
    
    staggered_angles(i,:) = angles(i,:)+i*1.2;
end

figure(1);
subplot(2,1,1),plot(t,staggered_angles);
subplot(2,1,2),plot(t,unwrapped_angles);

figure(2);
plot(t, flat_angles, t, unwrapped_angles_norm);

figure(3);
plot(t(end/4:end-end/4),uangles(:,end/4:end-end/4),t(end/4:end-end/4),pd(end/4:end-end/4) );
axis([1*size(angles,2)/4 3*size(angles,2)/4 -100 max(pd(end/4:end-end/4))]); %min(pd(end/4:end-end/4))

figure(4)
plot(t(end/4:end-end/4),median(uangles(:,end/4:end-end/4),1), t(end/4:end-end/4), pd(end/4:end-end/4) );
%axis([1*size(angles,2)/4 3*size(angles,2)/4 -10 max(pd(end/4:end-end/4))]); %min(pd(end/4:end-end/4))
