
% p1 = rot90(double( rgb2gray(imread('data/00003phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('data/00003phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('data/00003phase3.jpg'))));

% p1 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('../Processing/ThreePhase/img/phase3.jpg'))));

[t,angles1,unwrapped_angles1,pd]=structuredlight('../Processing/SimThreePhase/phase1001.jpg','../Processing/SimThreePhase/phase1002.jpg','../Processing/SimThreePhase/phase1003.jpg');

len1 = length(angles1);
angles = zeros(5,len1);
angles(1,:) = angles1;
unwrapped_angles = zeros(5,len1);
uangles = unwrapped_angles;
unwrapped_angles(1,:) = unwrapped_angles1;


[t,angles(2,:),unwrapped_angles(2,:),pd]=structuredlight('../Processing/SimThreePhase/phase1004.jpg','../Processing/SimThreePhase/phase1005.jpg','../Processing/SimThreePhase/phase1006.jpg');
[t,angles(3,:),unwrapped_angles(3,:),pd]=structuredlight('../Processing/SimThreePhase/phase1007.jpg','../Processing/SimThreePhase/phase1008.jpg','../Processing/SimThreePhase/phase1009.jpg');
[t,angles(4,:),unwrapped_angles(4,:),pd]=structuredlight('../Processing/SimThreePhase/phase1010.jpg','../Processing/SimThreePhase/phase1011.jpg','../Processing/SimThreePhase/phase1012.jpg');
[t,angles(5,:),unwrapped_angles(5,:),pd]=structuredlight('../Processing/SimThreePhase/phase1013.jpg','../Processing/SimThreePhase/phase1014.jpg','../Processing/SimThreePhase/phase1015.jpg');

flat_angles = [1:len1]/len1;
flat_angles = flat_angles*1.6+1;

maxi = size(angles,1);
for i = [1:maxi]
    uangles(i,:) = unwrapped_angles(i,:)/(2^(i-1)) - flat_angles;
    uangles(i,:) = uangles(i,:) - uangles(i,end/2)
    unwrapped_angles(i,:) = unwrapped_angles(i,:); %/(2^(i-1));
    
    angles(i,:) = angles(i,:)+i*1.2;
end



figure(1);
subplot(2,1,1),plot(t,angles);
subplot(2,1,2),plot(t,unwrapped_angles)

figure(2);
plot(t(end/4:end-end/4),uangles(:,end/4:end-end/4),t(end/4:end-end/4),pd(end/4:end-end/4)/(13.0*max(max(abs(pd(end/4:end-end/4))))));
