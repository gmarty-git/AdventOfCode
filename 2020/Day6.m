function  Day6()
%Part 1
%===================
%Read textfile
cArr_inputRawData = regexp(fileread('Day6_input.txt'), '\r?\n\r?\n', 'split');
int_out_part1 = sum(cellfun(@(x)size(unique(regexprep(x, '\r?\n', '')), 2), cArr_inputRawData), 2);

%Part 2
%===================
    function int_commonAnswCount = getCommonAnswersCount(str_GroupAnswer)
        int_rowsCount = size(str_GroupAnswer(str_GroupAnswer==10), 2)+1;
        str_GroupAnswer = regexprep(str_GroupAnswer, '\r?\n', '');
        [str_UniqueAnswers, ~, int_GroupAnswersIdx] = unique(str_GroupAnswer);
        int_occurencesCount = histc(int_GroupAnswersIdx, 1:numel(str_UniqueAnswers));
        int_commonAnswCount = sum(int_occurencesCount==int_rowsCount, 1);
    end

int_out_part2 =  sum(cellfun(@(x)getCommonAnswersCount(x), cArr_inputRawData), 2);

%Print results
%===================
sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end