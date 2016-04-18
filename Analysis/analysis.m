%% Amplitude Envelope Analysis
% Plot projected area over time of various amplitude envelopes
x = 0:3.5:24.5;
x1 =  [28, 24.5, 21, 17.5, 14, 10.5, 5, 3.5];
x2 = [24.5, 21, 17.5, 14, 10.5, 5, 3.5, 0];
x3 = [21, 17.5, 14, 10.5, 5, 3.5, 0, 24.5];
x4 = [17.5, 14, 10.5, 5, 3.5, 0, 21, 24.5];

L = 28;
lambda = L;
Amax = 7;
w = 7;
A = @(x)Amax*x/L;
k = 2 * pi / lambda;
y = @(x, t) A(x) .* sin(k.* x - w * t);

figure,plot(x, y(x1,0))