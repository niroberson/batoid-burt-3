clear, clc

%% Calibration
config_info = struct();
config_info.direct = 'cal-4-5/';
cal = compute_stats(config_info);
c = [19.8 39.8 59.3 89.6 129.3];
avg0 = [];
parm_names = fields(cal.avg);
for idxParm = 1:numel(parm_names)
    avg0 = [avg0 cal.avg.(parm_names{idxParm})];
end
avg0 = avg0(1:end-1);

F = c*9.81/1000;
% Linear fit the data
P = polyfit(avg0, F, 1);
Ffit = P(1)*avg0 + P(2);
Rsq = 1 - sum((avg0 - Ffit).^2)/sum((avg0 - mean(avg0)).^2);

figure, hold on
plot(avg0, F, '.', 'markersize', 30)
plot(avg0, Ffit, 'r-.')
xlabel('Voltage')
ylabel('Force')

% State conversion function
Fn = @(V) V*P(1) + P(2);

%% Silicon Fin
config_info = struct();
config_info.direct = 'silicon-4-7/';
silicon_results = compute_stats(config_info);
sw = [10 10 10 10 10 10 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6 6 6 6 7 7 7 7 7 7 7 7 7, ...
    7 7 7 7 8 8 8 8 8 8 8 8 8 8 8 8 9 9 9 9 9 9 9 9 9 9];
sa = [10 5 6 7 7 8 10 11 12 12 7 7 8 8 9 9 10 10 11 11 12 12 7 7 8 8 9 9 10 10 11 11 12 12 , ...
    5 7 7 8 8 9 9 10 10 11 11 12 12 5 7 8 8 9 9 10 11 12 5 7 7 8 8 9 9];

avg1 = [];
err1 = [];
parm_names = fields(silicon_results.avg);
for idxParm = 1:numel(parm_names)
    avg1 = [avg1 silicon_results.avg.(parm_names{idxParm})];
    err1 = [err1 silicon_results.std.(parm_names{idxParm})];
end

%% Lycra Fin
config_info = struct();
config_info.direct = 'lycra-4-7/';
lycra_results = compute_stats(config_info);
lw = [10 5 7 7 7 7 7 7 8 8 8 8 8 8 9 9 9 9 9 9];
la = [7 7 10 11 12 7 8 9 10 11 12 7 8 9 10 11 12 7 8 9];

avg2 = [];
err2 = [];
parm_names = fields(lycra_results.avg);
for idxParm = 1:numel(parm_names)
    avg2 = [avg2 lycra_results.avg.(parm_names{idxParm})];
    err2 = [err2 lycra_results.std.(parm_names{idxParm})];
end

%% Plot lycra and silicon fin of same parameters
figure, hold on
% errorbar(la(lw==7), Fn(avg2(lw==7)), Fn(err2(lw==7)), '.')
% errorbar(la(lw==8), Fn(avg2(lw==8)), Fn(err2(lw==8)), '.')
% errorbar(la(lw==9), Fn(avg2(lw==9)), Fn(err2(lw==9)), '.')

p1 = polyfit(sa(sw==7), Fn(avg1(sw==7)), 2);
p2 = polyfit(sa(sw==8), Fn(avg1(sw==8)), 2);
p3 = polyfit(sa(sw==9), Fn(avg1(sw==9)), 2);
p4 = polyfit(la(lw==7), Fn(avg2(lw==7)), 2);
p5 = polyfit(la(lw==8), Fn(avg2(lw==8)), 2);
p6 = polyfit(la(lw==9), Fn(avg2(lw==9)), 2);


x = linspace(7, 12);
y1 = polyval(p1, x);
y2 = polyval(p2, x);
y3 = polyval(p3, x);
y4 = polyval(p4, x);
y5 = polyval(p5, x);
y6 = polyval(p6, x);


plot(sa(sw==7), Fn(avg1(sw==7)), 'b.')
plot(sa(sw==8), Fn(avg1(sw==8)), 'r.')
plot(sa(sw==9), Fn(avg1(sw==9)), 'y.')
plot(la(lw==7), Fn(avg2(lw==7)), 'm.')
plot(la(lw==8), Fn(avg2(lw==8)), 'g.')
plot(la(lw==9), Fn(avg2(lw==9)), 'black.')

plot(x, y1, 'b')
plot(x, y2, 'r')
plot(x, y3, 'y')
plot(x, y4, 'm')
plot(x, y5, 'g')
plot(x, y6, 'black')

legend('Silicon w=7', 'Silicon w=8', 'Silicon w=9',...
'Lycra w=7', 'Lycra w=8', 'Lycra w=9', 'location', 'best')

%% Analyze Results
figure, hold on
plot3(sw, sa, avg1, '.')
plot3( lw, la, avg2, 'r.')
xlabel('Frequency (Rad/s)')
ylabel('Max Amplitude (Degrees)')
zlabel('Voltage Amplitude')
legend('Silicon Fin', 'Lycra Fin', 'location', 'best')
