function  Day3()
    %Part 1
    %===================
    %Read textfile
    cArr_inputRawData = regexp(fileread('Day3_input.txt'), '\r?\n', 'split');
    bool_TreesMap = vertcat(cArr_inputRawData{:})=='#';
    
    %Build indexes related to positions for slop dx, dy
    function [int_1D_Idx, int_rowIdx, int_colIdx] = getPositionsIdx(bool_map, dx, dy)
        [int_mapLenY, int_mapLenX] = size(bool_map);
        int_rowIdx = (1:dy:int_mapLenY);
        int_colIdx = mod(dx*((1:length(int_rowIdx))-1), int_mapLenX) + 1;
        int_1D_Idx = (int_colIdx-1)*int_mapLenY + int_rowIdx;
    end
    [int_Slope31_Idx, ~, ~] = getPositionsIdx(bool_TreesMap, 3, 1);
    
    %Sum trees
    int_out_part1 = sum(bool_TreesMap(int_Slope31_Idx), 'all');
    
    %Part 2
    %===================
    [int_Slope11_Idx, ~, ~] = getPositionsIdx(bool_TreesMap, 1, 1);
    [int_Slope51_Idx, ~, ~] = getPositionsIdx(bool_TreesMap, 5, 1);
    [int_Slope71_Idx, ~, ~] = getPositionsIdx(bool_TreesMap, 7, 1);
    [int_Slope12_Idx, ~, ~] = getPositionsIdx(bool_TreesMap, 1, 2);
    
    int_out_part2 = int_out_part1 ...
        * sum(bool_TreesMap(int_Slope11_Idx), 'all') ...
        * sum(bool_TreesMap(int_Slope51_Idx), 'all') ...
        * sum(bool_TreesMap(int_Slope71_Idx), 'all') ...
        * sum(bool_TreesMap(int_Slope12_Idx), 'all');
    
    %Print results
    %===================
    sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end