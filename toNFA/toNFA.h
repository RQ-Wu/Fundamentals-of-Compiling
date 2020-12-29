#pragma once
struct NFAState
{
	int state;		//该NFAState的状态值
	char value;		//NFA状态弧上的值
	int state_next;		//通过value连接的下一个状态的值
};

struct NFA{
	NFAState* head;		//NFA节点的头指针
	NFAState* tail;		//NFA节点的尾指针
};

void toNFA(char* str);