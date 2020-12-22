#include<iostream>
#include <cstring>

using namespace std;
#include "Compile.h"
#include "SimulateComputer.h"
#include "Decompile.h"
/*��������������ļ��ľ���·��*/
vector<string> readFile(char* filePath){
    vector<string> readResult;
    char temp[200];
    FILE * pFileInput;
    if((pFileInput =  fopen(filePath, "r"))==NULL){
        printf("ERROR: cannot open file\n");
        exit(0);
    }
    while(!feof(pFileInput)){

        if(fgets(temp,200,pFileInput)==NULL)
			break;
        readResult.push_back(temp);
    }
    return readResult;

}
int main() {
    Compile* compile = new Compile();
    char filePath[] = "D:\\Practice\\Compile\\cmake-build-debug\\sum.txt";
    vector<string>::iterator iterator;
    vector<string> result;
    vector<string> compileResult;
    result = readFile(filePath);
    compileResult =  compile->getResultOfCompile(result);
    cout<<"����Ļ���ļ�Ϊ��\n";
    for(iterator = result.begin(); iterator != result.end();iterator++){
        cout<<*iterator;
    }
    cout<<"---------------------------------------------------"<<endl;
    cout<<"�����ִ�еĽ��Ϊ��"<<endl;
    for(iterator = compileResult.begin(); iterator != compileResult.end();iterator++){
        cout<<*iterator;
    }
    cout<<endl;
    cout<<"";
    SimulateComputer* computer = new SimulateComputer();
    computer->simulateComputer(compileResult);
    cout<<"�����ִ�н����"<<endl;
    Decompile* decompile = new Decompile();
    result = decompile->getResultOfDecompile(compileResult);
    for(iterator = result.begin(); iterator != result.end();iterator++){
        cout<<*iterator<<endl;
    }
    return 0;
}
