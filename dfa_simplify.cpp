#include<iostream>
#include <fstream>
#include<list>
#include<vector>
using namespace std;
class DFA_State
{
public:
    char name{};
    vector<char> accept;
    vector<char> transferList;
    string print(){
        int len = accept.size();
        string str = "";

        if(accept.size() > 0){
            str = str + name +" ";
            //out<<name<<" ";
        }else{
            str = str + name + "\n";
            //out<<name<<"\n";
        }
        for(int i = 0; i < len; i++){
            if(i < len - 1){
                str = str + name + "-" + accept[i] + "->" + transferList[i] +" ";
                //out<<name<<"-"<<accept[i]<<"->"<<transferList[i]<<" ";
            }else{
                str = str + name + "-" + accept[i] + "->" + transferList[i] + "\n";
                //out<<name<<"-"<<accept[i]<<"->"<<transferList[i]<<"\n";
            }
        }
        return str;
    }
};
void dfa_simplify(){
    vector<DFA_State> dfaStateList;
    string ss;
    //读入所有状态
    ifstream in(R"(F:\nfa2dfa\connect\temp.txt)");
    while(getline(in, ss)){
        if(ss.size() == 0){
            break;
        }
//        cout << ss << endl;
        DFA_State dfa_State;
        dfa_State.name = ss[0];
        int flag = 0;
        int flag1 = 0;
        for(int i = 1; i < ss.length(); i++){
            //标志没有读入状态符
            if(ss[i] != ' ' && ss[i] != '>' && ss[i] != '-'){
                if(ss[i] == ss[0] && flag1 == 0){
                    flag1 = 1;
                }
                else if(flag == 0 && flag1 == 1){
                    dfa_State.accept.push_back(ss[i]);
                    flag = 1;
                }else if(flag == 1 && flag1 ==1){
                    dfa_State.transferList.push_back(ss[i]);
                    flag = 0;
                    flag1 = 0;
                }
            }
        }
        dfaStateList.push_back(dfa_State);
    }
    in.close();
    //剔除相同状态点
    char save[20];
    char del[20];
    int k = 0;
    for(int i = 0; i < dfaStateList.size(); i++){
        for(int j = i + 1; j < dfaStateList.size(); j++){
            if(dfaStateList[i].accept == dfaStateList[j].accept && dfaStateList[i].transferList == dfaStateList[j].transferList && i != 1 && j != 1){
                save[k] = dfaStateList[i].name;
                del[k] = dfaStateList[j].name;
                k++;
            }
        }
    }

    for(int i = 0; i < k; i++){
        for(int j = 0; j < dfaStateList.size(); j++){
            if(dfaStateList[j].name == del[i]){
                dfaStateList.erase(dfaStateList.begin() + j);
            }
            for(int t = 0; t < dfaStateList[j].transferList.size(); t++){
                if(dfaStateList[j].transferList[t] == del[i]){
                    dfaStateList[j].transferList[t] = save[i];
                    t = t - 1;
                }
            }
        }
    }
    string acc = "";
    string state = "";
    string str = "";
    vector<char> a;
    for (int n = 0; n < dfaStateList.size(); ++n) {
        for(int i = 0; i< dfaStateList[n].accept.size(); i++){
            a.push_back(dfaStateList[n].accept[i]);
        }
    }
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    for(int i = 0; i< a.size(); i++){
        acc = acc + a[i] + " ";
    }
    for(int i = 0; i < dfaStateList.size(); i++){
        str += dfaStateList[i].print();
        state = state + dfaStateList[i].name + " ";
        //dfaStateList[i].print();
    }
    acc += "#\n";
    state += "#\n";
    str = acc + state + str;
    ofstream out(R"(F:\nfa2dfa\connect\temp.txt)");
    out<<str;
}