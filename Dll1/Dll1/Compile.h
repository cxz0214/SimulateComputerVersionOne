#pragma once
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
/*�����Ϣ*/
typedef struct marknumber {
	char name[30];
	int lineNumber;
}markNumber;
/*������Ϣ*/
typedef struct variable {
	char name[30];
	int size;
	int offset;
	short value[256];
	int type;
}variable;
class Compile {
private:
	list<markNumber> markInfoList;
	list<variable> variableInfoList;
public:
	Compile() {}
	int getCodeOfInstruct(const char* charOfOperator);
	/*��һ��ָ��ת��Ϊ��Ӧ�ı���*/
	unsigned long transferInstructToCode(char* instructLine, int instructCode);
	int saveMarkNumberInfo(char* markNumberName, int lineNumber);
	/*��ű�����Ϣ*/
	int saveVariableInfo(char* instructLine, int offset, int size, int type);
	/*ͨ���Ĵ�������ȡ�Ĵ������*/
	int getNumberOfReg(char* instructLine, char* regName);
	/*��ָ�����Ƿ���ȡ��Ӧ�Ĳ�����*/
	int getOperatorCodeOfInstruct(const char* opString);
	/*����*/
	vector<string> getResultOfCompile(vector<string> assembleCode);
};


#endif //SIMULATECOMPUTERVERSIONONE_COMPILE_H
