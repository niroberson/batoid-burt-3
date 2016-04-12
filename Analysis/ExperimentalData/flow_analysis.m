clear, clc
%% Linearly increasing amplitude
config_info = struct();
config_info.direct = '4-11-lin-inc/';
lin_inc_results = compute_stats(config_info);
lin_inc_parameters

%% Linearly decreasing amplitude
config_info = struct();
config_info.direct = '4-11-lin-dec/';
lin_dec_results = compute_stats(config_info);

%% Streamlined
config_info = struct();
config_info.direct = '4-11-streamlined/';
streamlined_results = compute_stats(config_info);

%% Linearly decreasing amplitude
config_info = struct();
config_info.direct = '4-11-streamlined2/';
streamlined2_results = compute_stats(config_info);

%% Analyze Results

