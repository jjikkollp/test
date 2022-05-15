#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

map<string, string> match{
    {"\n", ""},
    {"\r", ""},
    {"//.*", "note line"},
    {"[ ]*", ""},
    {"/\\*(.|\n|\r)*\\*/", "note lines"},
    {"int", "INT"},
    {"float", "FLOAT"},
    {"void", "VOID"},
    {"const", "CONST"},
    {"return", "RETURN"},
    {"if", "IF"},
    {"else", "ELSE"},
    {"for", "FOR"},
    {"while", "WHILE"},
    {"do", "DO"},
    {"break", "BREAK"},
    {"continue", "CONTINUE"},
    {"\\(", "LP"},
    {"\\)", "RP"},
    {"\\[", "LB"},
    {"\\]", "RB"},
    {"\\{", "LC"},
    {"\\}", "RC"},
    {",", "COMMA"},
    {";", "SEMICOLON"},
    {"\\?", "QUESTION"},
    {":", "COLON"},
    {"!", "NOT"},
    {"=", "ASSIGN"},
    {"\\-", "MINUS"},
    {"\\+", "ADD"},
    {"\\*", "MUL"},
    {"/", "DIV"},
    {"%", "MOD"},
    {"&&", "AND"},
    {"\\|\\|", "OR"},
    {"==", "EQ"},
    {"!=", "NE"},
    {"<", "LT"},
    {"<=", "LE"},
    {">", "GT"},
    {">=", "GE"},
    {"[a-zA-Z_][a-zA-Z0-9_]*", "ID"},
    {"[-+]?(([1-9][0-9]*)|(0[0-7]*)|(0x[0-9a-f]+)|(0X[0-9A-F]+))", "INT_LIT"},
    {"0x[0-9a-f]+", "INT_LIT"},
    {"[-+]?(([0-9]*\\.([0-9]*)?)(e[-+]?[0-9]+)?f?)|(([0-9]*(\\.[0-9]*)?)(e[-+]?"
     "[0-9]+)f?)",
     "FLOAT_LIT"},
    {"([0-9]+[a-wyzA-WYZ]+[a-zA-Z0-9]*)|(0[8-9a-fA-F][0-9a-fA-F]*)", "error"}};
int main(int argc, char **argv) {
  if (argc == 1)
    cout << "please input a file path" << endl;
  else {
    string path = argv[1];
    fstream fin(path, ios::in);
    int line = 1;
    string buf;
    while (getline(fin, buf)) {
      string nxtLine;
      if (buf.find("/*") != string::npos) {
        line++;
        while (nxtLine.find("*/") == string::npos) {
          line++;
          getline(fin, nxtLine);
          buf += nxtLine;
        }
        continue;
      }
      while (!buf.empty()) {
        smatch sm;
        auto pre = buf.cend();
        string content, res, rem;
        for (auto &each : match) {
          regex r(each.first);
          if (regex_search(buf, sm, r)) {
            if (sm.prefix().second < pre) {
              pre = sm.prefix().second;
              content = sm.str();
              res = each.second;
              rem = sm.suffix();
            } else if (sm.prefix().second == pre &&
                       content.length() < sm.str().length()) {
              content = sm.str();
              res = each.second;
              rem = sm.suffix();
            } else if (sm.prefix().second == pre &&
                       content.length() == sm.str().length() && res == "ID" &&
                       each.second != "ID") {
              content = sm.str();
              res = each.second;
              rem = sm.suffix();
            }
          }
        }
        buf = rem;
        if (res.find("note") != string::npos) {
          for (auto c : res)
            if (c == '\n')
              line++;
        } else if (res == "error")
          cout << "Lexical error - line " << line << " : " << content << "\n";
        else if (!res.empty()) {
          cout << content << " : " << res << "\n";
        }
      }
      line++;
    }
    fin.close();
  }
  return 0;
}