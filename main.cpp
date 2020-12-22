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
    vector<string> result;
    vector<string> compileResult;
    result = readFile(filePath);
    compileResult =  compile->getResultOfCompile(result);

    SimulateComputer* computer = new SimulateComputer();
    computer->simulateComputer(compileResult);
    Decompile* decompile = new Decompile();
    result = decompile->getResultOfDecompile(compileResult);
    vector<string>::iterator iterator;
    for(iterator = result.begin(); iterator != result.end();iterator++){
        cout<<*iterator<<endl;
    }

    return 0;
}
