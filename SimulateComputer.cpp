//
// Created by Alex on 2020/12/21.
//
#include "SimulateComputer.h"
/*PC 指令计数器，用来存放下条指令的内存地址*/
unsigned long *pc,d[300],op[300],num[100],IR;

short generalReg[8];/*8个通用寄存器*/
int pos=0;/*记录程序运行的位置*/
int ss[100],x=0;
int SimulateComputer::HLT(void) {
    pos++;
    return 0;
}

int SimulateComputer::JMP(void) {
    pos=ADDRESS/4;
    pc=(unsigned long *)(op+ADDRESS/4);
    return 1;
}

int SimulateComputer::CJMP(void) {
    if(programStateWord.compare)
    {
        pos=ADDRESS/4;
        pc=(unsigned long *)(op+ADDRESS/4);
    }
    else {
        pos++;
    }
    return 1;
}

int SimulateComputer::OJMP(void) {
    if(programStateWord.overflow)
    {
        pos=ADDRESS/4;
        pc=(unsigned long *)(op+ADDRESS/4);
    }
    else {
        pos++;
    }

    return 1;
}

int SimulateComputer::CALL(void) {
    if(interruptScenceList.empty())
    {
        sceneOfProgramInterrupt * newScene = (sceneOfProgramInterrupt*)malloc(sizeof(sceneOfProgramInterrupt));
        newScene->programLineNumber = pos;
        for(int i = 0 ; i < 8;i++)
            newScene->reg[i] = generalReg[i];
        newScene->statement = programStateWord;
        interruptScenceList.push_back(*newScene);
    }
    else
    {
        sceneOfProgramInterrupt * newScene = (sceneOfProgramInterrupt*)malloc(sizeof(sceneOfProgramInterrupt));
        newScene->programLineNumber = pos;
        newScene->statement = programStateWord;
        for(int i = 0 ; i < 8; i ++)
            newScene->reg[i] = generalReg[i];
        interruptScenceList.push_back(*newScene);
    }
    pos=ADDRESS/4;
    pc=(unsigned long *)(op+ADDRESS/4);
    return 1;
}

int SimulateComputer::RET(void) {
    return 0;
}

int SimulateComputer::PUSH(void) {
    return 0;
}

int SimulateComputer::POP(void) {
    return 0;
}

int SimulateComputer::LOADB(void) {
    return 0;
}

int SimulateComputer::LOADW(void) {
    return 0;
}

int SimulateComputer::STOREB(void) {
    return 0;
}

int SimulateComputer::STOREW(void) {
    return 0;
}

int SimulateComputer::LOADI(void) {
    return 0;
}

int SimulateComputer::NOP(void) {
    return 0;
}

int SimulateComputer::IN(void) {
    return 0;
}

int SimulateComputer::OUT(void) {
    return 0;
}

int SimulateComputer::ADD(void) {
    return 0;
}

int SimulateComputer::ADDI(void) {
    return 0;
}

int SimulateComputer::SUB(void) {
    return 0;
}

int SimulateComputer::SUBI(void) {
    return 0;
}

int SimulateComputer::MUL(void) {
    return 0;
}

int SimulateComputer::DIV(void) {
    return 0;
}

int SimulateComputer::AND(void) {
    return 0;
}

int SimulateComputer::OR(void) {
    return 0;
}

int SimulateComputer::NOR(void) {
    return 0;
}

int SimulateComputer::NOTB(void) {
    return 0;
}

int SimulateComputer::SAL(void) {
    return 0;
}

int SimulateComputer::SAR(void) {
    return 0;
}

int SimulateComputer::EQU(void) {
    return 0;
}

int SimulateComputer::LT(void) {
    return 0;
}

int SimulateComputer::LTE(void) {
    return 0;
}

int SimulateComputer::NOTC(void) {
    return 0;
}

void SimulateComputer::simulateComputer(vector<string> binaryCode) {

}
