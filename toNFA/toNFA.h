#pragma once
struct NFAState
{
	int state;		//��NFAState��״ֵ̬
	char value;		//NFA״̬���ϵ�ֵ
	int state_next;		//ͨ��value���ӵ���һ��״̬��ֵ
};

struct NFA{
	NFAState* head;		//NFA�ڵ��ͷָ��
	NFAState* tail;		//NFA�ڵ��βָ��
};

void toNFA(char* str);