function  Day7()
%Part 1
%===================
cArr_inputRawData = fileread('Day7_input.txt');

%Parse data
str_bagsUnique = unique(cellfun(@char, regexp(cArr_inputRawData, '(?:^|\n|(\d ))([a-z ])+ bags?', 'tokens'), 'UniformOutput', false));
int_bagsUniqueCount = size(str_bagsUnique, 2);
cArr_regExp_parse =regexp(cArr_inputRawData, '([a-z ]+) bags contain ((\d+) ([a-z ]+) bags?, )*(\d+) ([a-z ]+) bags?\.', 'tokens');

%build Bags Relations Matrix
int_bagsRelationsMatrix = zeros(int_bagsUniqueCount, int_bagsUniqueCount);
for i_rowId = 1:size(cArr_regExp_parse, 2)

   int_containerId = find(strcmp(str_bagsUnique, cArr_regExp_parse{i_rowId}{1}), 1);
    
   str_regExp_match_Mid = cArr_regExp_parse{i_rowId}{2};
   if ~isempty(str_regExp_match_Mid)
       cArr_regExp_parseMid = regexp(str_regExp_match_Mid, '(\d+) ([a-z ]+) bags?,', 'tokens');
       for i_reMid = 1:size(cArr_regExp_parseMid, 2)
            int_currentBagId = find(strcmp(str_bagsUnique, cArr_regExp_parseMid{i_reMid}{2}), 1);
            int_bagsRelationsMatrix(int_containerId, int_currentBagId) = int_bagsRelationsMatrix(int_containerId, int_currentBagId) + str2double(cArr_regExp_parseMid{i_reMid}{1});
       end
   end
   
    int_currentBagId = find(strcmp(str_bagsUnique, cArr_regExp_parse{i_rowId}{end}), 1);
    int_bagsRelationsMatrix(int_containerId, int_currentBagId) = int_bagsRelationsMatrix(int_containerId, int_currentBagId) + str2double(cArr_regExp_parse{i_rowId}{end-1});
end

%Count bags color
    function [bool_colCanContainSpec, int_alreadySolvedColors] = get_checkContFeature(int_relationsMatrix, int_colorId, bool_colCanContainSpec, int_alreadySolvedColors)
        if nargin <  3
            bool_colCanContainSpec = false(size(int_relationsMatrix, 2), 1);
            int_alreadySolvedColors = [];
        end
        
        bool_colCanContainSpec_loc = int_relationsMatrix(:, int_colorId) ~= 0;
        bool_colCanContainSpec = bool_colCanContainSpec | bool_colCanContainSpec_loc;
        int_alreadySolvedColors = [int_alreadySolvedColors, int_colorId];
        
        int_colCanContainSpecIds_loc = find(bool_colCanContainSpec_loc);
        int_colIds2Solve = setdiff(int_colCanContainSpecIds_loc, int_alreadySolvedColors);
        if ~isempty(int_colIds2Solve)
            for i_col = 1:size(int_colIds2Solve, 1)
                [bool_colCanContainSpec, int_alreadySolvedColors] = get_checkContFeature(int_relationsMatrix, int_colIds2Solve(i_col), bool_colCanContainSpec, int_alreadySolvedColors);
            end
        end
    end

int_colId_shiny_gold = find(strcmp(str_bagsUnique, 'shiny gold'), 1);
int_out_part1 = sum(get_checkContFeature(int_bagsRelationsMatrix, int_colId_shiny_gold));
%Part 2
%===================
    %objects names are limited to 63 char ....
    function int_reqBagCount = getFuckingAnswerAfterSpentTimeToDebugPermutedColRowIndexesPartI(int_relationsMatrix, int_colorId)

         int_reqBagCount = 0;
        
         bool_bagContains_loc = int_relationsMatrix(int_colorId, :) ~= 0;
         int_bagContains_locIdx = find(bool_bagContains_loc);
         for i_col = 1:size(int_bagContains_locIdx, 2)
             int_reqBagCount = int_reqBagCount + int_relationsMatrix(int_colorId, int_bagContains_locIdx(i_col)) * ...
                (getFuckingAnswerAfterSpentTimeToDebugPermutedColRowIndexesPartI(int_relationsMatrix, int_bagContains_locIdx(i_col)) + 1);
         end
        
    end

int_out_part2 =  getFuckingAnswerAfterSpentTimeToDebugPermutedColRowIndexesPartI(int_bagsRelationsMatrix, int_colId_shiny_gold);

%Print results
%===================
sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end