//
// Created by Alex on 2020/12/22.
//
#include <cmath>
#include "Decompile.h"
const char instructFormat[33] = "12222133444451667575777778778881";
string ox[16] = { "0000","0001","0010","0011" ,
                  "0100" ,"0101" ,"0110" ,"0111" ,
                  "1000" ,"1001","1010","1011",
                  "1100","1101","1110","1111" };

Decompile::Decompile() {
    numberOfLine = 0;
    countOfMark = 0;
}
/*初始化汇编指令集*/
void Decompile::initAssembleInstructions(vector<string> &assIns) {
    assIns.clear();
    assIns.push_back("HALT");
    assIns.push_back("JUMP");
    assIns.push_back("CJMP");
    assIns.push_back("OJMP");
    assIns.push_back("CALL");
    assIns.push_back("RET");
    assIns.push_back("PUSH");
    assIns.push_back("POP");
    assIns.push_back("LOADB");
    assIns.push_back("LOADW");
    assIns.push_back("STOREB");
    assIns.push_back("STOREW");
    assIns.push_back("LOADI");
    assIns.push_back("NOP");
    assIns.push_back("IN");
    assIns.push_back("OUT");
    assIns.push_back("ADD");
    assIns.push_back("ADDI");
    assIns.push_back("SUB");
    assIns.push_back("SUBI");
    assIns.push_back("MUL");
    assIns.push_back("DIV");
    assIns.push_back("AND");
    assIns.push_back("OR");
    assIns.push_back("NOR");
    assIns.push_back("NOTB");
    assIns.push_back("SAL");
    assIns.push_back("SAR");
    assIns.push_back("EQU");
    assIns.push_back("LT");
    assIns.push_back("LTE");
    assIns.push_back("NOTC");
}
/*建立二进制指令到汇编指令的映射，初始化*/
void Decompile::InitBinaryToAssemble(const vector<string> &assIns, map<BinIns, string> &binToIns) {
    binToIns.clear();
    for (auto i = 0; i != assIns.size(); ++i)
    {
        // assIns和BinIns的指令次序一致
        binToIns[static_cast<BinIns>(i)] = assIns[i];
    }
}
/*读入二进制指令*/
void Decompile::ReadBinary(vector<string>& bin){
    bin.clear();
    string line;
    while (getline(cin, line))
    {
        bin.push_back(line);
    }
}
int Decompile::todec(string a) {
    int count = 0;
    for (int i = 0; i < a.size(); i++) {
        count += int(a[i]-'0')*pow(2, (a.size()-i-1));
    }
    return count;
}
void Decompile::MyBinaryToAssemble(const vector<string> &bin, Instruction *&ins, vector<string> &ass,
                                   const map<BinIns, string> &binToIns) {
    ass.clear();
    string binLine;
    for (auto i = 0; i != bin.size(); ++i)
    {
        binLine += bin[i] + ' ';
    }
    istringstream sin(binLine);//从binline中读取字符
    string strOp, strArg;
    string op;
    string arg;
    string assIns;
    BinIns opBin;
    string a, b;
    int i = 0;
    while (sin >> strOp)//strop接受读入的字符
    {
        assIns.clear();
        ins[i].op = static_cast<BinIns>(todec(strOp.substr(0, 5)));
        //opBin = static_cast<BinIns>(atoi(strOp.c_str()));//string转char*转int
        auto cit = binToIns.find(ins[i].op);
        if (cit == binToIns.end())
        {
            // 非法二进制指令
            // 忽略处理
            ;
            break;
        }
        op = cit->second;
        //	int t = instructFormat[ins[i].op];
        if (instructFormat[ins[i].op]-'0' ==1) {
            //break;
            assIns = op ;
        }

        if (instructFormat[ins[i].op] - '0' ==2) {

            ins[i].arg = todec(strOp.substr(8, 24));
            int argNum = ins[i].arg;
            arg = to_string(ins[i].arg);
            if (op == "CJMP") {

                markNumber[countOfMark] = argNum;
                if (argNum == 36)
                {

                    assIns = op + ' ' + "loop1";

                }
                if (argNum == 20)
                {

                    assIns = op + ' ' + "again1";

                }
                if (argNum == 68)
                {

                    assIns = op + ' ' + "loop2";

                }
                ass.push_back(assIns);
                i++;
                countOfMark++;
                continue;
            }
            assIns = op + ' ' + arg;
        }
        if (instructFormat[ins[i].op ] - '0' ==3) {
            int reg= todec(strOp.substr(5,3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            assIns = op + ' ' + sreg;
        }
        if (instructFormat[ins[i].op] - '0' ==4) {
            int reg = todec(strOp.substr(5, 3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            int add= todec(strOp.substr(8, 24));
            string sadd = to_string(add);
            assIns = op + ' ' + sreg+ ' ' + sadd;
        }
        if (instructFormat[ins[i].op] - '0' == 5) {
            int reg = todec(strOp.substr(5, 3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            int imm= todec(strOp.substr(16, 16));
            string simm = to_string(imm);
            assIns = op + ' ' + sreg+ ' ' + simm;

        }
        if (instructFormat[ins[i].op] - '0' == 6) {
            int reg = todec(strOp.substr(5, 3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            int port = todec(strOp.substr(24, 8));
            string sport = to_string(port);
            assIns = op + ' ' + sreg + ' ' + sport;
        }
        if (instructFormat[ins[i].op] - '0' == 7) {
            int reg = todec(strOp.substr(5, 3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            int reg2 = todec(strOp.substr(8, 4));
            string sreg2;
            if (reg2 == 0)sreg2 = "Z";
            if (reg2 == 1)sreg2 = "A";
            if (reg2 == 2)sreg2 = "B";
            if (reg2 == 3)sreg2 = "C";
            if (reg2 == 4)sreg2 = "D";
            if (reg2 == 5)sreg2 = "E";
            if (reg2 == 6)sreg2 = "F";
            if (reg2 == 7)sreg2 = "G";
            int reg3 = todec(strOp.substr(12, 4));
            string sreg3;
            if (reg3 == 0)sreg3 = "Z";
            if (reg3 == 1)sreg3 = "A";
            if (reg3 == 2)sreg3 = "B";
            if (reg3 == 3)sreg3 = "C";
            if (reg3 == 4)sreg3 = "D";
            if (reg3 == 5)sreg3 = "E";
            if (reg3 == 6)sreg3 = "F";
            if (reg3 == 7)sreg3 = "G";
            assIns = op + ' ' + sreg + ' '+sreg2 + ' '+sreg3;
        }
        if (instructFormat[ins[i].op] - '0' == 8) {
            int reg = todec(strOp.substr(5, 3));
            string sreg;
            if (reg == 0)sreg = "Z";
            if (reg == 1)sreg = "A";
            if (reg == 2)sreg = "B";
            if (reg == 3)sreg = "C";
            if (reg == 4)sreg = "D";
            if (reg == 5)sreg = "E";
            if (reg == 6)sreg = "F";
            if (reg == 7)sreg = "G";
            int reg2 = todec(strOp.substr(8, 4));
            string sreg2;
            if (reg2 == 0)sreg2 = "Z";
            if (reg2 == 1)sreg2 = "A";
            if (reg2 == 2)sreg2 = "B";
            if (reg2 == 3)sreg2 = "C";
            if (reg2 == 4)sreg2 = "D";
            if (reg2 == 5)sreg2 = "E";
            if (reg2 == 6)sreg2 = "F";
            if (reg2 == 7)sreg2 = "G";
            assIns = op + ' ' + sreg + ' ' + sreg2 ;
        }
        ass.push_back(assIns);
        i++;
    }
}
/*将二进制指令转化为汇编指令*/
void Decompile::BinaryToAssemble(const vector<string> &bin, vector<string> &ass, const map<BinIns, string> &binToIns,
                                 map<BinIns, int> &insArgNum) {
    ass.clear();
    string binLine;
    for (auto i = 0; i != bin.size(); ++i)
    {
        binLine += bin[i] + ' ';
    }

    istringstream sin(binLine);//从binline中读取字符
    string strOp, strArg;
    string op;
    string arg;
    string assIns;
    BinIns opBin;
    while (sin >> strOp)//strop接受读入的字符
    {
        opBin = static_cast<BinIns>(atoi(strOp.c_str()));//string转char*转int
        auto cit = binToIns.find(opBin);
        if (cit == binToIns.end())
        {
            // 非法二进制指令
            // 忽略处理
            ;
            break;
        }
        op = cit->second;
        int argNum = insArgNum[cit->first];
        if (argNum > 0)
        {
            sin >> strArg;
            arg = strArg;
        }
        else
        {
            arg = "";
        }
        assIns = op + ' ' + arg;
        ass.push_back(assIns);
    }
}
/*十六进制字符串转为十进制字符串*/
string Decompile::StringToNum(const string &str) {
    string ret;
    int num = 0;
    for (auto i = 0; i != str.size(); ++i)
    {
        num = num * 2 + str[i] - '0';
    }
    char tmp[101];
    _itoa(num, tmp, 10);
    ret = tmp;
    return ret;
}
/*二进制转换为十进制指令，针对输入的二进制为二进制编码的情况*/
void Decompile::BinaryToDec(vector<string> &bin) {
    for (auto i = 0; i != bin.size(); ++i)
    {
        istringstream sin(bin[i]);
        string tmp, ins;
        while (sin >> tmp)
        {
            ins += StringToNum(tmp) + ' ';
        }
        bin[i] = ins;
    }
}
string Decompile::xtoBinary_single(string &bin) {
    string res="";
    for (int i = 0; i < bin.size(); i++) {
        if (bin[i] >= 'a'&&bin[i] <= 'f') {
            res += ox[bin[i] - 87];
        }
        else {
            res += ox[bin[i] - '0'];
        }
    }
    return res;
}
vector<string> Decompile::xtoBinary(vector<string> &bin) {
    vector<string>res;
    res.resize(bin.size());
    for (int i = 0; i < bin.size(); i++) {
        bin[i] = bin[i].substr(2, 8);
        res[i] = xtoBinary_single(bin[i]);
    }
    return res;
}
void Decompile::addCountOfMark(vector<string> &ass) {
    for (int i = 0; i < ass.size(); i++) {
        for (int j = 0; j < countOfMark; j++) {
            if (markNumber[j] / 4 == i) {
                if (markNumber[j] == 36)
                {
                    ass[i].insert(0, "loop1:  ");
                    break;
                }
                if (markNumber[j] == 68)

                    ass[i].insert(0, "loop2:  ");
                if (markNumber[j] == 20)

                    ass[i].insert(0, "again1:  ");

            }
        }

    }
}
vector<string> Decompile::getResultOfDecompile(vector<string> binaryCode) {
    // 汇编指令集
    vector<string> assIns;
    initAssembleInstructions(assIns);


    // 汇编指令到二进制的映射
    map<BinIns, string> binToAss;
    InitBinaryToAssemble(assIns, binToAss);

//    //vector<string> binaryCode; // 保存读入的二进制指令
//    //read_file(binaryCode);
//    cout << endl;
//    cout << endl;
    binaryCode = xtoBinary(binaryCode);


    vector<string> ass; // 保存转换后的汇编指令
    Instruction* ins = (Instruction*)malloc(sizeof(Instruction)*numberOfLine);
    MyBinaryToAssemble(binaryCode, ins, ass, binToAss);
    addCountOfMark(ass);
    return ass;
}