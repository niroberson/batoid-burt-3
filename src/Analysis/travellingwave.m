clear, clc, close all

%% Define wave parameters
x = [0; 3; 6; 9; 12; 15; 18; 21;];
L = 21;
f = 0.5; % Hz
lambda = L;
Amax = 0.1;
k = (2*pi)/lambda;

%% Save the frame and setup the output movie
Nz = 100; % s 
fps = 1000;
filename = 'waveform1.gif';

%% Define several waveforms
y = @(t)sin(k*x - f*t);

lin_inc = x*Amax/L;
exp_incr = x.^2*Amax/L.^2;
lin_inc_dec = [x(1:4).*Amax/L; Amax-x(5:end).*Amax/L];
exp_inc_dec = [x(1:4).^(1/2).*Amax/L^(1/2); Amax-x(5:end).^(1/2).*Amax/L^(1/2)];

create_fin_gif(y, lin_inc, x, Nz, fps, filename)