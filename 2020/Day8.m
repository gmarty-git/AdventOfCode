function  Day8()
%Part 1
%===================
cArr_inputRawData = fileread('Day8_input.txt');

%Parse data
cArr_regExp_parse =regexp(cArr_inputRawData, '([a-z]+) ((?:+|-)\d+)(?:(\r?\n)|$)', 'tokens');
str_cmdIds = cellfun(@(x)x{1}, cArr_regExp_parse, 'UniformOutput', false);
int_cmdIncrements = cellfun(@(x)str2double(x{2}), cArr_regExp_parse);

%run boot loop
    function [int_acc, bool_cmdExecutionTrace, bool_infLoopAbort] = run_bootSequence(str_cmdIds, int_cmdIncrements, int_currentCmdId, bool_cmdExecutionTrace, int_acc)
        if nargin < 3
            int_acc = 0;
            int_currentCmdId = 1;
            bool_cmdExecutionTrace = false(size(int_cmdIncrements));
        end
        
        bool_infLoopAbort = false;
        if bool_cmdExecutionTrace(int_currentCmdId)
            bool_infLoopAbort = true;
            return %Abnormal exit due to infinite loop
            
        else
            bool_cmdExecutionTrace(int_currentCmdId) = true;
            bool_isLastRow = int_currentCmdId==size(int_cmdIncrements, 2);
            str_cmdId = str_cmdIds{int_currentCmdId};
            int_cmdIncrement = int_cmdIncrements(int_currentCmdId);
                        
            if strcmp(str_cmdId, 'acc')
                int_acc = int_acc + int_cmdIncrement;
                int_currentCmdId = int_currentCmdId + 1;
            elseif strcmp(str_cmdId, 'jmp')
                int_currentCmdId = int_currentCmdId + int_cmdIncrement;
            elseif strcmp(str_cmdId, 'nop')
                int_currentCmdId = int_currentCmdId + 1;
            else
                error('[ERROR] - run_bootSequence function : unknown cmdId "%s".', str_cmdId);
            end

            if bool_isLastRow && (int_currentCmdId==size(int_cmdIncrements, 2) + 1)
                return %Normal boot sequence exit
            else
                [int_acc, bool_cmdExecutionTrace, bool_infLoopAbort] = run_bootSequence(str_cmdIds, int_cmdIncrements, int_currentCmdId, bool_cmdExecutionTrace, int_acc);
            end
        end
    end

int_out_part1 = run_bootSequence(str_cmdIds, int_cmdIncrements);

%Part 2
%===================
int_jmpnop_Instr_rowsIds = find(strcmp(str_cmdIds, 'jmp')|strcmp(str_cmdIds, 'nop'));
for i_bSeq = 1:size(int_jmpnop_Instr_rowsIds, 2)
    str_cmdIds_fixed = str_cmdIds;
    if strcmp(str_cmdIds_fixed{int_jmpnop_Instr_rowsIds(i_bSeq)}, 'jmp')
        str_cmdIds_fixed{int_jmpnop_Instr_rowsIds(i_bSeq)} = 'nop';
    else
        str_cmdIds_fixed{int_jmpnop_Instr_rowsIds(i_bSeq)} = 'jmp';
    end
    
    [int_out_part2, ~, bool_infLoopAbort] = run_bootSequence(str_cmdIds_fixed, int_cmdIncrements);
    if ~bool_infLoopAbort
        break
    end
end


%Print results
%===================
sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end