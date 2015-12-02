clear, clc, close all
%% Define several waveforms
lin_inc = @(x,Amax,L) x*Amax/L;
exp_incr = @(x,Amax,L) x.^2*Amax/L.^2;

% Fix these with xhalf shit
lin_inc_dec = @(x,Amax,L) [x.*Amax/L; Amax-x(5:end).*Amax/L];
exp_inc_dec = @(x,Amax,L) [x(1:4).^(1/2).*Amax/L^(1/2); Amax-x(5:end).^(1/2).*Amax/L^(1/2)];

%% Define wave parameters
x = [0; 3; 6; 9; 12; 15; 18; 21;];
L = 21;
f = 0.5*2*pi; % Hz
lambda = L;
Amax = 0.1;
k = (2*pi)/lambda;
Nz = 10; % s 
dt = 0.1; % s
filename = 'waveform1.gif';
y = @(t) sin(k*x - f*t);
create_fin_gif(y, lin_inc(x, Amax, L), x, dt, Nz, filename)


