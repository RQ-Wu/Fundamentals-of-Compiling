#include<iostream>
#include<cstring>
#include<stack>
#include<vector>
#include"toRPN.h"
#include"toNFA.h"
using namespace std;




void RE2NFA() {
	char* prefixExpression = new char[100];
	int n1, n2;
	cin >> prefixExpression;
	getchar();
	//cout << prefixExpression;
	//����ı��ʽ�ĳ���
	n1 = strlen(prefixExpression);
	//������ӷ���ı��ʽ������
	char* pfe = preprocess(prefixExpression, n1 + 1);
	n2 = strlen(pfe);
	//cout << "������ӷ���ı��ʽΪ:" << pfe << endl;
	//cout << "������ӷ���ĳ���Ϊ:"<<n2 << endl;
	//��ǰ׺���ʽת��Ϊ��׺���ʽ
	char* rpn = toRPN(pfe, n2 + 1);
	//cout << "��׺���ʽΪ:" << rpn << endl;
	toNFA(rpn);
	delete [] prefixExpression;
	free(pfe);
	free(rpn);
	prefixExpression = NULL;
	pfe = NULL;
	rpn = NULL;
}