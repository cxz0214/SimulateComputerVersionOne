#pragma once
//
// Created by Alex on 2020/12/22.
//

#ifndef SIMULATECOMPUTERVERSIONONE_DECOMPILE_H
#define SIMULATECOMPUTERVERSIONONE_DECOMPILE_H
// ʵ��һ���������
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include<fstream>
using namespace std;
// ������ָ��ṹ��
// ָ����+������
// ö�����͵Ķ�����ָ�
enum BinIns
{
	binHalt, binJump,
	binCjmp, binOjmp, binCall,
	binRet, binPush, binPop,
	binLoadb, binLoadw, binStoreb, binStorew,
	binLoadi, binNop, binIn, binOut, binAdd, binAddi, binSub, binSubi,
	binMul, binDiv, binAnd, binOr, biNor, binNotb, binSal, binSar,
	binEqu, binLt, binLte, binNotc
};
struct Instruction
{
	BinIns op;  // ָ����ֻռһ���ֽ�
	int    arg; // ��������ռ�ĸ��ֽ�
};
class Decompile {
private:
	int numberOfLine;
	int countOfMark;
	int markNumber[100];
public:
	Decompile();
	void initAssembleInstructions(vector<string>& assIns);
	void InitBinaryToAssemble(const vector<string>& assIns, map<BinIns, string>& binToIns);
	void ReadBinary(vector<string>& bin);
	int todec(string a);
	void MyBinaryToAssemble(const vector<string>& bin, Instruction *&ins, vector<string>& ass, const map<BinIns, string>& binToIns);
	void BinaryToAssemble(const vector<string>& bin, vector<string>& ass, const map<BinIns, string>& binToIns, map<BinIns, int>& insArgNum);
	string StringToNum(const string& str);
	void BinaryToDec(vector<string>& bin);
	string xtoBinary_single(string& bin);
	vector<string>xtoBinary(vector<string>& bin);
	void addCountOfMark(vector<string>& ass);
	vector<string> getResultOfDecompile(vector<string> binaryCode);
};

#endif //SIMULATECOMPUTERVERSIONONE_DECOMPILE_H
