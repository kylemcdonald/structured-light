function [angles,unwrapped_angles,p1]=slight;
% binarymillenium Jan 2010
% GNU GPL v3.0

% phases generates any number of equally spaced sine waves.
% 
% slight (need to rename to avoid collision with existing matlab slight) loads images and performs the structured light calculations on them.  It is very slow, so is best suited for only narrow strips of the images.
% 
% Copying from https://binarymillenium.googlecode.com/svn/trunk/processing/simstructuredlight
% 
% p1 = rot90(double( rgb2gray(imread('data/00003phase1.jpg'))));
% p2 = rot90(double( rgb2gray(imread('data/00003phase2.jpg'))));
% p3 = rot90(double( rgb2gray(imread('data/00003phase3.jpg'))));

p1 = rot90(double( rgb2gray(imread('data/phase1.jpg'))));
p2 = rot90(double( rgb2gray(imread('data/phase2.jpg'))));
p3 = rot90(double( rgb2gray(imread('data/phase3.jpg'))));


figure(10);
colormap('gray')
image(p1/4);

p1 = p1(end/2, end/4:3*end/4);
p2 = p2(end/2, end/4:3*end/4);
p3 = p3(end/2, end/4:3*end/4);

% p1 = double( rgb2gray(imread('data/i1.png')));
% p2 = double( rgb2gray(imread('data/i2.png')));
% p3 = double( rgb2gray(imread('data/i3.png')));

p = zeros(size(p1,1),size(p1,2),3);

p(:,:,1) = p1;
p(:,:,2) = p2;
p(:,:,3) = p3;

pavg = sum(p,3)/3;



figure(1);


%colormap('gray')
%image(pavg/4);
plot(p1);


figure(2);

angles = get_angle_full(p); %p(1:1,1:end/8,:));


x = [1:size(p1,2)/8];
p1sub = p1(1:1,:); %1:end/8);
p2sub = p2(1:1,:); %1:end/8);
p3sub = p3(1:1,:); %1:end/8);

% subplot(2,1,1), plot(x,p1sub, x,p2sub, x,p3sub);
% subplot(2,1,2), plot(x,angles(1:1,1:floor(end/8)));
figure(3),
%subplot(1,2,1), image(p1/4);
%subplot(1,2,2),

plot(angles);
%image(255/4*angles/max(max(angles)));
%colormap('gray');

figure(4),

unwrapped_angles = get_unwrapped(angles);
% TBD the offsets are just arbitrary to the images I'm using
plot(unwrapped_angles);
%image(255/4*(unwrapped_angles+4)/12);
%colormap('gray');







%%
if (0)
figure(2);

p1_fft = fft((p1));

p1_fft = p1_fft(1:size(p1_fft,1)/4,:);
subplot(2,1,2),plot( angle(p1_fft(:,[1:40:end]) ) );
%image(255*angle(p1_fft)/pi);
colormap('gray');
%colormap('jet');
subplot(2,1,1),plot(abs(p1_fft(:,[1:40:end])));
%image(255*abs(p1_fft)/ max(max(abs(p1_fft) )));


% find the peak frequency

avgfft = (sum(abs(p1_fft')));
ofs = 20;
ofs_avgf = avgfft(ofs:end);
peak_f = ofs + find(ofs_avgf == max(ofs_avgf));
display(['peak freq at ' num2str(peak_f)]);


figure(3);

flatp1 = double(reshape(p1,prod(size(p1)),1));
flatp2 = double(reshape(p2,prod(size(p2)),1));
flatp3 = double(reshape(p3,prod(size(p3)),1));
flatpa = double(reshape(pavg,prod(size(pavg)),1));
numbins = 128;
[n1,x1] = hist(flatp1,numbins);
[n2,x2] = hist(flatp2,numbins);
[n3,x3] = hist(flatp3,numbins);
[na,xa] = hist(flatpa,numbins);

%plot(xa,na-n3);
plot(xa,na,x1,n1,x2,n2,x3,n3);
end

%%
% p is a width * height * number of phases matrix
function angles = get_angle_full(p)

[h,w,phasenum] = size(p);

angles = zeros(h,w) -0.5;

for y=[1:h]
    for x = [1:w]
        i1 = p(y,x,1);
        i2 = p(y,x,2);
        i3 = p(y,x,3);
        angles(y,x) = 0.5+ atan2(sqrt(3) * (i1 - i3), 2*i2 - i1 - i3) / (2*pi);
        %get_angle(p(y,x,:));
    end
end

%%
% p is a vector of length number of phases
% angle is normalized 0.0 to 1.0
function angle = get_angle(ps)

% there will be 2*phasenum brackets in which to place the angles
phasenum = length(ps);

[ps_sorted,i] = sort(ps);

frt = ps_sorted(end);
frt_i = i(end);

sec = ps_sorted(end-1);
sec_i = i(end-1);

trd = ps_sorted(end-2);
trd_i = i(end-2);

angle_min = ((frt_i-1)- 0.5)/phasenum;

if ((frt-trd) > 0)
    angle_offset = (sec-trd)/(frt-trd)*0.5/phasenum;

    if (mod(sec_i-trd_i,phasenum) == 1)
        angle = angle_min - angle_offset; %angle_min + 0.5/phasenum - angle_offset
    else
        angle = angle_min + angle_offset; %angle_min; % + angle_offset;
    end

   %angle =angle_min;
   angle = mod(angle,1);
else  
   angle = -2;   
end

% for i = [1:phasenum]
%    testval = ps(i);
%    restvals = [ps(1:i-1),ps(:i+1:end)];
%    
%    if testval > max(restvals) 
%       % now we know we are in a section of angle ((i-1) +/- 0.5)/phasenum 
%    end
% end

%% 
function [unwrapped_angles, has_unwrapped, queue] = unwrap(unwrapped_angles, has_unwrapped, queue,item)

[hmax,wmax]= size(unwrapped_angles);

h = item(1);
w = item(2);
hn = item(3);
wn = item(4);

if (hn <1) || (hn > hmax) || (wn <1) || (wn > wmax) || (has_unwrapped(hn,wn) == 1)
    return
end

new_angle = unwrapped_angles(hn,wn);
 
base = floor(unwrapped_angles(h,w));
old_angle = unwrapped_angles(h,w) - base;

tol = 0.1;

unwrapped_angles(hn,wn) = unwrapped_angles(hn,wn) + base;

if (old_angle > 1-tol) && (new_angle < tol) 
   unwrapped_angles(hn,wn) = unwrapped_angles(hn,wn) +1;  
elseif (old_angle < tol) && (new_angle > 1 - tol)
   unwrapped_angles(hn,wn) = unwrapped_angles(hn,wn) -1; 
end

%display([ num2str(unwrapped_angles(hn,wn)) ' ' num2str(base) ' ' num2str(hn) ' ' num2str(wn)]);
has_unwrapped(hn,wn) = 1;

queue = [queue; [hn,wn,hn,wn+1]];
queue = [queue; [hn,wn,hn,wn-1]];
queue = [queue; [hn,wn,hn+1,wn]];
queue = [queue; [hn,wn,hn-1,wn]];



%%
function unwrapped_angles = get_unwrapped(angles)

[h,w] = size(angles);

sh = floor(h/2);
sw = floor(w/2);
if sh == 0 
    sh = 1
end
if sw == 0
    sw = 1
end

has_unwrapped = zeros(h,w);
has_unwrapped(sh,sw) = 1;

unwrapped_angles = angles;

queue = [sh,sw,sh,sw+1];
queue = [queue; [sh,sw,sh,sw-1]];
queue = [queue; [sh,sw,sh+1,sw]];
queue = [queue; [sh,sw,sh-1,sw]];

do_loop = 1;

tot = prod(size(unwrapped_angles));
count = 0;

figure(4);
while (do_loop == 1)    
    [unwrapped_angles,has_unwrapped, queue] = unwrap(unwrapped_angles,has_unwrapped, queue, queue(1,:));
    queue = queue(2:end,:);
    count= count+1;
    if (mod(count,1000) == 0)
        display([ num2str(count/tot) ' ' num2str(size(queue,1)) ' ' num2str(sum(sum(has_unwrapped))/tot)] );
    end
    if (size(queue,1) < 1)
        do_loop = 0
    end
end