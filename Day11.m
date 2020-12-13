function  Day11()
tic;
%Part 1 :
%===================
int_inputData = regexp(fileread('Day11_input.txt'), '\r?\n', 'split');

    function bool_occSeatsMap = runSimulation(int_inputData, str_part2solve, bool_occSeatsMap, bool_floorMap, int_rowsCount, int_colsCount, int_interiorMapIdx)
        %First call init
        if nargin < 3
            %Init space domain (+ add boundaries set as floor)
            int_rowsCount = size(int_inputData, 2);
            int_colsCount =  size(int_inputData{1}, 2);
            bool_floorMap = true(int_rowsCount+2, int_colsCount+2);
            bool_floorMap([false, true(1, int_rowsCount), false], [false, true(1, int_colsCount), false]) = vertcat(int_inputData{:})=='.';
            bool_occSeatsMap = false(int_rowsCount+2, int_colsCount+2);
            
            %Build 1D indexes related to interior domain (without floor
            %border)
            int_rowsMapIdx = (2:int_rowsCount+1)'*ones(1, int_colsCount);
            int_colsMapIdx = ones(1, int_rowsCount)'*(2:int_colsCount+1);
            int_interiorMapIdx = (int_colsMapIdx-1)*(int_rowsCount+2) + int_rowsMapIdx;
            int_interiorMapIdx = int_interiorMapIdx(:);
        end
        
        %Sum neigbour occupied seats
        int_occSeatsSumMap = zeros(size(bool_occSeatsMap));
        if strcmp(str_part2solve, 'part1')
            int_maxOccupiedSeatsInNeigh = 3;
            
            for i_neighInc = [(-int_rowsCount-3):(-int_rowsCount-1), -1, +1, (+int_rowsCount+1):(+int_rowsCount+3)]
                int_occSeatsSumMap(int_interiorMapIdx) = int_occSeatsSumMap(int_interiorMapIdx) + bool_occSeatsMap(int_interiorMapIdx+i_neighInc);
            end
        elseif strcmp(str_part2solve, 'part2')
            int_maxOccupiedSeatsInNeigh = 4;
            
            int_horzDirection = [-1, -1, -1, 0, 0, +1, +1, +1];
            int_vertDirection = [-1, 0, +1, -1, +1, -1, 0, +1];
            for i_col = 2:int_colsCount+1
                for i_row = 2:int_rowsCount+1
                    
                    for i_dir = 1:size(int_horzDirection, 2)
                        int_rowId = i_row + int_vertDirection(i_dir);
                        int_colId = i_col + int_horzDirection(i_dir);
                        while int_rowId>0 && int_rowId<=int_rowsCount+2 && int_colId>0 && int_colId<=int_colsCount+2
                            if bool_occSeatsMap(int_rowId, int_colId)
                                int_occSeatsSumMap(i_row, i_col) = int_occSeatsSumMap(i_row, i_col)+1;
                                break
                            elseif ~bool_floorMap(int_rowId, int_colId)
                                break %Empty seats hide seats behind
                            end
                            int_rowId = int_rowId + int_vertDirection(i_dir);
                            int_colId = int_colId + int_horzDirection(i_dir);                          
                        end
                    end                 
                end
            end
            
        else
            error('[ERROR] - runSimulation function : unknown use case "%s".', str_part2solve);
        end
        
        %Solve seats occupation rules
        bool_setOccupiedMap = ~bool_occSeatsMap & (int_occSeatsSumMap==0) & ~bool_floorMap;
        bool_setEmptyMap = bool_occSeatsMap & (int_occSeatsSumMap>int_maxOccupiedSeatsInNeigh);
        
        %Update domain values
        if any(bool_setOccupiedMap(:)) || any(bool_setEmptyMap(:))
            bool_occSeatsMap(bool_setOccupiedMap) = true;
            bool_occSeatsMap(bool_setEmptyMap) = false;
            bool_occSeatsMap = runSimulation(int_inputData, str_part2solve, bool_occSeatsMap, bool_floorMap, int_rowsCount, int_colsCount, int_interiorMapIdx);
        end
    end

bool_occSeatsMap = runSimulation(int_inputData, 'part1');
int_out_part1 = sum(bool_occSeatsMap(:));


%Part 2 :
%===================
bool_occSeatsMap = runSimulation(int_inputData, 'part2');
int_out_part2 = sum(bool_occSeatsMap(:));


%Print results
%===================
sprintf('Part_1 answer : %.0f\n Part_2 answer : %.0f\n', int_out_part1, int_out_part2)
toc;
end