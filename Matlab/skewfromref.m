function [skew, scale, incr,uangle] = skewfromref(refname,targsize,div)
% compute the skew, scale, and angle from the ref image and its known size
% (as measured from the center to the edge)
p1 = rot90(double( sum(imread(refname),3)/3));

p1 = p1(end/2, :);

% angles could be centered around zero, and skew could
% be a lot a smaller
ang=2*(p1/max(p1)-0.5);
angle = acos(ang)/pi;
figure(30), plot(ang);


% this only works okay, might need a full three phase on the ref
% or just count the cycles present
incr = abs(diff(angle));
uangle = incr;
%uangle(end/2) = 0;

uangle(end/2+1:end) = cumsum(incr(end/2+1:end));
uangle(1:end/2)  = -fliplr(cumsum(fliplr(incr(1:end/2))));
uangle(1:end/2) = uangle(1:end/2) + -uangle(floor(end/2))

%angle = angle/div;

maxangle = max(uangle);
pixmax = length(uangle)/2;
pixend = min(find(uangle == maxangle))-pixmax;
uangle = uangle*pixend/pixmax;

skew = 0;
scale = 0;



