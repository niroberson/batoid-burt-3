clear, clc, close all
vars = {'XL1', 'XL2', 'XL3', 'XL4', ...
    'XL5', 'XL6', 'XL7', 'XL8', 'XL9', 'XL10'};
%     'XR1', 'XR10', ...
%     'XR2', 'XR3', 'XR4', 'XR5', 'XR6', 'XR7', 'XR8', 'XR9'};
allpoints = load('allpoints.mat', vars{:});

%% Rearrange the data into 8 points (3d) by time
point_mat = zeros(numel(vars),3,31);
average_mat = zeros(numel(vars),3);
for i=1:numel(vars)
    vec = allpoints.(vars{i});
    point_mat(i, 1, :) = vec(1, :);
    point_mat(i, 2, :) = vec(2, :);
    point_mat(i, 3, :) = vec(3, :);
    average_mat(i, 1) = mean(vec(1, :));
    average_mat(i, 2) = mean(vec(2, :));
    average_mat(i, 3) = mean(vec(3, :));
end

%% Find line of best fit for average points on fin
fin_neutral_mat = average_mat(3:10,:);
rig_mat = average_mat(1:2, :);
% r0=mean(fin_neutral_mat);
% fin_neutral_mat=bsxfun(@minus,fin_neutral_mat,r0);
% [~,~,V]=svd(fin_neutral_mat,0);
% direction=V(:,1);

%% Find scaling factor by using fin length
fin_length = sqrt((average_mat(3, 1)-average_mat(10,1))^2 + ...
    (average_mat(3,2) - average_mat(10,2))^2 + ...
    (average_mat(3,3) - average_mat(10,3))^2);
q = fin_length/23;

%% Create 3d fit line matrix
% fin_fit_mat = zeros(20, 3);
% for t=1:570
%     fin_fit_mat(t, :) = r0 + t.*direction';
% end

%% Find the average spar spacing
d = @(x1,x2,y1,y2,z1,z2) sqrt((x2-x1)^2+(y2-y1)^2+(z2-z1)^2);

% Find the amplitudes over time
fin_mat = point_mat(3:10, :, :);
A_exp = zeros(size(fin_mat,1),31);
for t=1:size(fin_mat,3)
    for i=1:size(fin_mat,1)
        A_exp(i,t) = d(fin_mat(i,1,t),fin_neutral_mat(i,1),fin_mat(i,2,t),fin_neutral_mat(i,2),fin_mat(i,3,t),fin_neutral_mat(i,3));
        vec = [fin_mat(i,1,t)-fin_neutral_mat(i,1) fin_mat(i,2,t)-fin_neutral_mat(i,2) fin_mat(i,3,t)-fin_neutral_mat(i,3)];
        sign(vec)
        s = sign(vec(3));
        A_exp(i,t) = s*A_exp(i,t);
    end
end

exp_spacing = zeros(size(fin_neutral_mat,1) - 1, 1);
for i=2:size(fin_neutral_mat,1)
    exp_spacing(i - 1) = d(fin_neutral_mat(i,1),fin_neutral_mat(i-1,1),fin_neutral_mat(i,2),fin_neutral_mat(i-1,2),fin_neutral_mat(i,3),fin_neutral_mat(i-1,3));
end

% Normalize data
exp_spacing = exp_spacing/q;
xd_exp = zeros(numel(exp_spacing)+1,1) + 3;
for i=1:numel(exp_spacing)
    xd_exp(i+1) = xd_exp(i) + exp_spacing(i);
end
A_exp = A_exp/q;

%% Constants
xd = [6, 9, 12, 15, 18, 21, 23, 26];
xc = 6:0.1:26;
L = 26;
f = 5; % Rad/s
lambda = L;
spar_length = 7;
Amax = 10*pi/180*spar_length; % inches
k = (2*pi)/lambda;
dt = 1; % s

%% Linearly increasing amplitude
A = @(x) (L-x + 3)*Amax/L;
y = @(x, t) sin(k*(L-x) - f*t);

figure(1), hold on
h1 = plot(xd/L, A(xd).*y(xd, 0));
h2 = plot(xd_exp/L, A_exp(:,1));
h3 = plot(xc/L, A(xc).*y(xc, 0));
xlim([0 1])
ylim([-max(A(xd)) max(A(xd))])
xlabel('x [in]')
ylabel('y [degrees]')

%% Run timesteps

for t=2:dt:31
    t_offset = t/6 + 0.84;
    set(h1,'YData', A(xd).*y(xd, t_offset));
    set(h2,'YData', A_exp(:,t));
    set(h3,'YData', A(xc).*y(xc, t_offset));
    pause(dt)
end

