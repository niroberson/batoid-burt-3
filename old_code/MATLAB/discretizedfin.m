%Discretized motion of fin    Brittany Gater     11/15/2014

clear
clc

L = 1;      % Radial length of fin
A = 0.3;    % Amplitude of fin wave
ilam = 1.5; % # of sine waves on fin at a time
lambda = L/ilam;    % Wavelength
omega = 0.5/(2*pi); % Frequency of flapping

N = 6;      % # of servos
% pos = (0:N)' * L/N;
% pos = [0 0.5 1 2 3 4 5 6]' * L/N;
% pos = [0 1/9 2/9 linspace(1/3,1,4)]';
pos = [0 0.4/3 0.8/3 linspace(0.4,1,4)]';

y =@(x,t) (A*x/L) .*sin(2*pi/lambda.*x - omega*t);
dt = 0.5;
tmax = 540;

for t = 0:dt:tmax
    f =@(x) y(x,t);
    fplot(f,[0 L])
    hold on    
    
    Y = y(pos,t);
    plot(pos,Y,'r')
    xlim([0 L]); ylim([-A A])
    hold off
    
    pause(0.0001)
end