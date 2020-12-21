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
/*定义宏来模拟指令的解码*/
/*IR为指令寄存器*/
#define REG0 ((IR>>24)&0x07)
#define REG1 ((IR>>20)&0x0f)
#define REG2 ((IR>>16)&0x0f)
#define IMMEDIATE (IR&0xffff)
#define ADDRESS (IR&0xffffff)
#define PORT (IR&0xff)
#define OPCODE ((IR>>27)&0x1f)
/*状态字信息*/
typedef struct state{
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
    list<sceneOfProgramInterrupt> interruptScenceList;  /*记录程序调用子函数时的状态*/
public:
    int HLT(void);
    int JMP(void);
    int CJMP(void);
    int OJMP(void);
    int CALL(void);
    int RET(void);
    int PUSH(void);
    int POP(void);
    int LOADB(void);
    int LOADW(void);
    int STOREB(void);
    int STOREW(void);
    int LOADI(void);
    int NOP(void);
    int IN(void);
    int OUT(void);
    int ADD(void);
    int ADDI(void);
    int SUB(void);
    int SUBI(void);
    int MUL(void);
    int DIV(void);
    int AND(void);
    int OR(void);
    int NOR(void);
    int NOTB(void);
    int SAL(void);
    int SAR(void);
    int EQU(void);
    int LT(void);
    int LTE(void);
    int NOTC(void);
    void simulateComputer(vector<string> binaryCode);
};

#endif //SIMULATECOMPUTERVERSIONONE_SIMULATECOMPUTER_H
