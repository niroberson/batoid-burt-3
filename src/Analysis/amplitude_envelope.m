clear, clc
L = 21;
f = 0.1; % Hz
lambda = L/12;
Amax = 0.1*39.3701;
k = 1/lambda;
x = 0:0.001:L;

%% Define several waveforms
lin_inc = @(t) x*Amax/L.*sin(k*x-f*t);
exp_incr = @(t) x.^2*Amax/L.^2.*sin(k*x-f*t);
x_half1 = floor(numel(x)/2);
x_half2 = ceil(numel(x)/2);
lin_inc_dec = @(t) [x(1:x_half1).*Amax/L Amax-x(x_half2:end).*Amax/L].*sin(k*x-f*t);
exp_inc_dec = @(t) [x(1:x_half1).^(1/2).*Amax/L^(1/2) Amax-x(x_half2:end).^(1/2).*Amax/L^(1/2)].*sin(k*x-f*t);

%% Plot the thing
figure, hold on
h = plot(x,lin_inc_dec(12));
plot(x, [x(1:x_half1).*Amax/L Amax-x(x_half2:end).*Amax/L], 'r--')
plot(x, [x(1:x_half1).*Amax/L Amax-x(x_half2:end).*Amax/L], 'r--')
xlim([x(1) x(end)])
ylim([-Amax Amax])
xlabel('x[in]')
ylabel('y[in]')

%% Save the frame and setup the output movie
Nz = 100; % s 
fps = 1000;
filename = 'waveform1.gif';
init_getframe = struct('cdata',[],'colormap',[]);
frames = repmat(init_getframe, Nz, 1 );
frames(1) = getframe;
im = frame2im(frames(1));
[imind,cm] = rgb2ind(im,256);
imwrite(imind,cm,filename,'gif', 'Loopcount',inf);


%% Make movie
for t = 1:1:Nz
    set(h,'YData', lin_inc_dec(t));
    drawnow
    frames(t) = getframe;
    im = frame2im(frames(t));
    [imind,cm] = rgb2ind(im,256);
    pause(1/1000)
end

%% Test amplitude envelope functions
A = @(x, e, th, q, Amax) Amax/(L.^e)*(th*x-q).^e;
y = @(x, f, k, t)sin(k*x - f*t);

Ax = A(x, 2, 1, L/2, 0.1);
Yx = y(x, f, k, 0);
wave = Ax.*Yx;
figure, hold on
plot(x,wave)
plot(x, Ax, 'r--')
plot(x, -Ax, 'r--')