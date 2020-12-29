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
	//输入的表达式的长度
	n1 = strlen(prefixExpression);
	//添加连接符后的表达式及长度
	char* pfe = preprocess(prefixExpression, n1 + 1);
	n2 = strlen(pfe);
	//cout << "添加连接符后的表达式为:" << pfe << endl;
	//cout << "添加连接符后的长度为:"<<n2 << endl;
	//将前缀表达式转化为后缀表达式
	char* rpn = toRPN(pfe, n2 + 1);
	//cout << "后缀表达式为:" << rpn << endl;
	toNFA(rpn);
	delete [] prefixExpression;
	free(pfe);
	free(rpn);
	prefixExpression = NULL;
	pfe = NULL;
	rpn = NULL;
}