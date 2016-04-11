clear, clc
%% Calibration
config_info = struct();
config_info.direct = 'cal-4-4/';
resultsCal = compute_stats(config_info);

%% Experiment 1
config_info.direct = 'experiment1/';
results1 = compute_stats(config_info);

%% Experiment 2
config_info.direct ='experiment2/';
results2 = compute_stats(config_info);


%% Plot summary results of experiments
% First get the function to convert voltage to force
mass = [20, 39.9, 59.8, 79.7];
F = mass*9.81/1000;
calNames = fields(resultsCal.avg);
vcal = [];
for idxCal = 1:numel(calNames)
    vcal = [vcal resultsCal.avg.(calNames{idxCal})];
end

% Linear fit the data
P = polyfit(vcal, F, 1);
Ffit = P(1)*vcal + P(2);
Rsq = 1 - sum((vcal - Ffit).^2)/sum((vcal - mean(vcal)).^2);

figure, hold on
plot(vcal, F, '.', 'markersize', 30)
plot(vcal, Ffit, 'r-.')
xlabel('Voltage')
ylabel('Force')

% State conversion function
Fn = @(V) V*P(1) + P(2);

% Get frequency results
names1 = fields(results1.avg);
freq = [10, 12, 4, 6, 8];
avg1 = [];
max1 = [];
min1 = [];
for idx1=1:numel(names1)
    iName = names1{idx1};
    avg1 = [avg1 results1.avg.(iName)];
    max1 = [max1 results1.max_force.(iName)];
    min1 = [min1 results1.min_force.(iName)];
end


% Get amplitude results
names2 = fields(results2.avg);
amp = [12, 3, 6, 9];
avg2 = [];
max2 = [];
min2 = [];
for idx2=1:numel(names2)
    iName = names2{idx2};
    avg2 = [avg2 results2.avg.(iName)];
    max2 = [max2 results2.max_force.(iName)];
    min2 = [min2 results2.min_force.(iName)];
end

figure, hold on
freqD = [freq; Fn(avg1)]';
freqD = sortrows(freqD);
plot(freqD(:,1), freqD(:,2), '.-', 'markersize', 20)
ampD = [amp; Fn(avg2)]';
ampD = sortrows(ampD);
plot(ampD(:,1), ampD(:,2), '.-', 'markersize', 20)
xlim([0 13])
xlabel('Wave Parameter')
ylabel('Average Force [N]')
legend('Frequency [rad/s]', 'Amplitude [degrees]', 'location', 'best')
title('Effect of Wave Parameter on Time Average Force')
grid

figure, hold on
plot(freq,Fn(max1), '.', 'markersize', 20)
plot(amp, Fn(max2), '.', 'markersize', 20)
xlabel('Wave Parameter')
ylabel('Top 0.1% Force [N]')
legend('Frequency [rad/s]', 'Amplitude [degrees]', 'location', 'best')
grid
%% Plot data from a trial
t = 0.001:0.001:numel(dat.data{1})/1000;
figure,plot(t(ceil(x):end),dat.data{1}(ceil(x):end)*4.89999+0.0193)
xlabel('t [s]')
ylabel('F_x [N]')
title('Force over Time w = 8 rad/s Amax = 9 degrees')

%% Get individual time plots
a = 'experiment2/A9.tdms';
w = 'experiment1/w6.tdms';

A  = TDMS_readTDMSFile(a);
W = TDMS_readTDMSFile(w);

A_int = A.data{1}(25000:28000);
W_int = W.data{1}(60000:63000);
t = 50:0.001:53;

figure
A_int_smooth = smooth(A_int);
plot(t, A_int_smooth*4.89999+0.0193)
xlabel('t [s]')
ylabel('F_x [N]')
title('Force over Time w = 8 rad/s Amax = 9 degrees')

figure
W_int_smooth = smooth(W_int);
plot(t, W_int_smooth*4.89999+0.0193)
xlabel('t [s]')
ylabel('F_x [N]')
title('Force over Time w = 6 rad/s Amax = 9 degrees')
