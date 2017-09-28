function [peaks, locs, half] = FindFirstPeak(ecg) 
    peaks = [];
    ecg = ecg - mean(ecg);
    [pks, locs] = findpeaks(ecg, 'MINPEAKHEIGHT', 2); 
    half = 0;
    for i = 1:length(locs) - 1
        half = half + (locs(i+1) - locs(i))/2;
    end
    half = int64(half /(length(locs)-1));
    if locs(1) > half
        [p1, i1] = findpeaks(ecg(1:locs(1)));
        [m, i2] = max(p1);
        idx = i1(i2);
        peaks = [peaks, idx];
    end
end