clear, clc
%% Linearly increasing amplitude
config_info = struct();
config_info.direct = '4-11-lin-inc/';
lin_inc_results = compute_stats(config_info);
parm_names = fields(lin_inc_results.avg);
amax1 = [10, 10, 10, 11, 11, 11, 12, 12, 12, 15, 15, 15, 17, 17, 17, 20,...
    20, 20, 25, 25, 25, 7, 7, 7, 8, 8, 8, 9, 9, 9];
avg1 = [];
for idxParm = 1:numel(parm_names)
    avg1 = [avg1 lin_inc_results.avg.(parm_names{idxParm})];
end
%% Linearly decreasing amplitude
config_info = struct();
config_info.direct = '4-11-lin-dec/';
lin_dec_results = compute_stats(config_info);
parm_names = fields(lin_dec_results.avg);
amax2 = [10, 11, 12, 15, 15, 15, 17, 17, 17, 20,...
    20, 20, 25, 25, 25, 7, 8, 9];
avg2 = [];
for idxParm = 1:numel(parm_names)
    avg2 = [avg2 lin_dec_results.avg.(parm_names{idxParm})];
end

%% Streamlined
config_info = struct();
config_info.direct = '4-11-streamlined/';
streamlined_results = compute_stats(config_info);
parm_names = fields(streamlined_results.avg);
amax3 = [10, 10, 10, 11, 11, 11, 12, 12, 12, 15, 15, 15, 20,...
    20, 20, 25, 25, 25, 7, 7, 7, 8, 8, 8, 9, 9, 9];
avg3 = [];
for idxParm = 1:numel(parm_names)
    avg3 = [avg3 streamlined_results.avg.(parm_names{idxParm})];
end

%% Linearly decreasing amplitude
config_info = struct();
config_info.direct = '4-11-streamlined2/';
streamlined2_results = compute_stats(config_info);
parm_names = fields(streamlined2_results.avg);
amax4 = [10, 10, 10, 11, 11, 11, 12, 12, 12, 15, 15, 15, 20,...
    20, 20, 25, 25, 25, 7, 7, 7, 8, 8, 8, 9, 9, 9];
avg4 = [];
for idxParm = 1:numel(parm_names)
    avg4 = [avg4 streamlined2_results.avg.(parm_names{idxParm})];
end
%% Analyze Results
% Fit polynomial to results
p1 = polyfit(amax1, avg1, 2);
p2 = polyfit(amax2, avg2, 2);
p3 = polyfit(amax3, avg3, 2);
p4 = polyfit(amax4, avg4, 2);

x = linspace(7, 25);
y1 = polyval(p1, x);
y2 = polyval(p2, x);
y3 = polyval(p3, x);
y4 = polyval(p4, x);

% Plot results
figure, hold on
plot(amax1, avg1, 'r.')
plot(amax2, avg2, 'm.')
plot(amax3, avg3, 'g.')
plot(amax4, avg4, 'bl.')
plot(x, y1, 'r')
plot(x, y2, 'm')
plot(x, y3, 'g')
plot(x, y4, 'bl')
legend('Linearly Increasing', 'Linearly Decreasing', 'Streamlined1', 'Streamlined2', 'location', 'best')
xlabel('Max Kinematic Amplitude (degrees)')
ylabel('Voltage Amplitude')
title('Various Amplitude Envelope Force Results')
