function  Day10()
%Part 1 : all devices are used and have unique joltage -> only one way to link them ...
%===================
int_inputData = cellfun(@(x)uint32(str2double(x)), regexp(fileread('Day10_input.txt'), '\r?\n', 'split'));
int_inputData = sort(int_inputData);
int_outDeviceJoltage = int_inputData(end) + 3;
int_inputData = [0, int_inputData, int_outDeviceJoltage];
int_JoltageDiff = diff(int_inputData);

int_out_part1 =  sum(int_JoltageDiff==1) * sum(int_JoltageDiff==3);

%Part 2 : recursive function, saving already solved path to process data
%only once
%===================
    function [int_totPermCount, int_permCountByJoltage] = get_permutationCount(int_inputData, int_permCountByJoltage, i_adapt)
        int_maxJoltDiff = 3;
        
        int_totPermCount = 0;
        if nargin < 2
            int_permCountByJoltage = zeros(size(int_inputData));
            int_permCountByJoltage(end) = 1;
            i_adapt = 1;
        end
        
        for i_deltaJ = 1:min(int_maxJoltDiff, abs(size(int_inputData,2) - i_adapt))
            if  int_inputData(i_adapt+i_deltaJ) <= int_inputData(i_adapt)+int_maxJoltDiff
                if int_permCountByJoltage(i_adapt+i_deltaJ) == 0
                    [int_subTreePermCount, int_permCountByJoltage] = get_permutationCount(int_inputData, int_permCountByJoltage, i_adapt+i_deltaJ);
                    int_permCountByJoltage(i_adapt+i_deltaJ) = int_subTreePermCount;
                end
                int_totPermCount =   int_totPermCount + int_permCountByJoltage(i_adapt+i_deltaJ);
            else
                break
            end
        end
    end

int_out_part2 = get_permutationCount(int_inputData);


%Print results
%===================
sprintf('Part_1 answer : %.0f\n Part_2 answer : %.0f\n', int_out_part1, int_out_part2)
end