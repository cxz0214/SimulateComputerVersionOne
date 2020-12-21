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
/*�������ģ��ָ��Ľ���*/
/*IRΪָ��Ĵ���*/
#define REG0 ((IR>>24)&0x07)
#define REG1 ((IR>>20)&0x0f)
#define REG2 ((IR>>16)&0x0f)
#define IMMEDIATE (IR&0xffff)
#define ADDRESS (IR&0xffffff)
#define PORT (IR&0xff)
#define OPCODE ((IR>>27)&0x1f)
/*״̬����Ϣ*/
typedef struct state{
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
    list<sceneOfProgramInterrupt> interruptScenceList;  /*��¼��������Ӻ���ʱ��״̬*/
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
