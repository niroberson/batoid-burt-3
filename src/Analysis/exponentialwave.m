clear, clc, close all

%% Define wave parameters
x = [3; 6; 9; 12; 15; 18; 21; 23];
L = 23;
f = 1; % Hz
lambda = L/4;
Amax = 0.1;
k = 1/lambda;
y = @(t) x.^2*Amax/L.^2.*sin(k*x-f*t);

%% Set up your plot environment
figure
h = plot(x,y(0));
xlim([x(1) x(end)])
ylim([-Amax Amax])

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
    set(h,'YData', y(t));
    drawnow
    frames(t) = getframe;
    im = frame2im(frames(t));
    [imind,cm] = rgb2ind(im,256);
    imwrite(imind,cm,filename,'gif','WriteMode','append','delaytime',1/fps);
    pause(1/1000)
end

%% Play movie
movie(frames)