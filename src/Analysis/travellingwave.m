clear, clc, close all

%% Define wave parameters
x = [0; 3; 6; 9; 12; 15; 18; 21;];
L = 21;
f = 0.5; % Hz
lambda = L;
Amax = 0.1;
k = (2*pi)/lambda;

%% Define several waveforms
lin_inc = @(Amax, t) x*Amax/L.*sin(k*x-f*t);
exp_incr = @(t) x.^2*Amax/L.^2.*sin(k*x-f*t);
lin_inc_dec = @(t) [x(1:4).*Amax/L; Amax-x(5:end).*Amax/L].*sin(k*x-f*t);
exp_inc_dec = @(t) [x(1:4).^(1/2).*Amax/L^(1/2); Amax-x(5:end).^(1/2).*Amax/L^(1/2)].*sin(k*x-f*t);

%% Set up your plot environment

figure, hold on
h = plot(x,lin_inc(Amax, 0), '.r', x,lin_inc(Amax, 0));
xlim([x(1) x(end)])
ylim([-max(Amax) max(Amax)])
title('\lambda=L \omega=8 rad/s A=4...12 degrees')

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
for t = 1:Nz
    set(h,'YData', lin_inc(Amax, t));
    drawnow
    frames(t) = getframe;
    im = frame2im(frames(t));
    [imind,cm] = rgb2ind(im,256);
    imwrite(imind,cm,filename,'gif','WriteMode','append','delaytime',1/fps);
    pause(1/1000)
end

%% Play movie
movie(frames, 10, fps)