%%
function structured_light()

gen_sawtooths(3);

function gen_sawtooths(n)

t = [1:500];
y = zeros(n, length(t));

for i = [1:n]
    
    new_y =  sawtooth(t,floor(length(t)/n)*(i-1)+1);
    
    y(i,:) = new_y;
% y2 = sawtooth(t,floor(500/n));
% y3 = sawtooth(t,floor(500/3*2));
end


figure(1);
plot(t/max(t),y);


%% generate one sawtooth cycle
function y=sawtooth(t,phase)

y = zeros(size(t));
ynew = 2*[ [floor(t(end)/2-1):-1:0], [0:1:floor(t(end)/2-1)] ]/t(end);
y(1:length(ynew)) = ynew;

y = [y(phase:end), y(1:phase-1)];