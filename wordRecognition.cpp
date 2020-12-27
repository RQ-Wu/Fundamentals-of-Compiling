//
// Created by 84320 on 2020/12/19.
//

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

ifstream infile;
ofstream outfile;
map<string, string> tokens;        //存储单词种别码
string line;                    //读取一行字符串
string first[32] = {"const", "int", "char", "void", "main", "if", "else", "do", "while", "for", "scanf", "printf", "return",
                "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "=", ";", ",", "(", ")", "[", "]", "{", "}"};

string second[32] = {"CONSTTK", "INTTK", "CHARTK", "VOIDTK", "MAINTK", "IFTK", "ELSETK", "DOTK", "WHILETK", "FORTK",
                "SCANFTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU", "MULT", "DIV", "LSS", "LEQ", "GRE", "GEQ", "EQL",
                "NEQ", "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE"};


void init()     //初始化，去掉多余的空格，注释
{
    for (int i = 0; i < 32; i++)
        tokens.insert(make_pair(first[i], second[i]));
}

bool is_digit(char c)   //判断是否是数字
{
    if (c <= '9' && c >= '0')
        return true;
    else
        return false;
}

bool is_alpbat(char c)  //判断是否字母    
{
    if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
        return true;
    else
        return false;
}

string is_oprator(int &i) {  //判断是否操作符
    string code;
    switch (line[i]) {
        case '+':
            code = "PLUS";
            break;
        case '-':
            code = "MINU";
            break;
        case '*':
            code = "MULT";
            break;
        case '/':
            if (line[i + 1] == '/') {
                code = "//";
                i = i + 1;
            } else {
                code = "DIV";
            }

            break;
        case '<':
            if (line[i + 1] == '=') {
                code = "LEQ";
                i = i + 1;
            } else {
                code = "LSS";

            }

            break;
        case '>':
            if (line[i + 1] == '=') {
                code = "GEQ";
                i = i + 1;
            } else {
                code = "GRE";

            }
            break;
        case '=':
            if (line[i + 1] == '=') {
                code = "EQL";
                i = i + 1;
            } else {
                code = "ASSIGN";
            }

            break;
        case '!':
            if (line[i + 1] == '=') {
                code = "NEQ";
                i = i + 1;
            }
            break;
        default:
            code = " ";
    }
    return code;
}//判断是否运算符

string is_range(int &i) {
    string code;
    switch (line[i]) {
        case ';':
            code = "SEMICN";
            break;
        case ',':
            code = "COMMA";
            break;
        case '(':
            code = "LPARENT";
            break;
        case ')':
            code = "RPARENT";
            break;
        case '[':
            code = "LBRACK";
            break;
        case ']':
            code = "RBRACK";
            break;
        case '{':
            code = "LBRACE";
            break;
        case '}':
            code = "RBRACE";
            break;
        default:
            code = " ";
            break;
    }
    return code;
}//判断是否界符


string alpbat_c(int &i) {   //判断c打头的保留字
    if (line[i + 1] == 'h' && line[i + 2] == 'a' && line[i + 3] == 'r' && line[i + 4] == ' ') {
        i = i + 4;
        return "CHARTK";
    } else if (line[i + 1] == 'o' && line[i + 2] == 'n' && line[i + 3] == 's' && line[i + 4] == 't' &&
               line[i + 5] == ' ') {
        i = i + 5;
        return "CONSTTK";
    } else return " ";
}


string alpbat_i(int &i) {   //判断i打头的保留字
    if (line[i + 1] == 'n' && line[i + 2] == 't' && line[i + 3] == ' ') {
        i = i + 3;
        return "INTTK";
    } else if (line[i + 1] == 'f' && !is_alpbat(line[i + 2]) && !is_digit(line[i + 2]) && line[i + 2] != '_') {
        i = i + 2;
        return "IFTK";
    } else return " ";
}

string find_key(string s) { //查找s的key值
    map<string, string>::iterator iter;
    for (iter = tokens.begin(); iter != tokens.end(); iter++) {
        if (iter->second == s)
            break;
    }
    return iter->first;
}


void recog_reserve(int &i, string &code) {  //识别保留字
    code = " ";
    switch (line[i]) {
        case 'c':
            code = alpbat_c(i);
            break;
        case 'd':
            if (line[i + 1] == 'o' && !is_alpbat(line[i + 2]) && !is_digit(line[i + 2]) && line[i + 2] != '_') {
                code = "DOTK";
                i = i + 2;
            }
            break;
        case 'e':
            if (line[i + 1] == 'l' && line[i + 2] == 's' && line[i + 3] == 'e' && !is_alpbat(line[i + 4]) &&
                !is_digit(line[i + 4]) && line[i + 4] != '_') {
                code = "ELSETK";
                i = i + 4;
            }
            break;
        case 'f':
            if (line[i + 1] == 'o' && line[i + 2] == 'r' && !is_alpbat(line[i + 3]) && !is_digit(line[i + 3])) {
                code = "FORTK";
                i = i + 3;
            }
            break;
        case 'i':
            code = alpbat_i(i);
            break;
        case 'm':
            if (line[i + 1] == 'a' && line[i + 2] == 'i' && line[i + 3] == 'n' && line[i + 4] == '(') {
                code = "MAINTK";
                i = i + 4;
            }
            break;
        case 'p':
            if (line[i + 1] == 'r' && line[i + 2] == 'i' && line[i + 3] == 'n' && line[i + 4] == 't' &&
                line[i + 5] == 'f' && !is_alpbat(line[i + 6]) && !is_digit(line[i + 6]) && line[i + 6] != '_') {
                code = "PRINTFTK";
                i = i + 6;
            }
            break;
        case 'v':
            if (line[i + 1] == 'o' && line[i + 2] == 'i' && line[i + 3] == 'd' && !is_alpbat(line[i + 4]) &&
                !is_digit(line[i + 4]) && line[i + 4] != '_') {
                code = "VOIDTK";
                i = i + 4;
            }
        case 'r':
            if (line[i + 1] == 'e' && line[i + 2] == 't' && line[i + 3] == 'u' && line[i + 4] == 'r' &&
                line[i + 5] == 'n' && (line[i + 6] == ' ' || line[i + 6] == ';')) {
                code = "RETURNTK";
                i = i + 6;
            }
            break;
        case 's':
            if (line[i + 1] == 'c' && line[i + 2] == 'a' && line[i + 3] == 'n' && line[i + 4] == 'f' &&
                !is_alpbat(line[i + 5]) && !is_digit(line[i + 5]) && line[i + 5] != '_') {
                code = "SCANFTK";
                i = i + 5;
            }
            break;
        case 'w':
            if (line[i + 1] == 'h' && line[i + 2] == 'i' && line[i + 3] == 'l' && line[i + 4] == 'e' &&
                !is_alpbat(line[i + 5]) && !is_digit(line[i + 5]) && line[i + 5] != '_') {
                code = "WHILETK";
                i = i + 5;
            }
            break;
        default:
            break;
    }
}


void cope_firsrtaph(int &i) {   //处理首字母是字母
    string code;
    recog_reserve(i, code);
    if (code != " ") {
        outfile << code << " " << find_key(code) << endl;
    } else {
        for (; is_digit(line[i]) || is_alpbat(line[i]) || line[i] == '_'; i++) {
            code.push_back(line[i]);
        }
        outfile << "IDENFR" << " " << code << endl;
    }
}


void cope_firsrtnum(int &i) {       //处理首字母是数字
    string code;
    for (; isdigit(line[i]); i++) {
        code.push_back(line[i]);
    }
    outfile << "INTCON" << " " << code << endl;
}


void cope_operaor(int &i) {     //处理操作符
    string code = is_oprator(i);
    if (code == "//") {
        while (line[i] != '\n')
            i++;
    } else if (code != " ") {
        outfile << code << " " << find_key(code) << endl;
    }
    i++;
}

void cope_string(int &i) {      //处理字符串常量
    i++;
    string str;
    while (line[i] != '"') {
        str.push_back(line[i]);
        i++;
    }
    i++;
    outfile << "STRCON" << " " << str << endl;
}

void cope_char(int &i) {    //处理字符常量
    outfile << "CHARCON" << " " << line[i + 1] << endl;
    i = i + 3;
}

void cope_range(string code, int &i) {  //处理首字符是界符
    outfile << code << " " << find_key(code) << endl;
    i++;
}

bool analysis(char *in_file, char *out_file) {  //  分析
    infile.open(in_file, ios::in);
    outfile.open(out_file, ios::out);
    if (!infile || !outfile)
        return false;
    init();
    string code;
    while (!infile.eof()) {
        getline(infile, line);
        line.push_back('\n');
        for (int i = 0; line[i] != '\n';) {
            while (line[i] == ' ') i++;        //除空格
            if (is_alpbat(line[i]) || line[i] == '_') {  //如果是字母
                cope_firsrtaph(i);
            } else if (is_digit(line[i]) && line[i] != '\n') { //如果是数字
                cope_firsrtnum(i);
            } else if ((code = is_range(i)) != " " && line[i] != '\n') {//首字符为界符
                cope_range(code, i);
            } else if (line[i] == '"') {
                cope_string(i);
            } else if (line[i] == '\'') {
                cope_char(i);
            } else {//首字符为操作符
                if (line[i] != '\n')
                    cope_operaor(i);
            }
        }
    }
    infile.close();
    outfile.close();
    return true;
}

int main() {
    char input[13] = "/connect/testfile.txt";
    char output[11] = "/connect/output.txt";
    bool result = analysis(input, output);
    return result;
}
