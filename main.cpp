#include<iostream>
#include <cstring>

using namespace std;
#include "Compile.h"
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
//        if(strchr(temp,'#') != NULL)
//            continue;
//        else
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
    vector<string>::iterator iterator;
    for(iterator = compileResult.begin(); iterator != compileResult.end();iterator++){
        cout<<*iterator;
    }
    return 0;
}
