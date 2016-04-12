function results = compute_stats(config_info)
base = [config_info.direct '*.tdms'];
data_files = dir(base);

results = struct();
avg_v = struct();
max_v = struct();
min_v = struct();

for idxFile = 1:numel(data_files)
        % Import data
        fileName = data_files(idxFile).name;
        dat = TDMS_readTDMSFile([config_info.direct fileName]);
        [name, ~] = strsplit(fileName, '.');
        h1 = figure;
        filtered = sgolayfilt(dat.data{2},1,5);
        plot(filtered)
        x = ginput(2);
        close(h1);
        
        % Extract voltage vector
        v_steady = filtered(ceil(x(1)):floor(x(2)));
        
        v_avg = mean(v_steady);
        avg_v.(name{1}) = v_avg;
        max_v.(name{1}) = compute_avg_max(@max, v_steady, 0.01);
        min_v.(name{1}) = compute_avg_min(@min, v_steady, 0.01);
end

results.avg = avg_v;
results.max_force = max_v;
results.min_force = min_v;
end


function y_avg_f = compute_avg_max(f, y, p)
    yf = f(y);
    yselect = y(y > yf * p);
    y_avg_f = mean(yselect);
end

function y_avg_f = compute_avg_min(f, y, p)
    yf = f(y);
    yselect = y(y < yf * p);
    y_avg_f = mean(yselect);
end