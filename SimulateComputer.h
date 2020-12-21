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

/*״̬����Ϣ*/
struct state{
    unsigned short overflow:1;/*�ж����1Ϊ�����0Ϊ����*/
    unsigned short compare:1;/*�Ƚϴ�С��1Ϊ�棬0Ϊ��*/
    unsigned short reserve:14;/*��ʵ������*/
}programStateWord;
/*�����Ӻ���ʱ���浱ǰ����ִ�е�״̬*/
typedef struct scence{
    struct state statement; /*��¼�ֶνṹ*/
    short reg[8]; /*��¼�Ĵ���ֵ*/
    unsigned long programLineNumber; /*��¼��ǰִ�е�������*/
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
