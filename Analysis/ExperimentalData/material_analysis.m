clear, clc
%% Silicon Fin
config_info = struct();
config_info.direct = 'silicon-4-7/';
silicon_results = compute_stats(config_info);

%% Lycra Fin
config_info = struct();
config_info.direct = 'lycra-4-7/';
lycra_results = compute_stats(config_info);

%% Analyze Results