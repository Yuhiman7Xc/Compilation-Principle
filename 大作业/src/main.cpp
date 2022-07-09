#include "translation.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    if(Word_analysis()){    // 进行词法分析，在 word_analtsis.h 中
        puts("词法分析：成功!");
        if(grammar_analysis()){ // 进行语法分析，在 grammer_analysis.h 中
            puts("语法分析：成功!");
            if(translation()) puts("翻译：成功！"); // 进行中间代码、目标代码生成，在 translation.h 中
            else puts("翻译：失败！");
        }
        else puts("语法分析：失败！");
    }
    else puts("词法分析：失败！");

    return 0;
}