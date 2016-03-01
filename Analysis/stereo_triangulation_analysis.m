clear, clc
vars = {'XL1', 'XL10', 'XL2', 'XL3', 'XL4', ...
    'XL5', 'XL6', 'XL7', 'XL9','XR1', 'XR10', ...
    'XR2', 'XR3', 'XR4', 'XR5', 'XR6', 'XR7', 'XR8', 'XR9'};
allpoints = load('allpoints.mat', vars{:});

%% Constants
xd = [0; 3; 6; 9; 12; 15; 18; 21];
xc = 0:0.001:21;
L = 21;
f = 8; % Rad/s
lambda = L;
Amax = 9; % degrees
k = (2*pi)/lambda;
Nz = 10; % s 
dt = 1; % s

%% Linearly increasing amplitude
A = @(x) x*Amax/L;
y = @(x, t) sin(k*x - f*t);

figure(1), hold on
h1 = plot(xd, A(xd).*y(xd, 0));
h2 = plot(xc, A(xc).*y(xc, 0));
xlim([xd(1) xd(end)])
ylim([-max(A(xd)) max(A(xd))])
xlabel('x [in]')
ylabel('y [degrees]')

%%
figure, hold on
h1 = plot(xd, A(xd).*f(xd, 0));

for t=1:30
    for i=1:numel(vars)
        vec = allpoints.(vars{i});
        plot3(vec(1,t), vec(2,t), vec(3,t), '.')
    end
end