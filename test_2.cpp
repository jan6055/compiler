#include <fstream>
#include "library.h"
using namespace std;
//
// Created by qming on 23-3-30.
//
int main() {
    std::ifstream f("../test.txt");
    Tokenizer t(f);
    while (!t.eof()) {
        cout << t.next_word().value() << endl;
    }
    std::ifstream f1("../nothing.txt");
    
    Tokenizer t1(f1);
    while (!t1.eof()) {
        auto opt = t1.next_word();
        if(opt.has_value()) {
            cout << opt.value() << endl;
        }
    }
    cout << "here nothing" << endl;
    int ifelse;
}