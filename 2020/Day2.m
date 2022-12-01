function  Day2()
    %Part 1
    %===================
    %Read textfile
    fileId = fopen('Day2_input.txt');
    cArr_inputRawData = textscan(fileId, '%u-%u %c: %s');
    fclose(fileId);
    
    %Parse input
    int_minRange = cArr_inputRawData{1};
    int_maxRange = cArr_inputRawData{2};
    int_char = cArr_inputRawData{3};
    int_pwd = char(cArr_inputRawData{4}); %cast password to integer array to easily perform character search
    int_maxPwdLength = size(int_pwd, 2);
    
    %Check passwords
    bool_currentLetterMatch = (int_pwd == repmat(int_char, 1, int_maxPwdLength)); %detect where letter is used
    int_usedLetterCount = sum(bool_currentLetterMatch, 2); %Count letter occurences
    int_out_part1 = sum((int_usedLetterCount>=int_minRange) & (int_usedLetterCount<=int_maxRange)); %Check condition on each pwd
    
    %Part 2
    %===================
    int_PwdCount = size(bool_currentLetterMatch, 1);
    int_TrueMatch_idx = uint32(1:int_PwdCount)'+ (int_minRange-1)*uint32(int_PwdCount); %Indexes related to first letter
    int_FalseMatch_idx = uint32(1:int_PwdCount)'+(int_maxRange-1)*uint32(int_PwdCount); %Indexes related to second letter
    int_out_part2 = sum(bool_currentLetterMatch(int_TrueMatch_idx')  ~= bool_currentLetterMatch(int_FalseMatch_idx')); %Check condition on each pwd
    
    %Print results
    %===================
    sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end