#include"toRPN.h"
#include<iostream>
#include<cstring>
#include<stack>
using namespace std;

char* preprocess(char* prefixExpression, int size) {
	char* str = new char[size * 2];
	int i, j;
	for (i = 0, j = 0; i < size; i++, j++) {
		str[j] = prefixExpression[i];
		if ((prefixExpression[i] >= 'a' && prefixExpression[i] <= 'z') || (prefixExpression[i] >= '0' && prefixExpression[i] <= '9') || prefixExpression[i] == '*' || prefixExpression[i] == ')') {
			if ((prefixExpression[i + 1] >= 'a' && prefixExpression[i + 1] <= 'z') || (prefixExpression[i + 1] >= '0' && prefixExpression[i + 1] <= '9') || prefixExpression[i + 1] == '(') {
				str[j + 1] = '&';
				j++;
			}
		}
	}
	for (j + 1; j < size * 2; j++) {
		str[j] = 0;
	}
	return str;
}

int priosity(char ch) {

	if (ch == '*')
	{
		return 3;
	}

	if (ch == '&')
	{
		return 2;
	}

	if (ch == '|')
	{
		return 1;
	}

	if (ch == '(')
	{
		return 0;
	}
}

char* toRPN(char* pfe, int size) {
	stack<char> operation;
	char* str = new char[size + 1];
	int i, j;
	for (i = 0, j = 0; (i < size) && (j < size); i++) {
		if ((pfe[i] >= 'a' && pfe[i] <= 'z') || (pfe[i] >= '0' && pfe[i] <= '9')) {
			str[j] = pfe[i];
			j++;
		}
		else {
			//如果栈空或运算符为左括号，则直接进栈
			if (operation.empty() || pfe[i] == '(')
				operation.push(pfe[i]);
			//否则比较运算符与栈顶元素的优先级
			else {
				while (!operation.empty() && (pfe[i] != ')') && (priosity(operation.top()) >= priosity(pfe[i]))) {
					//把优先级大于等于该运算符的栈内元素出栈赋给结果字符串
					str[j] = operation.top();
					j++;
					operation.pop();
				}
				//当出栈操作完成后，该运算符进栈，但是')'不进栈
				if (pfe[i] != ')')
					operation.push(pfe[i]);
				else {
					while ((j < size) && (operation.top() != '(')) {
						str[j] = operation.top();
						j++;
						operation.pop();
					}
					operation.pop();
				}
			}
		}
	}
	if (!operation.empty()) {
		str[j] = operation.top();
		j++;
		operation.pop();
	}
	for (j + 1; j < size; j++) {
		str[j] = 0;
	}
	return str;
}