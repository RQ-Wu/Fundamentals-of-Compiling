#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define STATE char // state is a char type
#define VALUE char // value is a char type
#define CLUSTER vector<STATE> // cluster is a set of states

using namespace std;

typedef struct nfaState{
    STATE headState;
    STATE tailState;
    VALUE processingValue;
}nfaState,dfaState;

//string 2 nfa
vector<nfaState> nfaInit(){
    vector<nfaState> nfa;
    string s;
    ifstream fin(R"(F:\nfa2dfa\connect\nfa.txt)");
    if (fin.is_open()){
        while (!fin.eof()){
            getline(fin, s);
            STATE headState, tailState, processingState;
            int length = s.length();
            int scaner = 0;
            headState = s[scaner];
            length -= 2;
            scaner += 2;
            while (length > 0){
                nfaState temp;
                tailState = s[scaner + 5];
                processingState = s[scaner + 2];

                temp.headState = headState;
                temp.processingValue = processingState;
                temp.tailState = tailState;

                nfa.push_back(temp);

                length -= 7;
                scaner += 7;
            }
        }
    }

    return nfa;
}

// get list(states or values)
// get value: false for states | true for values
CLUSTER getIndexState(const vector<nfaState>& nfa, bool get_value = false){
    CLUSTER states;

    // get values or states
    if(get_value){
        for(auto & i : nfa){
            states.push_back(i.processingValue);
        }
    } else{
        for(auto & i : nfa){
            states.push_back(i.headState);
            states.push_back(i.tailState);
        }
    }

    //eliminate duplicate elements
    sort(states.begin(),states.end());
    states.erase(unique(states.begin(), states.end()), states.end());

    return states;
}

//find all states that state 's' can reach via value 'v'
CLUSTER findNfa(const vector<nfaState>& nfa, STATE s, VALUE v){
    CLUSTER res; // result for all states can reach
    for (auto & i : nfa){
        if (i.headState == s && i.processingValue == v)
            res.push_back(i.tailState);
    }
    return res;
}

// get epsilon-cluster for state 's'
void getEpsilonCluster(const vector<nfaState>& nfa, CLUSTER &cluster, STATE s){
    CLUSTER res = findNfa(nfa,s,'~');
    cluster.push_back(s);
    cluster.insert(cluster.end(), res.begin(), res.end());

    for (STATE & c : res){
        getEpsilonCluster(nfa, cluster, c);
    }
    if (res.size() == 0){
        sort(cluster.begin(), cluster.end());
        cluster.erase(unique(cluster.begin(), cluster.end()), cluster.end());

        return;
    }
}

// move operation
CLUSTER move(const vector<nfaState>& nfa, CLUSTER c, VALUE v){
    CLUSTER res;
    for (STATE & s : c) {
        CLUSTER temp = findNfa(nfa, s, v);
        res.insert(res.end(), temp.begin(), temp.end());
    }
    sort(res.begin(),res.end());
    res.erase(unique(res.begin(), res.end()), res.end());

    return res;
}

CLUSTER joinCluster(vector<CLUSTER> epsilonClusters, CLUSTER clusterMove){
    CLUSTER joinRes;
    for(STATE & s : clusterMove){
        if (s == 'X')
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[epsilonClusters.size() - 2].begin(),
                    epsilonClusters[epsilonClusters.size() - 2].end()
            );
        else if (s == 'Y')
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[epsilonClusters.size() - 1].begin(),
                    epsilonClusters[epsilonClusters.size() - 1].end()
            );
        else
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[s - '0'].begin(),
                    epsilonClusters[s - '0'].end()
            );
    }
    sort(joinRes.begin(),joinRes.end());
    joinRes.erase(unique(joinRes.begin(),joinRes.end()),joinRes.end());

    return joinRes;
}

// name start state to 'X', name terminate state to 'Y'
vector<dfaState> renameDFA(vector<dfaState> dfa, STATE start, STATE terminate){
    for (dfaState &d : dfa){
        // rename head state
        if (d.headState == start) d.headState = 'X';
        else if (d.headState == terminate) d.headState = 'Y';
        else d.headState--;
        // rename tail state
        if (d.tailState == start) d.tailState = 'X';
        else if (d.tailState == terminate) d.tailState = 'Y';
        else d.tailState--;
    }
    return dfa;
}

//output dfa with specific format
void outputDFA(vector<dfaState> dfa, int stateNum){
    ofstream fout(R"(F:\\nfa2dfa\\connect\\dfa.txt)");
    // output start state first
    fout << "X";
    for (dfaState &d : dfa){
        if (d.headState == 'X')
            fout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
    }
    fout << "\n";
    // output terminate state then
    fout << "Y";
    for (dfaState &d : dfa){
        if (d.headState == 'Y')
            fout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
    }
    fout << "\n";
    // output other state
    for (int i = 0; i < stateNum; ++i) {
        fout << i;
        for (dfaState &d : dfa){
            if (d.headState == '0' + i)
                fout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
        }
        fout << "\n";
    }
}
void NFA2DFA(){
    vector<dfaState> dfa;
    vector<nfaState> nfa = nfaInit();
    vector<STATE> stateList = getIndexState(nfa);
    vector<VALUE> valueList = getIndexState(nfa, true);

    // get all epsilon-cluster
    vector<CLUSTER> epsilonClusters;
    for(STATE &s : stateList){
        CLUSTER temp;
        getEpsilonCluster(nfa, temp, s);
        epsilonClusters.push_back(temp);
    }

    // build DFA
    vector<CLUSTER> newCluster;
    newCluster.push_back(epsilonClusters[epsilonClusters.size() - 2]);
    unsigned int i = 0;
    while(i < newCluster.size()){
        for(VALUE & v : valueList){
            if (v == '~') continue; // ignore value epsilon

            CLUSTER clusterMove = move(nfa, newCluster[i], v);
            if (clusterMove.empty()) continue; // ignore empty cluster

            CLUSTER clusterTemp = joinCluster(epsilonClusters, clusterMove);
            // if temp does not exist in newCluster
            if (!count(newCluster.begin(), newCluster.end(), clusterTemp)){
                newCluster.push_back(clusterTemp);     //create new state in newCluster
                dfaState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + newCluster.size() - 1;
                dfa.push_back(dfaTemp);
            } else {
                int position = distance(
                        newCluster.begin(),
                        find(newCluster.begin(), newCluster.end(), clusterTemp)
                );
                dfaState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + position;
                dfa.push_back(dfaTemp);
            }
        }
        i++;
    }
    // rename the state of DFA
    STATE start = '0';
    STATE terminate = '0' + newCluster.size() - 1;
    dfa = renameDFA(dfa, start, terminate);

    int stateNum = newCluster.size() - 2;
    outputDFA(dfa, stateNum);

}
int main(){
    NFA2DFA();
    return 0;
}
