#include<iostream>
#include<list>
#include<vector>
using namespace std;
class DFA_State
{
  public:
    char name;
    vector<char> accept;
    vector<char> transferList;
    void print(){
      int len = accept.size();
      if(accept.size() > 0){
        cout<<name<<" ";
      }else{
        cout<<name<<endl;
      }
      for(int i = 0; i < len; i++){
        if(i < len - 1){
          cout<<name<<"-"<<accept[i]<<"->"<<transferList[i]<<" ";
        }else{
          cout<<name<<"-"<<accept[i]<<"->"<<transferList[i]<<endl;
        }
      }
    }
};

int main(){
  vector<DFA_State> dfaStateList;
  string ss;
  //读入所有状态
	while(getline(cin, ss)){
    if(ss.size() == 0){
      break;
    }
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
  
  for(int i = 0; i < dfaStateList.size(); i++){
    dfaStateList[i].print();
  }
}