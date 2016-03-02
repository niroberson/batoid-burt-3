clear, clc, close all
%% Define several waveforms
lin_inc = @(x) x*Amax/L;
exp_incr = @(x) x.^2*Amax/L.^2;

% Fix these with xhalf shit
x_half1 = floor(numel(xc)/2);
x_half2 = ceil(numel(xc)/2);
%  [x(1:x_half1).*Amax/L Amax-x(x_half2:end).*Amax/L]
exp_inc_dec = @(x)  [x(1:x_half1).^(1/2).*Amax/L^(1/2) Amax-x(x_half2:end).^(1/2).*Amax/L^(1/2)];

%% Constants
xd = [3, 6, 9, 12, 15, 18, 21, 23];
xc = 3:.1:23;
L = 23;
f = 5; % Rad/s
lambda = L;
Amax = 10; % degrees
k = (2*pi)/lambda;
Nz = 10; % s 
dt = 0.05; % s

% General wave functions
A = @(x, e, th, q) Amax/(L.^e)*(th*x-q).^e;
y = @(x, t) sin(k*x - f*t);

%% Linearly increasing amplitude
filename = 'undulate8.gif';
A = @(x) x*Amax/L;
create_fin_gif(y, A, xd, xc, dt, Nz, filename)

%% Linearly increasing decreasing amplitude
A = @(x, t) Amax*(x/L).^(1/2).*(x<L/2) + (- Amax*((x - L/2) /L).^(1/2)).*(x>L/2);
create_fin_gif(y, A, xd, xc, dt, Nz, filename)


%% Airfoil shape functions
m = 0.001;
p = 0.5;
t = 0.1;
L = 21;
yth = @(x) (5*t*L)*(0.2969*(x/L).^(1/2) - 0.1260*(x/L) - 0.3537*(x/L).^2 + 0.2843*(x/L).^3 - 0.1015*(x/L).^4);
yc = @(x) ((m/p^2)*(2*p*(x/L) - (x/L).^2)).*(1 - heaviside(x/L - p)) + ((m/(1-p)^2)*(1 - 2*p + 2*p*(x/L) - (x/L).^2)).*heaviside(x/L - p)

x = 0:0.001:21;
ycx = yc(x);
% theta = atan((ycx(end)-ycx(end-1))/(x(end)-x(end-1)));
theta = -5/(2*pi)
xu = x - yth(x)*cos(theta);
yu = yc(x) + yth(x)*sin(theta);

xl = x + yth(x)*cos(theta);
yl = yc(x) - yth(x)*sin(theta);

figure, hold on
plot(xu, yu, 'r')
plot(xl, yl, 'g')
plot(xc, yc(xc), 'bl')

%% More airfoil shape functions
