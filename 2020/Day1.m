function out = Day1(Input_expenseReport)
    %Part 1
    int_SumMat = Input_expenseReport+Input_expenseReport';
    int_ProdMat = Input_expenseReport*Input_expenseReport';
    int_out_part1 = int_ProdMat(int_SumMat==2020);
    
    %Part 2
	%bsxfun : Binary singleton expansion function
    int_SumMat = bsxfun(@plus, int_SumMat,reshape(Input_expenseReport,1,1,[]));
    int_ProdMat = bsxfun(@times, int_ProdMat,reshape(Input_expenseReport,1,1,[]));
    int_out_part2 = int_ProdMat(int_SumMat==2020);
    
    %Return result
    out = [int_out_part1(1);int_out_part2(1)];
end

