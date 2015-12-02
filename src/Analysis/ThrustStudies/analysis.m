%%
clear, clc
config_info = struct();
config_info.direct = 'calibration_data/';
config_info.results = 'calibration_post.csv';
results = compute_stats(config_info)

%% Experiment 1
config_info.direct = 'experiment1/';
config_info.results = 'experiment1_post.csv';
results = compute_stats(config_info)

%% Experiment 2
config_info.direct ='experiment2/';
config_info.results = 'experiment2_post.csv';
resultsA = compute_stats(config_info)


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
