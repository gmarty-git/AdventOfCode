#include "class_MonkeyGang.hpp"

Operation::Operation() {
    setOperation("", "", "");
};

Operation* Operation::setOperation(std::string arg1, std::string arg2, std::string operand){
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->operand = operand;
    return this;
}

unsigned long long Operation::eval(unsigned long long old){
    unsigned long long a,b;
    if (arg1 == "old")
        a = old;
    else
        a = std::stoi(arg1);
    
    if (arg2 == "old")
        b = old;
    else
        b = std::stoi(arg2);

    if (operand == "+")
        return a+b;
    else if(operand == "*")
        return a*b;
    else{
        std::cout << "[ERROR] - Operation::eval : unknown operand '" <<  operand << "'. Aborting execution ..." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::ostream& operator<< (std::ostream &os, Operation &operation){
    os << "new = " << operation.arg1 << " " << operation.operand << " " << operation.arg2 ;
    return os;
}

Monkey::Monkey(std::ifstream &ifs) {
    std::string str_buffer, str_arg1, str_arg2, str_op;
    int int_buffer;

    std::getline(ifs, str_buffer);                                              //Monkey id
    ifs >> str_buffer >> str_buffer;                                            //Monkey items
    do {
        ifs >> int_buffer >> str_buffer;
        items.push(int_buffer);
    } while( str_buffer == "," );
    ifs >> str_buffer >> str_buffer;                                            //Monkey Operation
    ifs >> str_arg1 >> str_op >> str_arg2;
    op.setOperation(str_arg1, str_arg2, str_op);
    ifs >> str_buffer >> str_buffer >> str_buffer;                              //Monkey Test divisor    
    ifs >> testDivisor;
    ifs >> str_buffer >> str_buffer >> str_buffer >> str_buffer >> str_buffer;  //Monkey taget if Test True
    ifs >> targetTrue_Id;
    ifs >> str_buffer >> str_buffer >> str_buffer >> str_buffer >> str_buffer;  //Monkey taget if Test False
    ifs >> targetFalse_Id;
    std::getline(ifs, str_buffer);                                              //end line
    std::getline(ifs, str_buffer);                                              //skip line

    inspectedItemsCount = 0UL;
}

Monkey* Monkey::resolveTurn(bool manageableWorryLevel, int testsDivisorLCM) {
    unsigned long long currentItem;
    while (!items.empty()){
        inspectedItemsCount++;
        currentItem = items.front();
        items.pop();

        currentItem = op.eval(currentItem); 

        if (testsDivisorLCM > 0)     
            currentItem = currentItem%testsDivisorLCM; //Overflow manager :)

        if (manageableWorryLevel)
            currentItem /= 3;

        if (currentItem%testDivisor == 0)         
            targetTrue->items.push(currentItem);
        
        else 
            targetFalse->items.push(currentItem);

    }   
}

Monkey* Monkey::setTargets(Monkey* targetTrue, Monkey* targetFalse){
    this->targetTrue = targetTrue;
    this->targetFalse = targetFalse;

    return this;
}

std::ostream& operator<< (std::ostream &os, Monkey &monkey){
    os << std::endl << "    Items :";
    std::queue<unsigned long long> itemsCopy(monkey.items) ;
     while (!itemsCopy.empty()) {
        os <<  " " << itemsCopy.front();
        itemsCopy.pop();
    }
    os << std::endl << "    Operarion : " <<  monkey.op << std::endl;
    os << "    Test divisor = " << monkey.testDivisor << std::endl;
    os << "    Targets : ifTrue-> " << monkey.targetTrue_Id << " -- ifFalse-> " <<  monkey.targetFalse_Id << std::endl;
    
    return os;
}

MonkeyGang::MonkeyGang(std::ifstream &ifs) {
    while (ifs)
        monkeys.push_back(Monkey(ifs));

    updateTargets();

    //Store Monkeys tests divisor LCM to avoid overflow #klqsfjkdsfbsdfj  stupids monkeys !!!!!
    testsDivisorLCM = 1;
    for (auto it = monkeys.begin() ; it != monkeys.end() ; ++it)
        testsDivisorLCM = std::lcm(testsDivisorLCM, it->testDivisor);
}

MonkeyGang* MonkeyGang::updateTargets() {
    for (auto it = monkeys.begin() ; it != monkeys.end() ; ++it)
        it->setTargets( &monkeys[it->targetTrue_Id] , &monkeys[it->targetFalse_Id] );

    return this;
}

MonkeyGang* MonkeyGang::resolveRounds(int roundsCount, bool manageableWorryLevel){
    for (int i = 0 ; i<roundsCount ; ++i){
        for (auto it = monkeys.begin() ; it != monkeys.end() ; ++it)
            it->resolveTurn(manageableWorryLevel, testsDivisorLCM);
    }
}

unsigned long long MonkeyGang::getMonkeyBusinessLevel(){

    if (monkeys.size() < 2 )
        return 0;

    std::vector<unsigned long long> inspectedItemsCountVect(monkeys.size());
    for (int i = 0 ; i < monkeys.size() ; ++i)
        inspectedItemsCountVect[i] = monkeys[i].inspectedItemsCount;

    std::sort(inspectedItemsCountVect.rbegin(), inspectedItemsCountVect.rend());
    return inspectedItemsCountVect[0] * inspectedItemsCountVect[1];
}

std::ostream& operator<< (std::ostream &os, MonkeyGang &monkeyGang){
    os << " -----  MONKEY GANG   ----- " << std::endl << std::endl;
    int currentMonkeyId = 0;
    for (auto it = monkeyGang.monkeys.begin() ; it != monkeyGang.monkeys.end() ; ++it) {
        os << "Monkey #" << currentMonkeyId;
        currentMonkeyId++;
        os << *it << std::endl;
    }
    os << " -----  MONKEY GANG   ----- " << std::endl;

    return os;
}

MonkeyGang* MonkeyGang::dispInspectedItemsCount (std::ostream &os) {
    for (int i = 0 ; i < monkeys.size() ; ++i) 
        os << "    Monkey " <<  i << " inspected items " << monkeys[i].inspectedItemsCount << " times." << std::endl;

    return this;
}
