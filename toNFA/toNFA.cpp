#include<iostream>
#include<cstring>
#include<stack>
#include<vector>
#include"toNFA.h"
#include<iomanip>
#include<fstream>

using namespace std;

int NFAStateNum = -1;
vector<NFA>nfa;
NFA nfaStruct;

void initiate(NFA& nfaStruct) {
	nfaStruct.head = (NFAState*)malloc(sizeof(NFAState));
	nfaStruct.tail = (NFAState*)malloc(sizeof(NFAState));
}

void toNFA(char* str) {
	int i, j;
	for (i = 0; i < strlen(str); i++) {
		initiate(nfaStruct);
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9')) {
			nfaStruct.head->state = NFAStateNum + 1;
			nfaStruct.head->state_next = NFAStateNum + 2;
			nfaStruct.head->value = str[i];
			nfaStruct.tail->state = NFAStateNum + 2;
			nfaStruct.tail->state_next = -1;
			nfaStruct.tail->value = '#';
			if (nfa.size() > 0) {
				nfa[nfa.size() - 1].tail->state_next = nfaStruct.head->state;
			}
			nfa.push_back(nfaStruct);
			NFAStateNum += 2;
		}
		else if (str[i] == '*') {
			j = nfa.size() - 1;
			nfaStruct.head->state = nfa[j].head->state;
			nfaStruct.tail->state = nfa[j].tail->state;

			while ((j >= 0) && (nfa[j].head->state == nfaStruct.head->state) && (nfa[j].tail->state == nfaStruct.tail->state)) {
				nfa[j].head->state = nfa[j].tail->state;
				nfa[j].tail->value = '~';
				j--;
			}
			nfaStruct.head->state = nfa[j + 1].head->state - 1;
			nfaStruct.head->state_next = nfa[j + 1].head->state;
			nfaStruct.tail->state_next = nfa[j + 1].tail->state;
			nfaStruct.tail->value = nfa[j + 1].head->value;
			nfaStruct.head->value = '~';

			j++;
			nfa.insert(nfa.begin() + j, nfaStruct);
			initiate(nfaStruct);

			nfaStruct.head->state = nfa[j].tail->state;
			nfaStruct.head->state_next = nfa[j].tail->state + 1;
			nfaStruct.tail->state = nfa[j].tail->state + 1;
			nfaStruct.tail->value = '#';
			nfaStruct.head->value = '~';
			nfa.insert(nfa.begin() + j + 1, nfaStruct);

			NFAStateNum++;
		}
		else if (str[i] == '|') {
			//cout <<""<< nfa.back().head->state << endl;
			nfa[nfa.size() - 1].head->state -= 2;
			nfa[nfa.size() - 1].head->state_next -= 2;
			nfa[nfa.size() - 1].tail->state -= 2;

			NFAStateNum -= 2;
		}
		else if (str[i] == '&') {
			j = nfa.size() - 1;
			nfaStruct = nfa[j];
			while (j > 0) {
				if ((nfa[j - 1].tail->value == '#') && (nfa[j - 1].head->state_next != NFAStateNum)) {
					nfa[j - 1].tail->value = nfaStruct.head->value;
					nfa[j - 1].tail->state_next = nfaStruct.head->state - 1;
					break;
				}
				j--;
			}
			if (j != 0) {
				nfaStruct = nfa[j - 1];
				while (j < nfa.size()) {
					if (nfa[j].head->state > nfaStruct.tail->state) {
						nfa[j].head->state--;
						nfa[j].head->state_next--;
						nfa[j].tail->state--;
						if (nfa[j].tail->value != '#')
							nfa[j].tail->state_next--;
					}
					j++;
				}
			}
			else {
				j = nfa.size() - 1;
				nfa[j].head->state--;
				nfa[j].head->state_next--;
				nfa[j].tail->state--;
			}

			NFAStateNum--;
		}
		/*cout << "vector is : ";
		for (int j = 0; j < nfa.size(); j++) {
			cout << nfa[j].head->state << " ";
		}
		cout << endl;
		*/
	}

	//输出结果写入文件
	ofstream f(R"(F:\nfa2dfa\connect\temp.txt)");
	if (!f.is_open()) {
		cout << "file open faied" << endl;
		return;
	}
	
	i = 0;
	f << 'X' << " ";
	while ((i < nfa.size()) && (nfa[i].head->state == 0)) {
		f << 'X' << "-" << nfa[i].head->value << "->" << nfa[i].tail->state - 1;
		f << " ";
		i++;
	}
	f << endl;

	//输出终止态
	f << 'Y' << endl;

	//输出过程态,遍历nfa，找到头状态为0-NFAStateNum的nfaState
	//循环输出状态后的nfaState
	j = i;
	for (i = 1; i < NFAStateNum; i++) {
		f << i - 1 << " ";
		while ((j < nfa.size()) && (nfa[j].head->state == i)) {
			if (nfa[j].tail->state == NFAStateNum) {
				f << i - 1 << "-" << nfa[j].head->value << "->" << 'Y';
			}
			else {
				f << i - 1 << "-" << nfa[j].head->value << "->" << nfa[j].tail->state - 1;
			}
			j++;
			f << " ";
		}
		f << endl;
	}

		f.close();

	/*
	//输出起始态，遍历nfa找头状态为0的nfaState
	i = 0;
	cout << 'X' << " ";
	while ((i < nfa.size()) && (nfa[i].head->state == 0)) {
		cout << 'X' << "-" << nfa[i].head->value << "->" << nfa[i].tail->state - 1;
		cout << " ";
		i++;
	}
	cout << endl;

	//输出终止态
	cout << 'Y' << endl;

	//输出过程态,遍历nfa，找到头状态为0-NFAStateNum的nfaState
	//循环输出状态后的nfaState
	j = i;
	for (i = 1; i < NFAStateNum; i++) {
		cout << i - 1 << " ";
		while ((j < nfa.size()) && (nfa[j].head->state == i)) {
			if (nfa[j].tail->state == NFAStateNum) {
				cout << i - 1 << "-" << nfa[j].head->value << "->" << 'Y';
			}
			else {
				cout << i - 1 << "-" << nfa[j].head->value << "->" << nfa[j].tail->state - 1;
			}
			j++;
			cout << " ";
		}
		cout << endl;

*/
	for (i = 0; i < nfa.size(); i++) {
		free(nfa[i].head);
		free(nfa[i].tail);
		nfa[i].head = NULL;
		nfa[i].tail = NULL;
	}
	
}

