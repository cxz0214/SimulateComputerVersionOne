//
// Created by Alex on 2020/12/20.
//
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include "Compile.h"
using namespace std;
/*指令集助记符*/
#define INSTRUCTSET { "HLT", "JMP", "CJMP", "OJMP", "CALL", "RET","PUSH", "POP", "LOADB", "LOADW","STOREB", "STOREW","LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB","SUBI","MUL", "DIV", "AND", "OR", "NOR", "NOTB","SAL", "SAR","EQU", "LT", "LTE", "NOTC"}
const char* instructSet[] = INSTRUCTSET;
/*32条指令对应的指令编码格式，编号1-8代表8中指令编码格式*/
/******************************************************************************
 *  1：  操作码 (5bit) + fill(27bit)                                          *
 *  2：  操作码 (5bit) + fill(3bit) + address(24bit)                          *
 *  3:   操作码 (5bit) + reg0(3bit) + fill(24bit)                             *
 *  4:   操作码 (5bit) + reg0(3bit) + address(24bit)                          *
 *  5:   操作码 (5bit) + reg0(3bit) + fill(8bit) + immediate(16bit)           *
 *  6:   操作码 (5bit) + reg0(3bit) + fill(16bit) + port(8bit)                *
 *  7:   操作码 (5bit) + reg0(3bit) + reg1(4bit) + reg2(4bit) + fill(16bit)   *
 *  8:   操作码 (5bit) + reg0(3bit) + reg1(4bit) + fill(20bit)                *
 ******************************************************************************/
const char instructFormat[33] = "12222133444451667575777778778881";
/*通过寄存器名获取寄存器编号*/
int Compile::getNumberOfReg(char *instructLine, char *regName) {
    int regNumber;
    if(tolower(*regName) == 'z')
    {
        regNumber = 0;
    }
    else if((tolower(*regName) >= 'a') && (tolower(*regName) <= 'g'))
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
/*将一条指令转化为对应的编码*/
unsigned long Compile::transferInstructToCode(char *instructLine, int instructCode) {
    unsigned long op_code; /*op_code为指令操作码*/
    unsigned long arg1, arg2, arg3; /*arg为寄存器对应的编号*/
    unsigned long ins_code = 0ul;
    char op_sym[8], reg0[8], reg1[8], reg2[8];/*op_sym为指令助记符reg为寄存器名*/
    unsigned long addr;
    int immed, port, n;
    char string[20];
    list<markNumber>::iterator markInfoIterator;
    list<variable>::iterator variableInfoIterator;
    switch(instructFormat[instructCode])
    {
        case '1': /*类型 op(5b) + padding(27)*/
        {
            op_code = instructCode;
            ins_code = op_code << 27;
            break;
        }
        case '2': /*类型op(5b) + padding(3b) + address(24b)*/
        {
            n = sscanf(instructLine, "%s %s", op_sym, string);
            if(n<2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            for(markInfoIterator = markInfoList.begin(); markInfoIterator != markInfoList.end(); markInfoIterator++)
            {
                if(strcmp(markInfoIterator->name,string)==0)
                {
                    break;
                }
            }
            if(markInfoIterator == markInfoList.end())
            {
                printf("ERROR:%s",instructLine);
                exit(-1);
            }
            addr = (unsigned long)((markInfoIterator->lineNumber)*4);
            op_code = getOperatorCodeOfInstruct(op_sym);/*由助记符得到操作码*/
            ins_code = (op_code << 27) | (addr & 0x00ffffff);
            break;
        }
        case '3': /*类型op(5b) + reg0(3b) + padding(24b)*/
        {
            n = sscanf(instructLine, "%s %s", op_sym, reg0);
            if(n < 2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = getOperatorCodeOfInstruct(op_sym);
            /*由寄存器名得到寄存器编号*/
            arg1 = getNumberOfReg(instructLine, reg0);
            ins_code = (op_code << 27) | (arg1 << 24);
            break;
        }
        case '4': /*类型op(5b) + reg0(3b) + address(24b)*/
        {
            n = sscanf(instructLine, "%s %s %s", op_sym, reg0, string);
            if(n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            for(variableInfoIterator = variableInfoList.begin(); variableInfoIterator != variableInfoList.end(); variableInfoIterator++)
            {
                if(strcmp(variableInfoIterator->name ,string)==0)
                {
                    break;
                }
            }
            if(variableInfoIterator != variableInfoList.end())
            {
                printf("ERROR:%s",instructLine);
                exit(-1);
            }
            addr = (unsigned long)(variableInfoIterator->offset);
            op_code = getOperatorCodeOfInstruct(op_sym);
            /*由寄存器名得到寄存器编号*/
            arg1 = getNumberOfReg(instructLine, reg0);
            ins_code = (op_code << 27) | (arg1 << 24) | (addr&0x00ffffff);
            break;
        }
        case '5': /*类型 op(5b)+reg0(3b)+padding(8b)+immediate(16b)*/
        {
            n = sscanf(instructLine, "%s %s %i", op_sym, reg0, &immed);
            if(n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = getOperatorCodeOfInstruct(op_sym);

            arg1 = getNumberOfReg(instructLine, reg0);
            ins_code = (op_code << 27) | (arg1 << 24) | (immed & 0x0000ffff);
            break;
        }
        case '6': /*类型op(5b) + reg0(3b) + padding(16b) + port(8b)*/
        {
            n = sscanf(instructLine, "%s %s %i", op_sym, reg0, &port);
            if(n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = getOperatorCodeOfInstruct(op_sym);
            arg1 = getNumberOfReg(instructLine, reg0);
            ins_code = (op_code << 27) | (arg1 << 24) | (port & 0x0000ffff);
            break;
        }
        case '7':/*类型op(5b) + reg0(3b) + reg1(4b) + reg2(4b) + padding(16b)*/
        {
            n = sscanf(instructLine, "%s%s%s%s", op_sym, reg0, reg1, reg2);
            if(n < 4)
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
        case '8':/*类型op(5b)+reg0(3b)+reg1(4b) + padding(20b)*/
        {
            n = sscanf(instructLine, "%s%s%s", op_sym, reg0, reg1);
            if(n < 3)
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
    return ins_code; /*返回目标代码*/
}
/*根据助记符获取指令的操作码*/
int Compile::getOperatorCodeOfInstruct(const char *opString) {
    int i = 0;
    for(; i < 32; i++){
        if(strcmp(instructSet[i],opString) == 0)
            break;
    }
    return i;
}
/*保存变量信息*/
int Compile::saveVariableInfo(char *instructLine, int offset, int size, int type) {
//    var *p1;
    list<variable>::iterator variableInfoIterator;
    short value1;
    char *p = NULL;
    char value_str[20];
    int i, k, fuzhi = 0;
    char varname_temp[20]; /*存放临时的变量名称*/
    char string[200]; /*存放定义变量name和size的字符串*/
    sscanf(instructLine,"%*s %s",string);
    sscanf(string,"%[^[]",varname_temp); /*存变量名*/

    for(variableInfoIterator = variableInfoList.begin(); variableInfoIterator != variableInfoList.end() ; variableInfoIterator++);
    variable*  newVariable = (variable*)malloc(sizeof(variable));
    newVariable->offset = offset;
    newVariable->size = size;
    newVariable->type = type;
    strcpy(newVariable->name, varname_temp); /*变量名称*/
    /*以下对变量的赋值进行处理*/
    for(i = 0; i < size; i++)
    {
//        pnew->value[i] = 0;
        newVariable->value[i] = 0;
    }
    if(p == strchr(instructLine,'='))
    {
        if(size == 1) /*只有一个值*/
        {
            sscanf(instructLine, "%*[^=]=%hd", &value1);
            newVariable->value[0] = value1;
        }
        else
        {
            for(; p != NULL; p++)
            {
                if(*p == '{')
                {
                    ++p;
                    fuzhi = 1; /*表示将会被赋值*/
                    k = 0;
                    while(*p != '}')
                    {
                        for(i = 0; *p != ',' && *p != '}'; p++)
                        {
                            value_str[i++] = *p;
                        }
                        value_str[i] = '\0';
                        newVariable->value[k++] = atoi(value_str);
                        if(*p == ',')
                        {
                            ++p;
                        }
                    }
                }
                else if(*p == '"')
                {
                    fuzhi = 1; /*表示将会被赋值*/
                    ++p;
                    k = 0;
                    for(i = 0; *p != '"'; p++)
                    {
                        newVariable->value[k++] = *p;
                    }
                    newVariable->value[k] = '\0';
                }
                if(fuzhi){break;}
            }
        }

    }
    variableInfoList.push_back(*newVariable);
    return 0;
}
/*保存标号信息*/
int Compile::saveMarkNumberInfo(char *markNumberName, int lineNumber) {
    list<markNumber>::iterator markNumberIterator;
    for(markNumberIterator = markInfoList.begin(); markNumberIterator != markInfoList.end();markNumberIterator++);
    markNumber* newMarkNumber = (markNumber*)malloc(sizeof(markNumber));
    newMarkNumber->lineNumber = lineNumber;
    strcpy(newMarkNumber->name,markNumberName);
    markInfoList.push_back(*newMarkNumber);
    return 1;
}
/*返回汇编结果*/
vector<string> Compile::getResultOfCompile(vector<string> assembleCode) {
    vector<string> resultOfCompile;
    char a_line[200];
    char op_sym[8];
    int op_num = 0, i;
    char *pcpos;
    int type;
    char biaohao_temp[20]; /*存放临时的标号名称*/
    char varstring[200]; /*存放定义变量name和size的字符串*/
    char var_size_str[20]; /*存放变量的大小(字符串格式)*/
    int var_size; /*存放变量的大小(int型格式)*/
    int offset = 0;
    int n;
    int line_num = 0; /*存代码当前的行数*/
    vector<string>::iterator assembleCodeIterator;
    for(assembleCodeIterator = assembleCode.begin(); assembleCodeIterator != assembleCode.end();assembleCodeIterator++){
        strcpy(a_line, assembleCodeIterator->c_str());
        if((pcpos = strchr(a_line,'#')) != NULL)
            *pcpos = '\0';
        n = sscanf(a_line,"%s",op_sym);
        if(n < 1)
            continue;
        if((pcpos = strchr(a_line,':'))!= NULL){
            sscanf(a_line,"%[^:]",biaohao_temp);
            printf("%s\n",biaohao_temp);
            saveMarkNumberInfo(biaohao_temp,line_num);
        }else if((pcpos = strstr(a_line,"WORD")) != NULL){
            sscanf(a_line,"%*s %s",varstring);
            type = 2;
            if((pcpos = strchr(varstring, '[')) != NULL) /*若变量为数组格式*/{
                sscanf(varstring, "%*[^[][%[^]]", var_size_str);/*存变量大小*/
                var_size = atoi(var_size_str);
            }
            else{
                var_size = 1;
            }
            saveVariableInfo(a_line,offset,var_size,type);
            offset += 2 * var_size;
            line_num--;
        }
        else if((pcpos = strstr(a_line,"BYTE")) != NULL){
            sscanf(a_line,"%*s %s",varstring);
            type = 1;
            if((pcpos = strchr(varstring, '[')) != NULL) /*若变量为数组格式*/{
                sscanf(varstring, "%*[^[][%[^]]", var_size_str);/*存变量大小*/
                var_size = atoi(var_size_str);
            }
            else{
                var_size = 1;
            }
            saveVariableInfo(a_line,offset,var_size,type);
            offset += var_size;
            line_num--;
        }
        line_num++;
    }
    /*第二次读源文件，处理32个指令助记符*/
    for(assembleCodeIterator = assembleCode.begin(); assembleCodeIterator != assembleCode.end();assembleCodeIterator++){
        strcpy(a_line,assembleCodeIterator->c_str());
        if((pcpos = strchr(a_line,'#')) != NULL)
            *pcpos = '\0';
        if(strstr(a_line,"WORD") != NULL || strstr(a_line,"BYTE") != NULL)
            continue;
        else if(strchr(a_line,':') != NULL){
            for(i = 0; a_line[i] != ':'; i++)
            {
                a_line[i]=' ';
            }
            a_line[i]=' ';
        }
        n = sscanf(a_line,"%s",op_sym);
        if(n < 1)
            continue;
        op_num = getOperatorCodeOfInstruct(op_sym);
        if(op_num > 31){
            printf("ERROR: %s is a invalid instruction! \n", a_line);
            exit(-1);
        }
        //printf("0x%08lx\n", transferInstructToCode(a_line,op_num));
        char str[50];
        sprintf(str,"0x%08lx\n",transferInstructToCode(a_line,op_num));
        resultOfCompile.push_back(str);
    }
	resultOfCompile.push_back("0x00000000");
    return resultOfCompile;
}
