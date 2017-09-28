%method will return the trial # and the onset of the freezing time for that trial 
function [trials, freezingTime] = DetectFreezing(mc) 
    trials = []; 
    peaks = [];
    freezingTime = [];
	for i = 1:30 %loop through 30 trials 
        timeOfPeak = [];
        pitchAngle = mc.getPitchAngle(i); %determining freezing using pitchAngles data
        time = (1:length(pitchAngle)) / mc.frequency; 
		%'MinPeakProminence' finds peaks guaranteed to have a vertical drop of more than MPP 
		%from the peak on both sides without encountering either the end of the signal or a larger
		%intervening peak. The default value of MPP is zero.
		%get all the 'main' peaks of the pitch angles, 'MinPeakProminence' will filter out 
		%the smaller peaks that have a vertical drop less than 0.008. This was found by analyzing 
		%individual data points of trials where many small peaks were found. 
		[peaks, locs] = findpeaks(getPitchAngle(mc, i),'MinPeakProminence', 0.008); 
        for j = 1:length(locs) %converting locations to time
            timeOfPeak = [timeOfPeak, time(locs(j))];
        end
        
        %case where there is a break caused by NaN
        %detect the max pitch angle of the trial, and if the max pitch angle is not in the peaks
        %array, add it into the array 
        %adds trial 15
        M = max(pitchAngle);
        found = ismember(M, peaks);

        if found == 0
        	peaks = [peaks; M]; %append the max value to peaks 
    	end

       %freezing gaits appear to peak at a greater frequency within the period of time
       %analyzed the freezing plots and found the number of 'main' peaks to be greater or equal to 3
        if (length(peaks) >= 3)
            trials = [trials, i];
            freezingTime = [freezingTime, timeOfPeak(2)]; %take the second peak as the onset 
        end
    end
end
	
		
			


