#include<iostream>
#include <cstring>

using namespace std;
#include "Compile.h"
#include "SimulateComputer.h"
#include "Decompile.h"
/*输入汇编代码所在文件的绝对路径*/
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
    cout<<"读入的汇编文件为：\n";
    for(iterator = result.begin(); iterator != result.end();iterator++){
        cout<<*iterator;
    }
    cout<<"---------------------------------------------------"<<endl;
    cout<<"汇编器执行的结果为："<<endl;
    for(iterator = compileResult.begin(); iterator != compileResult.end();iterator++){
        cout<<*iterator;
    }
    cout<<endl;
    cout<<"";
    SimulateComputer* computer = new SimulateComputer();
    computer->simulateComputer(compileResult);
    cout<<"反汇编执行结果："<<endl;
    Decompile* decompile = new Decompile();
    result = decompile->getResultOfDecompile(compileResult);
    for(iterator = result.begin(); iterator != result.end();iterator++){
        cout<<*iterator<<endl;
    }
    return 0;
}
