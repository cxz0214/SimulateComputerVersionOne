//
// Created by Alex on 2020/12/21.
//
#include"pch.h"
#include <cstring>
#include "SimulateComputer.h"
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
struct state {
	unsigned short overflow : 1;/*判定溢出1为溢出，0为正常*/
	unsigned short compare : 1;/*比较大小，1为真，0为假*/
	unsigned short reserve : 14;/*无实际意义*/
}programStateWord;
/*调用子函数时保存当前程序执行的状态*/
typedef struct scence {
	struct state statement; /*记录字段结构*/
	short reg[8]; /*记录寄存器值*/
	unsigned long programLineNumber; /*记录当前执行到的行数*/
}sceneOfProgramInterrupt;
/*PC 指令计数器，用来存放下条指令的内存地址*/
unsigned long *pc, d[300], op[300], num[100], IR;
short generalReg[8];/*8个通用寄存器*/
int pos = 0;/*记录程序运行的位置*/
int ss[100], x = 0;
list<sceneOfProgramInterrupt> interruptScenceList;  /*记录程序调用子函数时的状态*/

int SimulateComputer::HLT(void) {
	pos++;
	return 0;
}
int SimulateComputer::JMP(void) {
	pos = ADDRESS / 4;
	pc = (unsigned long *)(op + ADDRESS / 4);
	return 1;
}
int SimulateComputer::CJMP(void) {
	if (programStateWord.compare)
	{
		pos = ADDRESS / 4;
		pc = (unsigned long *)(op + ADDRESS / 4);
	}
	else {
		pos++;
	}
	return 1;
}

int SimulateComputer::OJMP(void) {
	if (programStateWord.overflow)
	{
		pos = ADDRESS / 4;
		pc = (unsigned long *)(op + ADDRESS / 4);
	}
	else {
		pos++;
	}

	return 1;
}

int SimulateComputer::CALL(void) {
	if (interruptScenceList.empty())
	{
		sceneOfProgramInterrupt * newScene = (sceneOfProgramInterrupt*)malloc(sizeof(sceneOfProgramInterrupt));
		newScene->programLineNumber = pos;
		for (int i = 0; i < 8; i++)
			newScene->reg[i] = generalReg[i];
		newScene->statement = programStateWord;
		interruptScenceList.push_back(*newScene);
	}
	else
	{
		sceneOfProgramInterrupt * newScene = (sceneOfProgramInterrupt*)malloc(sizeof(sceneOfProgramInterrupt));
		newScene->programLineNumber = pos;
		newScene->statement = programStateWord;
		for (int i = 0; i < 8; i++)
			newScene->reg[i] = generalReg[i];
		interruptScenceList.push_back(*newScene);
	}
	pos = ADDRESS / 4;
	pc = (unsigned long *)(op + ADDRESS / 4);
	return 1;
}

int SimulateComputer::RET(void) {
	sceneOfProgramInterrupt temp = interruptScenceList.back();
	pc = (unsigned long*)(op + temp.programLineNumber + 1);
	pos = temp.programLineNumber + 1;
	programStateWord = temp.statement;
	for (int i = 0; i < 8; i++)
		generalReg[i] = temp.reg[i];
	interruptScenceList.pop_back();
	return 1;
}

int SimulateComputer::PUSH(void) {
	if (x >= 100)
	{
		printf("ERROR:too many numbers\n");
		exit(0);
	}
	if (REG0 == 0)
	{
		printf("ERROR:wrong register\n");
		exit(0);
	}
	x++;
	ss[x] = generalReg[REG0];
	pos++;
	return 1;
}

int SimulateComputer::POP(void) {
	if (x == 0)
	{
		printf("ERROR:too few numbers\n");
		exit(0);
	}
	generalReg[REG0] = ss[x];
	x--;
	pos++;
	return 1;
}

int SimulateComputer::LOADB(void) {
	generalReg[REG0] = num[ADDRESS + generalReg[7] - 1];
	pos++;
	return 1;
}

int SimulateComputer::LOADW(void) {
	generalReg[REG0] = num[ADDRESS + generalReg[7] - 1] + (num[ADDRESS + generalReg[7]] << 8);
	pos++;
	return 1;
}
int SimulateComputer::STOREB(void) {
	num[ADDRESS + generalReg[7] - 1] = generalReg[REG0];
	pos++;
	return 1;
}

int SimulateComputer::STOREW(void) {
	num[ADDRESS + generalReg[7] - 1] = generalReg[REG0] & 0xff;
	num[ADDRESS + generalReg[7]] = (generalReg[REG0] >> 8) & 0xff;
	pos++;
	return 1;
}

int SimulateComputer::LOADI(void) {
	generalReg[REG0] = IMMEDIATE;
	pos++;
	return 1;
}

int SimulateComputer::NOP(void) {
	pos++;
	return 1;
}

int SimulateComputer::INI(void) {
	scanf("%hd", &generalReg[REG0]);
	pos++;
	return 1;
}

int SimulateComputer::OUTI(void) {
	printf("%c", generalReg[REG0] & (0xff));
	pos++;
	return 1;
}

int SimulateComputer::ADD(void) {
	long int a, b, c;
	b = generalReg[REG1];
	c = generalReg[REG2];
	generalReg[REG0] = generalReg[REG1] + generalReg[REG2];
	a = generalReg[REG0];
	if ((b + c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}

	pos++;
	return 1;
}

int SimulateComputer::ADDI(void) {
	int k;
	long int a, b, c;
	b = generalReg[REG0];
	c = IMMEDIATE;
	k = generalReg[REG0] + IMMEDIATE;
	a = k;
	if ((b + c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}

	generalReg[REG0] = k;
	pos++;
	return 1;
}

int SimulateComputer::SUB(void) {
	long int a, b, c;
	b = generalReg[REG1];
	c = generalReg[REG2];
	generalReg[REG0] = generalReg[REG1] - generalReg[REG2];
	a = generalReg[REG0];
	if ((b - c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}
	pos++;
	return 1;
}

int SimulateComputer::SUBI(void) {
	int k;
	long int a, b, c;
	b = generalReg[REG0];
	c = IMMEDIATE;
	k = generalReg[REG0] - IMMEDIATE;
	a = k;
	if ((b - c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}
	generalReg[REG0] = k;
	pos++;
	return 1;
}

int SimulateComputer::MUL(void) {
	long int a, b, c;
	b = generalReg[REG1];
	c = generalReg[REG2];
	generalReg[REG0] = generalReg[REG1] * generalReg[REG2];
	a = generalReg[REG0];
	if ((b*c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}
	pos++;
	return 1;
}

int SimulateComputer::DIV(void) {
	long int a, b, c;
	if (generalReg[REG2] == 0)
	{
		printf("WRONG NUMBER\n");
		exit(0);
	}
	b = generalReg[REG1];
	c = generalReg[REG2];
	generalReg[REG0] = generalReg[REG1] / generalReg[REG2];
	a = generalReg[REG0];
	if ((b / c) != a) {
		programStateWord.overflow = 1;
	}
	else {
		programStateWord.overflow = 0;
	}
	pos++;
	return 1;
}

int SimulateComputer::AND(void) {
	generalReg[REG0] = generalReg[REG1] & generalReg[REG2];
	pos++;
	return 1;
}

int SimulateComputer::OR(void) {
	generalReg[REG0] = generalReg[REG1] | generalReg[REG2];
	pos++;
	return 1;
}

int SimulateComputer::NOR(void) {
	generalReg[REG0] = generalReg[REG1] ^ generalReg[REG2];
	pos++;
	return 1;
}

int SimulateComputer::NOTB(void) {
	generalReg[REG0] = ~generalReg[REG1];
	pos++;
	return 1;
}

int SimulateComputer::SAL(void) {
	generalReg[REG0] = generalReg[REG1] << generalReg[REG2];
	pos++;
	return 1;
}

int SimulateComputer::SAR(void) {
	generalReg[REG0] = generalReg[REG1] >> generalReg[REG2];
	pos++;
	return 1;
}

int SimulateComputer::EQU(void) {
	if (generalReg[REG0] == generalReg[REG1]) {
		programStateWord.compare = 1;
	}
	else {
		programStateWord.compare = 0;
	}
	pos++;
	return 1;
}

int SimulateComputer::LT(void) {
	if (generalReg[REG0] < generalReg[REG1])
		programStateWord.compare = 1;
	else
		programStateWord.compare = 0;
	pos++;
	return 1;
}

int SimulateComputer::LTE(void) {
	if (generalReg[REG0] <= generalReg[REG1])
		programStateWord.compare = 1;
	else
		programStateWord.compare = 0;
	pos++;
	return 1;
}

int SimulateComputer::NOTC(void) {
	if (programStateWord.compare == 1) {
		programStateWord.compare = 0;
	}
	else {
		programStateWord.compare = 1;
	}
	pos++;
	return 1;
}

void SimulateComputer::simulateComputer(vector<string> binaryCode) {
	char a_line[200];
	int ret = 1, m, i = 0, j, y = 0, amount;
	pc = d;
	vector<string>::iterator binaryCodeIterator;
	for (binaryCodeIterator = binaryCode.begin(); binaryCodeIterator != binaryCode.end(); binaryCodeIterator++) {
		strcpy(a_line, binaryCodeIterator->c_str());
		sscanf(a_line, "%li", &pc[i++]);
		op[i - 1] = pc[i - 1];
	}
	amount = op[i - 2];
	if (amount != 0)
	{
		if (amount % 4 == 0)
		{
			j = i - amount / 4 - 2;
			amount = amount / 4;
		}
		else
		{
			j = i - amount / 4 - 1;
			amount = amount / 4 + 1;
		}
		for (m = 0; m < amount; m++, j++)
		{
			num[y++] = op[j] & 0xff;
			num[y++] = (op[j] >> 8) & 0xff;
			num[y++] = (op[j] >> 16) & 0xff;
			num[y++] = (op[j] >> 24) & 0xff;
		}
	}
	while (ret)
	{
		IR = *pc;
		pc++;
		ret = (*opsPtr[OPCODE])();
	}
	return;
}

SimulateComputer::SimulateComputer() {
	opsPtr[0] = HLT;
	opsPtr[1] = JMP;
	opsPtr[2] = CJMP;
	opsPtr[3] = OJMP;
	opsPtr[4] = CALL;
	opsPtr[5] = RET;
	opsPtr[6] = PUSH;
	opsPtr[7] = POP;
	opsPtr[8] = LOADB;
	opsPtr[9] = LOADW;
	opsPtr[10] = STOREB;
	opsPtr[11] = STOREW;
	opsPtr[12] = LOADI;
	opsPtr[13] = NOP;
	opsPtr[14] = INI;
	opsPtr[15] = OUTI;
	opsPtr[16] = ADD;
	opsPtr[17] = ADDI;
	opsPtr[18] = SUB;
	opsPtr[19] = SUBI;
	opsPtr[20] = MUL;
	opsPtr[21] = DIV;
	opsPtr[22] = AND;
	opsPtr[23] = OR;
	opsPtr[24] = NOR;
	opsPtr[25] = NOTB;
	opsPtr[26] = SAL;
	opsPtr[27] = SAR;
	opsPtr[28] = EQU;
	opsPtr[29] = LT;
	opsPtr[30] = LTE;
	opsPtr[31] = NOTC;
}
