#include "nfa2dfa.cpp"
#include "dfa_simplify.cpp"
#include "toNFA/main.cpp"
#include "wordRecognition.cpp"

using namespace std;

int main(){
    cout << "input the regular experssion: " << endl;
    RE2NFA();
    NFA2DFA();
    dfa_simplify();
    cout << "input the word to recognize: " << endl;
    wordRecognition();

    return 0;
}


