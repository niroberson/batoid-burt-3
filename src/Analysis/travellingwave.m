clear, clc, close all
%% Define several waveforms
lin_inc = @(x) x*Amax/L;
exp_incr = @(x) x.^2*Amax/L.^2;

% Fix these with xhalf shit
lin_inc_dec = @(x) [x.*Amax/L; Amax-x(5:end).*Amax/L];
exp_inc_dec = @(x) [x(1:4).^(1/2).*Amax/L^(1/2); Amax-x(5:end).^(1/2).*Amax/L^(1/2)];

%% Define wave parameters
xd = [0; 3; 6; 9; 12; 15; 18; 21;];
xc = 0:0.001:21;
L = 21;
f = 1.2*2*pi; % Hz
lambda = L;
Amax = 0.1;
k = (2*pi)/lambda;
Nz = 10; % s 
dt = 0.05; % s
filename = 'waveform12.gif';
y = @(x, t) sin(k*x - f*t);
A = @(x) x*Amax/L;
create_fin_gif(y, A, xd, xc, dt, Nz, filename)


