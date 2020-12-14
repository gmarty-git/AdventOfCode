function  Day12()
tic;
%Part 1 :
%===================
int_inputData = fileread('Day12_input.txt');
str_cmdId = cell2mat(regexp(int_inputData, '[NSEWLRF]', 'match'));
int_cmdValue = cellfun(@str2double, regexp(int_inputData, '\d+', 'match'));

int_allowedDirections = [0, 1 ; 1, 0; 0, -1; -1, 0]; %NESW
int_currentDirectionIdx = 2; %East
int_currentPosition = [0, 0];
for i_cmd = 1:numel(str_cmdId);
    if  str_cmdId(i_cmd) == 'N'
        int_currentPosition(2) = int_currentPosition(2) + int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'S'
        int_currentPosition(2) = int_currentPosition(2) - int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'E'
        int_currentPosition(1) = int_currentPosition(1) + int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'W'
        int_currentPosition(1) = int_currentPosition(1) - int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'R'
        int_currentDirectionIdx = mod(int_currentDirectionIdx - 1 + int_cmdValue(i_cmd)/90, 4) + 1;
    elseif str_cmdId(i_cmd) == 'L'
        int_currentDirectionIdx = mod(int_currentDirectionIdx - 1 - int_cmdValue(i_cmd)/90, 4) + 1;
    elseif str_cmdId(i_cmd) == 'F'
        int_currentPosition = int_currentPosition + int_cmdValue(i_cmd)*int_allowedDirections(int_currentDirectionIdx, :);
    else
        error('[ERROR] - Unkwown cmdId : %s', str_cmdId(i_cmd));
    end
end

int_out_part1 = sum(abs(int_currentPosition));


%Part 2 :
%===================
int_currentDirection = [10, 1]; %10E3N
int_currentPosition = [0, 0];
for i_cmd = 1:numel(str_cmdId)
    if  str_cmdId(i_cmd) == 'N'
        int_currentDirection(2) = int_currentDirection(2) + int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'S'
        int_currentDirection(2) = int_currentDirection(2) - int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'E'
        int_currentDirection(1) = int_currentDirection(1) + int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'W'
        int_currentDirection(1) = int_currentDirection(1) - int_cmdValue(i_cmd);
    elseif str_cmdId(i_cmd) == 'R'
        int_90DegRotCount = mod(int_cmdValue(i_cmd)/90, 4);
        if int_90DegRotCount == 1
            int_currentDirection = [int_currentDirection(2), -int_currentDirection(1)];
        elseif int_90DegRotCount == 2
            int_currentDirection = -int_currentDirection;
        elseif int_90DegRotCount == 3
            int_currentDirection = [-int_currentDirection(2), int_currentDirection(1)];
        end
    elseif str_cmdId(i_cmd) == 'L'
        int_90DegRotCount = mod(int_cmdValue(i_cmd)/90, 4);
        if int_90DegRotCount == 1
            int_currentDirection = [-int_currentDirection(2), int_currentDirection(1)];
        elseif int_90DegRotCount == 2
            int_currentDirection = -int_currentDirection;
        elseif int_90DegRotCount == 3
            int_currentDirection = [int_currentDirection(2), -int_currentDirection(1)];
        end
    elseif str_cmdId(i_cmd) == 'F'
        int_currentPosition = int_currentPosition + int_cmdValue(i_cmd)*int_currentDirection;
    else
        error('[ERROR] - Unkwown cmdId : %s', str_cmdId(i_cmd));
    end
end

int_out_part2 = sum(abs(int_currentPosition));

%Print results
%===================
sprintf('Part_1 answer : %.0f\n Part_2 answer : %.0f\n', int_out_part1, int_out_part2)
toc;
end