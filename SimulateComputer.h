//
// Created by Alex on 2020/12/21.
//
#ifndef SIMULATECOMPUTERVERSIONONE_SIMULATECOMPUTER_H
#define SIMULATECOMPUTERVERSIONONE_SIMULATECOMPUTER_H
#include<stdio.h>
#include <stdlib.h>
#include<list>
#include <string>
#include<vector>
using namespace std;

/*状态字信息*/
struct state{
    unsigned short overflow:1;/*判定溢出1为溢出，0为正常*/
    unsigned short compare:1;/*比较大小，1为真，0为假*/
    unsigned short reserve:14;/*无实际意义*/
}programStateWord;
/*调用子函数时保存当前程序执行的状态*/
typedef struct scence{
    struct state statement; /*记录字段结构*/
    short reg[8]; /*记录寄存器值*/
    unsigned long programLineNumber; /*记录当前执行到的行数*/
}sceneOfProgramInterrupt;
class SimulateComputer{
private:
    typedef int (*ops)(void) ;
    ops opsPtr[32];

public:

    SimulateComputer();
    static int HLT(void);
    static int JMP(void);
    static int CJMP(void);
    static int OJMP(void);
    static int CALL(void);
    static int RET(void);
    static int PUSH(void);
    static int POP(void);
    static int LOADB(void);
    static int LOADW(void);
    static int STOREB(void);
    static int STOREW(void);
    static int LOADI(void);
    static int NOP(void);
    static int IN(void);
    static int OUT(void);
    static int ADD(void);
    static int ADDI(void);
    static int SUB(void);
    static int SUBI(void);
    static int MUL(void);
    static int DIV(void);
    static int AND(void);
    static int OR(void);
    static int NOR(void);
    static int NOTB(void);
    static int SAL(void);
    static int SAR(void);
    static int EQU(void);
    static int LT(void);
    static int LTE(void);
    static int NOTC(void);
    void simulateComputer(vector<string> binaryCode);
};

#endif //SIMULATECOMPUTERVERSIONONE_SIMULATECOMPUTER_H
