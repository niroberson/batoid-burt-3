clear
clc

theta = pi/6;   %angle difference between rod and neighbors
N = 100;       % # increments used for calculation

yf = 8; %radial length of fin rod, inches
L = 4;  %distance between rods
t = 0.5;    %thickness of fin

dy = yf/N;
A = tan(theta);
z = A*(linspace(0,yf,N));
Lp = sqrt( L^2 + z.^2 );

for i = 1:N
    
end