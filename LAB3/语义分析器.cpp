#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <cassert>
#include <vector>
using namespace std;
/*
begin	BEGIN	1
end		END		2
if		IF		3
then	THEN	4
else	ELSE	5
while	WHILE	6
do		DO		7
��ʶ��	ID		8
���㳣��UCON	9
<		LT		10
<=		LE		11
==		EQ		12
<>		NE		13
>		GT		14
>=		GE		15
=		IS		16
+		PL		17
-		MI		18
*		MU		19
/		DI		20
(		LP		21
)		RP		22
*/

//�ʷ���������
const char* table1[] = { " ","begin","end","if","then","else","while","do" };
const char* table2[] = { " ","BEGIN","END","IF","THEN","ELSE","WHILE","DO","ID","UCON","LT","LE","EQ","NE","GT","GE","IS","PL","MI","MU","DI","LP","RP" };
char TOKEN[20];//�������δ��һ�����ʴ����еĸ����ַ���
char str[50];//�ʷ�������Ĵ���i�ı��ʽ
int len = 0;
//�﷨��������
string action[16][8] = { {"s4","null","null" ,"null" ,"null","null","s5" , "null" },
					{"null","null","s6","s7","null","null","null","acc"},
					{"null","r3","r3","r3","s8","s9","null","r3"} ,
					{"null","r6","r6","r6","r6","r6","null","r6"},
					{"s4","null","null","null","null","null","s5","null"},
					{"null","r8","r8","r8","r8","r8","null","r8"},
					{"s4","null","null","null","null","null","s5","null"},
					{"s4","null","null","null","null","null","s5","null"},
					{"s4","null","null","null","null","null","s5","null"},
					{"s4","null","null","null","null","null","s5","null"},
					{"null","s15","s6","s7","null","null","null","null"} ,
					{"null","r1","r1","r1","s8","s9","null","r1"},
					{"null","r2","r2","r2","s8","s9","null","r2"} ,
					{"null","r4","r4","r4","r4","r4","null","r4"} ,
					{"null","r5","r5","r5","r5","r5","null","r5"} ,
					{"null","r7","r7","r7","r7","r7","null","r7"} };
int go[16][3] = { {1 , 2 , 3 },
				{ -1 , -1 , -1 },
				{ -1 , -1 , -1 } ,
				{ -1 , -1 , -1 },
				{ 10 , 2 , 3 },
				{ -1 , -1 , -1 },
				{ -1 , 11 , 3 },
				{ -1 , 12 , 3 },
				{ -1 , -1 , 13 },
				{ -1 , -1 , 14 },
				{ -1 , -1 , -1 } ,
				{ -1 , -1 , -1 },
				{ -1 , -1 , -1 } ,
				{ -1 , -1 , -1 } ,
				{ -1 , -1 , -1 } ,
				{ -1 , -1 , -1 } };
//char vt[8] = { '(',')','+','-','*','/','i','#' };
//char vn[3] = { 'E','T','F' };
string grammar[9] = { "S->E","E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->i" };
string gg[9] = { "S.PLACE=","������Ԫʽ ","������Ԫʽ","E.PLACE=","������Ԫʽ","������Ԫʽ","T.PLACE=","F.PLACE= ","F.PLACE=" };
int grammarlength[9] = { 4,6,6,4,6,6,4,6,4 };
char grammarleft[9] = { 'S','E','E','E','T','T','T','F','F' };
string grammarright[9] = { "E","E+T","E-T","T","T*F","T/F","F","(E)","i" };
int grammarrightlength[9] = { 1,3,3,1,3,3,1,3,1 };

//�����������
#define PMAX 5 //�������ʽ���������ַ����ĳ���
int NXQ = 0;//ָʾ��һ����Ԫʽ���
int NXTemp = 1;//ָʾ��ʱ�����ı��

string eplace[10];//E����������
string tplace[10];//T����������
string fplace[10];//F����������
int eindex = -1,tindex =-1,findex = -1;
char* NewTemp(void) /*����һ����ʱ����*/
{
	char* TempID = (char*)malloc(PMAX);
	sprintf(TempID, "T%d", NXTemp++); /*���ͱ��� NXTemp ָʾ��ʱ�����ı��*/
	return TempID;
}


int lookup(char a[])/*ÿ����һ�Σ����� TOKEN �е��ַ����鱣���ֱ����鵽���ͽ���Ӧ�ؼ��ֵ�����븳�����ͱ��� c������ c ��Ϊ�㡣*/
{
	int i;
	for (i = 1; i <= 7; i++)
	{
		if (strcmp(a, table1[i]) == 0)return i;
	}
	if (i == 8)
	{
		return 0;
	}
	return 0;
}
void out(int a, const char b[])
/*һ����ڽ�����̬ʱ���ô˺��������õ���ʽΪ OUT(c��VAL)��
���У�ʵ�� c Ϊ��Ӧ���ʵ���������Ƿ���ʵ�� VAL Ϊ TOKEN�������ģ���Ϊ�մ���
���� OUT �Ĺ����ǣ����ͳ�һ�����ʵ��ڲ���ʾ֮�󣬷��ص����øôʷ�����������Ǹ�����*/
{
	ofstream fout("result.txt", fstream::app);
	char temp = ' ';
	if (a == 8 || a == 9)
	{
		fout << "(" << table2[a] << " ," << b << ")" << endl;
	}
	else
	{
		fout << "(" << table2[a] << " ," << temp << ")" << endl;
	}

	fout.close();
}

void report_error(void)//���ش���
{
	ofstream fout("result.txt", fstream::app);
	fout << "���벻�Ϲ淶��";
	fout.close();
	exit(0);
}
void scanner_example(FILE* fp)
{
	char ch;
	int i, c, flag = 0;;
	//ch = fgetc(fp);
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == ' ')ch = fgetc(fp);
		if (ch == '\n')ch = fgetc(fp);
		if (ch == '#')break;
		if (isalpha(ch)) //�ж��ַ��Ƿ�Ϊ��ĸ��ʶ���ʶ���͹ؼ���
		{
			TOKEN[0] = ch; ch = fgetc(fp); i = 1;
			while (isalnum(ch))//����������ַ��Ƿ�����ĸ������
			{
				TOKEN[i] = ch; i++;
				ch = fgetc(fp);
			}
			TOKEN[i] = '\0';//������
			fseek(fp, -1, 1); //���ļ���ǰλ�ú���һ���ַ�
			c = lookup(TOKEN);//�����ڵڶ������е�λ�ã�����ǹؼ��ַ����±꣬��ʶ������0
			if (c == 0)
			{
				out(8, TOKEN);//ʶ���ʶ��
				str[len] = 'i';
				len++;
			}
			else out(c, TOKEN);//ʶ��ؼ���
		}
		else if (isdigit(ch))//�ж��ַ��Ƿ�Ϊ�����ַ�
		{
			TOKEN[0] = ch; ch = fgetc(fp); i = 1;
			while (isdigit(ch) || ch == '.')
			{
				if (ch == '.')
				{
					if (flag == 0)
					{
						flag = 1;
						TOKEN[i] = ch; i++;
					}
					else report_error();
				}
				else
				{
					TOKEN[i] = ch; i++;

				}
				ch = fgetc(fp);
			}
			TOKEN[i] = '\0';
			fseek(fp, -1, 1);
			out(9, TOKEN);//ʶ������
			str[len] = 'i';
			len++;
		}
		else
			switch (ch)
			{
			case '<': ch = fgetc(fp);
				if (ch == '=')out(11, TOKEN);//<=
				else if (ch == '>') out(13, TOKEN);//<>
				else
				{
					fseek(fp, -1, 1);
					out(10, TOKEN);//<
				}
				break;
			case '=': ch = fgetc(fp);
				if (ch == '=')out(12, TOKEN);//==
				else
				{
					fseek(fp, -1, 1);
					out(16, TOKEN);//=
				}
				break;
			case '>': ch = fgetc(fp);
				if (ch == '=')out(15, TOKEN);//>=
				else
				{
					fseek(fp, -1, 1);
					out(14, TOKEN);//>
				}
				break;
			case '+':
			{
				out(17, TOKEN);
				str[len] = '+';
				len++;
				break;
			}
			case '-':
			{
				out(18, TOKEN);
				str[len] = '-';
				len++;
				break;
			}

			case '*':
			{
				out(19, TOKEN);
				str[len] = '*';
				len++;
				break;
			}
			case '/':
			{
				out(20, TOKEN);
				str[len] = '/';
				len++;
				break;
			}
			case '(':
			{
				out(21, TOKEN);
				str[len] = '(';
				len++;
				break;
			}
			case ')':
			{
				out(22, TOKEN);
				str[len] = ')';
				len++;
				break;
			}
			default: report_error(); break;
			}

	}
	return;
}

int main()
{
	cout << "������Ҫ�����ı��ʽ����#��������" << endl;
	char strr[50];//����ı��ʽ
	int i = 0;
	ofstream out1("demo.txt");
	out1.close();
	ofstream out("demo.txt", ios::out);
	while (true)//�����ʽ�����ļ�test
	{
		cin >> strr[i];
		out << strr[i];
		if (strr[i] == '#')break;
		i++;
	}
	out.close();
	FILE* fp1;
	FILE* fp2;
	fp2 = fopen("result.txt", "w");
	fclose(fp2);
	fp1 = fopen("demo.txt", "r");
	if (!fp1)
	{
		printf("���ļ�ʧ�ܣ�");
	}
	scanner_example(fp1);//��test�ļ��е����ݽ��дʷ�����������result�ļ�

	string temp;
	cout << endl;
	cout << "�ʷ��������Ϊ��" << endl;
	ifstream myfile2("result.txt", ios::in);
	if (!myfile2.is_open())
	{
		cout << "δ�ɹ����ļ�" << endl;
	}
	while (getline(myfile2, temp))//���rusult�ļ��е�����
	{
		cout << temp << endl;
	}
	myfile2.close();

	str[len] = '#';
	cout << endl;
	cout << "�ʷ���������ʽΪ��" << endl;
	for (int i = 0; i <= len; i++)
	{
		cout << str[i];
	}
	cout << endl;
	cout << endl;
	cout << "�﷨�������Ϊ��" << endl;

	int count;//����
	int state[100], stateIndex;//״̬�����±�
	state[0] = 0; stateIndex = 0;//��ʼ��״̬��
	char inSymbol[100], outSymbol[100];//ջ�з��Ŵ�  �������Ŵ�
	inSymbol[0] = '#';//��ʼ��
	int inSymbolIndex = 0, outSymbolIndex = 0;
	string analysis;//��������
	//string nextState;//��һ״̬
	for (int i = 0; i <= len; i++)
	{
		outSymbol[i] = str[i];
	}
	cout << "�ַ�������Ϊ��" << len << endl;
	cout << "����" << "\t\t" << "״̬" << "\t\t\t\t\t";
	cout << "ջ�з���" << "\t\t\t" << "�������Ŵ�" << "\t\t\t\t";
	cout << "��������" << "\t\t" << "���崦��" << "\t" << "�����м����" << "\t\t\n";
	for (count = 1;;)
	{
		cout << count << fixed << setprecision(3) << "\t\t";
		for (int i = 0; i <= stateIndex; i++)//���״̬��
		{
			cout << state[i] << " " << fixed << setprecision(4);
		}
		cout << "\t\t\t\t\t";
		for (int i = 0; i <= inSymbolIndex; i++)//���ջ�з��Ŵ�
		{
			cout << inSymbol[i] << " " << fixed << setprecision(4);
		}
		cout << "\t\t\t\t";
		for (int i = outSymbolIndex; i <= len; i++)//����������Ŵ�
		{
			cout << outSymbol[i] << " " << fixed << setprecision(4);
		}
		cout << "\t\t\t";


		int row = state[stateIndex], column;//����״̬�����������Ŵ���action��
		if (outSymbol[outSymbolIndex] == '(')column = 0;
		else if (outSymbol[outSymbolIndex] == ')')column = 1;
		else if (outSymbol[outSymbolIndex] == '+')column = 2;
		else if (outSymbol[outSymbolIndex] == '-')column = 3;
		else if (outSymbol[outSymbolIndex] == '*')column = 4;
		else if (outSymbol[outSymbolIndex] == '/')column = 5;
		else if (outSymbol[outSymbolIndex] == 'i')column = 6;
		else if (outSymbol[outSymbolIndex] == '#')column = 7;


		if (action[row][column] == "null")
		{
			cout << "error!" << endl;
			break;
		}
		else//���Ϊ��
		{
			analysis = action[row][column];
			cout << analysis << "\t\t\t";//�����������
			if (analysis[0] == 's')//�鵽��s��ͷ�ķ�������
			{
				cout << endl;
				stateIndex++;
				if (analysis.length() == 3)
				{
					state[stateIndex] = ((int)(analysis[1]) - 48) * 10 + ((int)(analysis[2]) - 48);
				}
				else state[stateIndex] = (int)analysis[1] - 48;
				inSymbolIndex++;
				inSymbol[inSymbolIndex] = outSymbol[outSymbolIndex];
				outSymbolIndex++;
				count++;
			}
			else if (analysis[0] == 'r')//�鵽��r��ͷ�ķ�������
			{
				int temp = (int)analysis[1] - 48;
				cout << grammar[temp] << endl;
				stateIndex = stateIndex - grammarrightlength[temp];
				inSymbolIndex = inSymbolIndex + 1 - grammarrightlength[temp];
				inSymbol[inSymbolIndex] = grammarleft[temp];
				cout << " " << fixed << setprecision(3) << "\t\t";
				for (int i = 0; i <= stateIndex; i++)//���״̬��
				{
					cout << state[i] << " " << fixed << setprecision(4);
				}
				cout << "\t\t\t\t\t";
				for (int i = 0; i <= inSymbolIndex; i++)//ջ�з��Ŵ�
				{
					cout << inSymbol[i] << " " << fixed << setprecision(4);
				}
				cout << "\t\t\t\t";
				for (int i = outSymbolIndex; i <= len; i++)//�������Ŵ�
				{
					cout << outSymbol[i] << " " << fixed << setprecision(4);
				}
				cout << "\t\t\t";


				int row2 = state[stateIndex], column2;//����״̬����ջ�з��Ŵ���go��
				if (inSymbol[inSymbolIndex] == 'E')column2 = 0;
				else if (inSymbol[inSymbolIndex] == 'T')column2 = 1;
				else if (inSymbol[inSymbolIndex] == 'F')column2 = 2;

				if (go[row2][column2] == -1)
				{
					cout << "error!" << endl;
					break;
				}
				else//��Ϊ�գ����
				{
					cout << "go[" << row2 << "," << column2 << "]=" << go[row2][column2] << "\t\t\t\t";
					stateIndex++;
					state[stateIndex] = go[row2][column2];
					count++;
					// ����Ϊ�����������
					if (temp == 8)
					{
						findex++;
						cout << gg[temp] << strr[outSymbolIndex - 1];
						fplace[findex] = strr[outSymbolIndex - 1];
						
					}
					else if(temp==6)
					{
						tindex++;
						cout << gg[temp] << strr[outSymbolIndex - 1];
						tplace[tindex] = strr[outSymbolIndex - 1];
						
						findex--;
					}
					else if (temp == 3)
					{
						eindex++;
						cout << gg[temp] << strr[outSymbolIndex - 1];
						eplace[eindex] = strr[outSymbolIndex - 1];
						
						tindex--;
					}
					else if (temp == 1)
					{
						cout << gg[temp] ;
						
						const char* temp1 = eplace[eindex].c_str();
						const char* temp2 = tplace[tindex].c_str();					
						tindex--;
						char* temp=NewTemp();
						//GEN("+", temp1, temp2, temp);
						cout << "( + , " << temp1 << " , " << temp2 << " , " << temp << " ) ";
						eplace[eindex] = NXTemp;

					}
					else if (temp == 2)
					{
						cout << gg[temp] ;
						
						const char* temp1 = eplace[eindex].c_str();
						const char* temp2 = tplace[tindex].c_str();
						tindex--;
						char* temp = NewTemp();
						//GEN("-", temp1, temp2, temp);
						cout << "( - , " << temp1 << " , " << temp2 << " , " << temp << " ) ";
						eplace[eindex] = temp;
					}
					else if (temp == 4)
					{
						cout << gg[temp] ;
						const char* temp1 = tplace[tindex].c_str();
						const char* temp2 = fplace[findex].c_str();
						findex--;
						char* temp = NewTemp();
						//GEN("*", temp1, temp2, temp);
						cout << "( * , " << temp1 << " , " << temp2 << " , " << temp << " ) ";
						tplace[tindex] = temp;
					}
					else if (temp == 5)
					{
						cout << gg[temp] ;
						const char* temp1 = tplace[tindex].c_str();
						const char* temp2 = fplace[findex].c_str();
						findex--;
						char* temp = NewTemp();
						//GEN("/", temp1, temp2, temp);
						cout << "( / , " << temp1 << " , " << temp2 << " , " << temp << " ) ";
						tplace[tindex] = temp;
					}
					else if (temp == 7)
					{
						findex++;
						fplace[findex] = eplace[eindex];
						eindex--;
					}
					cout << endl;
				}
			}
			else if (analysis == "acc")
			{
				cout << endl << "�����ɹ���";
				break;
			}

		}
	}
	
	fclose(fp1);
	fclose(fp2);
	return 0;
}
