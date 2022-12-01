function  Day5()
%Part 1
%===================
%Read textfile
cArr_inputRawData = regexp(fileread('Day5_input.txt'), '\r?\n', 'split');

    function int_seatId = getSeatId(str_bnrSpcPart, int_planeRowCount, int_planeColCount)
        i_char = 1;
        
        %Row Id
        int_seatRowId = int_planeRowCount-1;
        int_blkSize = int_planeRowCount/2;
        while (str_bnrSpcPart(i_char)=='F' || str_bnrSpcPart(i_char)=='B')
            if str_bnrSpcPart(i_char)=='F'
                int_seatRowId = int_seatRowId - int_blkSize;
            end
            
            int_blkSize = int_blkSize/2;
            i_char = i_char + 1;
        end
        int_colSeqStartId = i_char;
        
        %Col Id
        int_seatColId = int_planeColCount-1;
        int_blkSize = int_planeColCount/2;
        for i_char = int_colSeqStartId:size(str_bnrSpcPart, 2)
            if str_bnrSpcPart(i_char)=='L'
                int_seatColId = int_seatColId - int_blkSize;
            end
            
            int_blkSize = int_blkSize/2;
        end

        int_seatId = int_seatRowId*int_planeColCount + int_seatColId;
    end

int_seatsIds = cellfun(@(x)getSeatId(x, 128, 8), cArr_inputRawData);
int_out_part1 = max(int_seatsIds);

%Part 2
%===================
int_allSeatsIds = min(int_seatsIds):int_out_part1;
int_out_part2 = setdiff(int_allSeatsIds, int_seatsIds); %setdiff(A, B) returns elements in A that is not in B

%Print results
%===================
sprintf('Part_1 answer : %u\n Part_2 answer : %u\n', int_out_part1, int_out_part2)
end