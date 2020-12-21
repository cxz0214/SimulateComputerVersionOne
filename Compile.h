//
// Created by Alex on 2020/12/20.
//
#ifndef SIMULATECOMPUTERVERSIONONE_COMPILE_H
#define SIMULATECOMPUTERVERSIONONE_COMPILE_H
#include<list>
#include <vector>
#include <string>
using namespace std;
#define INS_COUNT 32
/*指令集助记符*/
#define INSTRUCTSET { "HLT", "JMP", "CJMP", "OJMP", "CALL", "RET","PUSH", "POP", "LOADB", \
                      "LOADW","STOREB", "STOREW","LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", \
                      "SUB","SUBI","MUL", "DIV", "AND", "OR", "NOR", "NOTB","SAL", "SAR", \
                      "EQU", "LT", "LTE", "NOTC"}
/*标号信息*/
typedef struct marknumber{
    char name[30];
    int lineNumber;
}markNumber;
/*变量信息*/
typedef struct variable{
    char name[30];
    int size ;
    int offset;
    short value[256];
    int type;
}variable;
class Compile {
private:
    list<markNumber> markInfoList;
    list<variable> variableInfoList;
public:
    Compile(){}
    int getCodeOfInstruct(const char* charOfOperator);
    /*将一条指令转化为对应的编码*/
    unsigned long transferInstructToCode(char* instructLine,int instructCode);
    int saveMarkNumberInfo(char* markNumberName,int lineNumber);
    /*存放变量信息*/
    int saveVariableInfo(char* instructLine,int offset,int size,int type);
    /*通过寄存器名获取寄存器编号*/
    int getNumberOfReg(char* instructLine,char* regName);
    /*由指令助记符获取对应的操作码*/
    int getOperatorCodeOfInstruct(const char* opString);
    /*编译*/
    vector<string> getResultOfCompile(vector<string> assembleCode);
};


#endif //SIMULATECOMPUTERVERSIONONE_COMPILE_H
