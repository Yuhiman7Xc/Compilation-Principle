#include "translation.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    if(Word_analysis()){    // ���дʷ��������� word_analtsis.h ��
        puts("�ʷ��������ɹ�!");
        if(grammar_analysis()){ // �����﷨�������� grammer_analysis.h ��
            puts("�﷨�������ɹ�!");
            if(translation()) puts("���룺�ɹ���"); // �����м���롢Ŀ��������ɣ��� translation.h ��
            else puts("���룺ʧ�ܣ�");
        }
        else puts("�﷨������ʧ�ܣ�");
    }
    else puts("�ʷ�������ʧ�ܣ�");

    return 0;
}