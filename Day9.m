function  Day9()
%Part 1
%===================
int_inputData = cellfun(@(x)uint32(str2double(x)), regexp(fileread('Day9_input.txt'), '\r?\n', 'split'));
int_BufferSize = 25;

bool_CheckRule = true;
i_Idx2Check = int_BufferSize+1;
while bool_CheckRule  && i_Idx2Check<(size(int_inputData, 2)+1)
    int_BufferIdx = (i_Idx2Check-int_BufferSize:i_Idx2Check-1);
    int_bufferCoupleSum = int_inputData(int_BufferIdx) + int_inputData(int_BufferIdx)';
    bool_CheckRule = any(int_bufferCoupleSum(1:end)==int_inputData(i_Idx2Check));
    i_Idx2Check = i_Idx2Check+1;
end

if  i_Idx2Check==size(int_inputData, 2)+1
    int_out_part1 = 0;
else
    int_out_part1 = int_inputData(i_Idx2Check-1);
end
    
%Part 2
%===================
int_buffSum = int_inputData;
int_bufferSize = 0;
int_sumEqInvalidNum_Idx = [];

while isempty(int_sumEqInvalidNum_Idx) && int_bufferSize < size(int_inputData, 2)+1
    int_bufferSize = int_bufferSize+1;
    int_buffSum = int_buffSum + [NaN(1,int_bufferSize), int_inputData(1:end-int_bufferSize)];
    int_sumEqInvalidNum_Idx = find(int_buffSum==int_out_part1, 1);
end

int_buffValues = int_inputData(int_sumEqInvalidNum_Idx-int_bufferSize:int_sumEqInvalidNum_Idx);
int_out_part2 = min(int_buffValues) + max(int_buffValues);

%Print results
%===================
sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end