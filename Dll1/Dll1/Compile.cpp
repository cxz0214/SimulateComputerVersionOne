//
// Created by Alex on 2020/12/20.
//
#include"pch.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include "Compile.h"
using namespace std;
/*ָ����Ƿ�*/
#define INSTRUCTSET { "HLT", "JMP", "CJMP", "OJMP", "CALL", "RET","PUSH", "POP", "LOADB", "LOADW","STOREB", "STOREW","LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB","SUBI","MUL", "DIV", "AND", "OR", "NOR", "NOTB","SAL", "SAR","EQU", "LT", "LTE", "NOTC"}
const char* instructSet[] = INSTRUCTSET;
/*32��ָ���Ӧ��ָ������ʽ�����1-8����8��ָ������ʽ*/
/******************************************************************************
 *  1��  ������ (5bit) + fill(27bit)                                          *
 *  2��  ������ (5bit) + fill(3bit) + address(24bit)                          *
 *  3:   ������ (5bit) + reg0(3bit) + fill(24bit)                             *
 *  4:   ������ (5bit) + reg0(3bit) + address(24bit)                          *
 *  5:   ������ (5bit) + reg0(3bit) + fill(8bit) + immediate(16bit)           *
 *  6:   ������ (5bit) + reg0(3bit) + fill(16bit) + port(8bit)                *
 *  7:   ������ (5bit) + reg0(3bit) + reg1(4bit) + reg2(4bit) + fill(16bit)   *
 *  8:   ������ (5bit) + reg0(3bit) + reg1(4bit) + fill(20bit)                *
 ******************************************************************************/
const char instructFormat[33] = "12222133444451667575777778778881";
/*ͨ���Ĵ�������ȡ�Ĵ������*/
int Compile::getNumberOfReg(char *instructLine, char *regName) {
	int regNumber;
	if (tolower(*regName) == 'z')
	{
		regNumber = 0;
	}
	else if ((tolower(*regName) >= 'a') && (tolower(*regName) <= 'g'))
	{
		regNumber = tolower(*regName) - 'a' + 1;
	}
	else
	{
		printf("ERROR:bad register name in %s!\n", instructLine);
		exit(-1);
	}
	return regNumber;
}
/*��һ��ָ��ת��Ϊ��Ӧ�ı���*/
unsigned long Compile::transferInstructToCode(char *instructLine, int instructCode) {
	unsigned long op_code; /*op_codeΪָ�������*/
	unsigned long arg1, arg2, arg3; /*argΪ�Ĵ�����Ӧ�ı��*/
	unsigned long ins_code = 0ul;
	char op_sym[8], reg0[8], reg1[8], reg2[8];/*op_symΪָ�����Ƿ�regΪ�Ĵ�����*/
	unsigned long addr;
	int immed, port, n;
	char string[20];
	list<markNumber>::iterator markInfoIterator;
	list<variable>::iterator variableInfoIterator;
	switch (instructFormat[instructCode])
	{
	case '1': /*���� op(5b) + padding(27)*/
	{
		op_code = instructCode;
		ins_code = op_code << 27;
		break;
	}
	case '2': /*����op(5b) + padding(3b) + address(24b)*/
	{
		n = sscanf(instructLine, "%s %s", op_sym, string);
		if (n < 2)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		for (markInfoIterator = markInfoList.begin(); markInfoIterator != markInfoList.end(); markInfoIterator++)
		{
			if (strcmp(markInfoIterator->name, string) == 0)
			{
				break;
			}
		}
		if (markInfoIterator == markInfoList.end())
		{
			printf("ERROR:%s", instructLine);
			exit(-1);
		}
		addr = (unsigned long)((markInfoIterator->lineNumber) * 4);
		op_code = getOperatorCodeOfInstruct(op_sym);/*�����Ƿ��õ�������*/
		ins_code = (op_code << 27) | (addr & 0x00ffffff);
		break;
	}
	case '3': /*����op(5b) + reg0(3b) + padding(24b)*/
	{
		n = sscanf(instructLine, "%s %s", op_sym, reg0);
		if (n < 2)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getOperatorCodeOfInstruct(op_sym);
		/*�ɼĴ������õ��Ĵ������*/
		arg1 = getNumberOfReg(instructLine, reg0);
		ins_code = (op_code << 27) | (arg1 << 24);
		break;
	}
	case '4': /*����op(5b) + reg0(3b) + address(24b)*/
	{
		n = sscanf(instructLine, "%s %s %s", op_sym, reg0, string);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		for (variableInfoIterator = variableInfoList.begin(); variableInfoIterator != variableInfoList.end(); variableInfoIterator++)
		{
			if (strcmp(variableInfoIterator->name, string) == 0)
			{
				break;
			}
		}
		if (variableInfoIterator != variableInfoList.end())
		{
			printf("ERROR:%s", instructLine);
			exit(-1);
		}
		addr = (unsigned long)(variableInfoIterator->offset);
		op_code = getOperatorCodeOfInstruct(op_sym);
		/*�ɼĴ������õ��Ĵ������*/
		arg1 = getNumberOfReg(instructLine, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (addr & 0x00ffffff);
		break;
	}
	case '5': /*���� op(5b)+reg0(3b)+padding(8b)+immediate(16b)*/
	{
		n = sscanf(instructLine, "%s %s %i", op_sym, reg0, &immed);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getOperatorCodeOfInstruct(op_sym);

		arg1 = getNumberOfReg(instructLine, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (immed & 0x0000ffff);
		break;
	}
	case '6': /*����op(5b) + reg0(3b) + padding(16b) + port(8b)*/
	{
		n = sscanf(instructLine, "%s %s %i", op_sym, reg0, &port);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getOperatorCodeOfInstruct(op_sym);
		arg1 = getNumberOfReg(instructLine, reg0);
		ins_code = (op_code << 27) | (arg1 << 24) | (port & 0x0000ffff);
		break;
	}
	case '7':/*����op(5b) + reg0(3b) + reg1(4b) + reg2(4b) + padding(16b)*/
	{
		n = sscanf(instructLine, "%s%s%s%s", op_sym, reg0, reg1, reg2);
		if (n < 4)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getOperatorCodeOfInstruct(op_sym);
		arg1 = getNumberOfReg(instructLine, reg0);
		arg2 = getNumberOfReg(instructLine, reg1);
		arg3 = getNumberOfReg(instructLine, reg2);
		ins_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20) | (arg3 << 16);
		break;

	}
	case '8':/*����op(5b)+reg0(3b)+reg1(4b) + padding(20b)*/
	{
		n = sscanf(instructLine, "%s%s%s", op_sym, reg0, reg1);
		if (n < 3)
		{
			printf("ERROR:bad instruction format!\n");
			exit(-1);
		}
		op_code = getOperatorCodeOfInstruct(op_sym);
		arg1 = getNumberOfReg(instructLine, reg0);
		arg2 = getNumberOfReg(instructLine, reg1);
		ins_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20);
		break;
	}
	}
	return ins_code; /*����Ŀ�����*/
}
/*�������Ƿ���ȡָ��Ĳ�����*/
int Compile::getOperatorCodeOfInstruct(const char *opString) {
	int i = 0;
	for (; i < 32; i++) {
		if (strcmp(instructSet[i], opString) == 0)
			break;
	}
	return i;
}
/*���������Ϣ*/
int Compile::saveVariableInfo(char *instructLine, int offset, int size, int type) {
	list<variable>::iterator variableInfoIterator;
	short value1;
	char *p = NULL;
	char value_str[20];
	int i, k, fuzhi = 0;
	char varname_temp[20]; /*�����ʱ�ı�������*/
	char string[200]; /*��Ŷ������name��size���ַ���*/
	sscanf(instructLine, "%*s %s", string);
	sscanf(string, "%[^[]", varname_temp); /*�������*/

	for (variableInfoIterator = variableInfoList.begin(); variableInfoIterator != variableInfoList.end(); variableInfoIterator++);
	variable*  newVariable = (variable*)malloc(sizeof(variable));
	newVariable->offset = offset;
	newVariable->size = size;
	newVariable->type = type;
	strcpy(newVariable->name, varname_temp); /*��������*/
	/*���¶Ա����ĸ�ֵ���д���*/
	for (i = 0; i < size; i++)
	{
		newVariable->value[i] = 0;
	}
	if (p == strchr(instructLine, '='))
	{
		if (size == 1) /*ֻ��һ��ֵ*/
		{
			sscanf(instructLine, "%*[^=]=%hd", &value1);
			newVariable->value[0] = value1;
		}
		else
		{
			for (; p != NULL; p++)
			{
				if (*p == '{')
				{
					++p;
					fuzhi = 1; /*��ʾ���ᱻ��ֵ*/
					k = 0;
					while (*p != '}')
					{
						for (i = 0; *p != ',' && *p != '}'; p++)
						{
							value_str[i++] = *p;
						}
						value_str[i] = '\0';
						newVariable->value[k++] = atoi(value_str);
						if (*p == ',')
						{
							++p;
						}
					}
				}
				else if (*p == '"')
				{
					fuzhi = 1; /*��ʾ���ᱻ��ֵ*/
					++p;
					k = 0;
					for (i = 0; *p != '"'; p++)
					{
						newVariable->value[k++] = *p;
					}
					newVariable->value[k] = '\0';
				}
				if (fuzhi) { break; }
			}
		}

	}
	variableInfoList.push_back(*newVariable);
	return 0;
}
/*��������Ϣ*/
int Compile::saveMarkNumberInfo(char *markNumberName, int lineNumber) {
	list<markNumber>::iterator markNumberIterator;
	for (markNumberIterator = markInfoList.begin(); markNumberIterator != markInfoList.end(); markNumberIterator++);
	markNumber* newMarkNumber = (markNumber*)malloc(sizeof(markNumber));
	newMarkNumber->lineNumber = lineNumber;
	strcpy(newMarkNumber->name, markNumberName);
	markInfoList.push_back(*newMarkNumber);
	return 1;
}
/*���ػ����*/
vector<string> Compile::getResultOfCompile(vector<string> assembleCode) {
	vector<string> resultOfCompile;
	char a_line[200];
	char op_sym[8];
	int op_num = 0, i;
	char *pcpos;
	int type;
	char biaohao_temp[20]; /*�����ʱ�ı������*/
	char varstring[200]; /*��Ŷ������name��size���ַ���*/
	char var_size_str[20]; /*��ű����Ĵ�С(�ַ�����ʽ)*/
	int var_size; /*��ű����Ĵ�С(int�͸�ʽ)*/
	int offset = 0;
	int n;
	int line_num = 0; /*����뵱ǰ������*/
	vector<string>::iterator assembleCodeIterator;
	for (assembleCodeIterator = assembleCode.begin(); assembleCodeIterator != assembleCode.end(); assembleCodeIterator++) {
		strcpy(a_line, assembleCodeIterator->c_str());
		if ((pcpos = strchr(a_line, '#')) != NULL)
			*pcpos = '\0';
		n = sscanf(a_line, "%s", op_sym);
		if (n < 1)
			continue;
		if ((pcpos = strchr(a_line, ':')) != NULL) {
			sscanf(a_line, "%[^:]", biaohao_temp);
			saveMarkNumberInfo(biaohao_temp, line_num);
		}
		else if ((pcpos = strstr(a_line, "WORD")) != NULL) {
			sscanf(a_line, "%*s %s", varstring);
			type = 2;
			if ((pcpos = strchr(varstring, '[')) != NULL) /*������Ϊ�����ʽ*/ {
				sscanf(varstring, "%*[^[][%[^]]", var_size_str);/*�������С*/
				var_size = atoi(var_size_str);
			}
			else {
				var_size = 1;
			}
			saveVariableInfo(a_line, offset, var_size, type);
			offset += 2 * var_size;
			line_num--;
		}
		else if ((pcpos = strstr(a_line, "BYTE")) != NULL) {
			sscanf(a_line, "%*s %s", varstring);
			type = 1;
			if ((pcpos = strchr(varstring, '[')) != NULL) /*������Ϊ�����ʽ*/ {
				sscanf(varstring, "%*[^[][%[^]]", var_size_str);/*�������С*/
				var_size = atoi(var_size_str);
			}
			else {
				var_size = 1;
			}
			saveVariableInfo(a_line, offset, var_size, type);
			offset += var_size;
			line_num--;
		}
		line_num++;
	}
	/*�ڶ��ζ�Դ�ļ�������32��ָ�����Ƿ�*/
	for (assembleCodeIterator = assembleCode.begin(); assembleCodeIterator != assembleCode.end(); assembleCodeIterator++) {
		strcpy(a_line, assembleCodeIterator->c_str());
		if ((pcpos = strchr(a_line, '#')) != NULL)
			*pcpos = '\0';
		if (strstr(a_line, "WORD") != NULL || strstr(a_line, "BYTE") != NULL)
			continue;
		else if (strchr(a_line, ':') != NULL) {
			for (i = 0; a_line[i] != ':'; i++)
			{
				a_line[i] = ' ';
			}
			a_line[i] = ' ';
		}
		n = sscanf(a_line, "%s", op_sym);
		if (n < 1)
			continue;
		op_num = getOperatorCodeOfInstruct(op_sym);
		if (op_num > 31) {
			printf("ERROR: %s is a invalid instruction! \n", a_line);
			exit(-1);
		}
		char str[50];
		sprintf(str, "0x%08lx\n", transferInstructToCode(a_line, op_num));
		resultOfCompile.push_back(str);
	}
	resultOfCompile.push_back("0x00000000");
	return resultOfCompile;
}
