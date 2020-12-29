#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

vector<string> split(const string &str, const string &Separator);
//a b#
//X Y 0#
//X X-0->0
//Y Y-1->Y
//0 0-1->Y
//
//011#
//10#

// a b#
// X Y 0 2#
// X X-a->0 X-b->X
// Y Y-a->0 Y-b->X
// 0 0-a->0 0-b->2
// 2 2-a->0 2-b->Y

// abb#
// ba#
// aca#

void wordRecognition() {
    ifstream testfile(R"(F:\nfa2dfa\connect\temp.txt)");
    ofstream outfile(R"(F:\nfa2dfa\connect\output.txt)");
    string line1, line2;
    getline(testfile, line1);
    int line1len = line1.length();
    vector<char> validChar(line1len / 2);
    for (int i = 0; i < line1len; i += 2)
        validChar[i / 2] = line1[i];
    getline(testfile, line2);
    int line2len = line2.length();
    vector<char> allState(line2len / 2);
    for (int i = 0; i < line2len; i += 2)
        allState[i / 2] = line2[i];
    int vcLen = validChar.size();
    int asLen = allState.size();
    char **transitionFunc = new char *[asLen];
    for (int i = 0; i < asLen; i++) {
        transitionFunc[i] = new char[vcLen];
        for (int j = 0; j < vcLen; j++) {
            transitionFunc[i][j] = '\0';
        }
        string newLine;
        getline(testfile, newLine);
        vector<string> res = split(newLine, " ");
        if (res.size() >= 2 && res[1][2] == '0') {
            validChar[0] = '0';
            validChar[1] = '1';
        }
        for (int j = 1; j < res.size(); ++j) {
            for (int k = 0; k < vcLen; k++) {
                if (validChar[k] == res[j][2]) {
                    transitionFunc[i][k] = res[j][5];
                    break;
                }
            }
        }
//        for (int i = 0; i < sizeof(transitionFunc) / sizeof(transitionFunc[0]); ++i) {
//            for (int j = 0; j < sizeof(transitionFunc[0]) / sizeof(char); ++j) {
//                cout << transitionFunc[i][j] << " ";
//            }
//            cout << "\n";
//        }
    }
    testfile.close();
    string str;
    while (getline(cin, str)) {
        int flag = 1;
        if (str.size() == 0)
            return;
        char state = 'X';
        int strLen = str.length();
        for (int i = 0; i < strLen; i++) {
            char input = str[i];
            if (input == '#') {
                if (state == 'Y') {
                    cout << "pass" << endl;
                    outfile << "pass" << endl;
                } else {
                    cout << "error" << endl;
                    outfile << "error" << endl;
                }
            } else {
                int j = 0;
                for (j = 0; j < vcLen; j++) {
                    if (input == validChar[j]) {
                        int k;
                        for (k = 0; k < asLen; k++) {
                            if (state == allState[k]) {
                                break;
                            }
                        }
                        if (!(transitionFunc[k][j] - '\0')) {
                            flag = 0;
                            break;
                        }
                        outfile << input << endl;
                        state = transitionFunc[k][j];
                        break;
                    }
                }
                if (j == vcLen || flag == 0) {
                    outfile << "e" << endl;
                    break;
                }
            }
        }
    }
}

vector<string> split(const string &str, const string &Separator)//用于分隔字符串
{
    vector<string> res;
    if ("" == str)//如果为空
        return res;
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[Separator.length() + 1];
    strcpy(d, Separator.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }
    return res;
}
