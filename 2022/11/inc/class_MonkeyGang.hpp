#ifndef CLASS_MONKEYS_GANG_YOM
#define CLASS_MONKEYS_GANG_YOM

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <numeric>

class Operation {
 private:
    std::string arg1, arg2, operand;

 public:
    Operation();
    Operation(std::string arg1, std::string arg2, std::string operand);
    Operation* setOperation(std::string arg1, std::string arg2, std::string operand);
    unsigned long long eval(unsigned long long old);
    friend std::ostream& operator<< (std::ostream &os, Operation &operation);
};

class Monkey {
 friend class MonkeyGang;

 private:
    std::queue<unsigned long long> items;
    Operation op;
    int testDivisor, targetTrue_Id, targetFalse_Id;
    unsigned long long inspectedItemsCount;
    Monkey *targetTrue, *targetFalse;

 public:
    Monkey(std::ifstream &ifs);
    Monkey* resolveTurn(bool manageableWorryLevel = true, int testsDivisorLCM = 0);
    Monkey* setTargets(Monkey* targetTrue, Monkey* targetFalse);
    friend std::ostream& operator<< (std::ostream &os, Monkey &monkey);
};

class MonkeyGang {
 private:
    std::vector<Monkey> monkeys;
    MonkeyGang* updateTargets();
    int testsDivisorLCM;    

 public:
    MonkeyGang(std::ifstream &ifs);
    MonkeyGang* resolveRounds(int roundsCount = 1, bool manageableWorryLevel = true);
    unsigned long long getMonkeyBusinessLevel();
    friend std::ostream& operator<< (std::ostream &os, MonkeyGang &monkeyGang);
    MonkeyGang* dispInspectedItemsCount (std::ostream &os);
};
#endif
