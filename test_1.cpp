//
// Created by qming on 23-3-29.
//
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "library.h"
using std::cout;
using std::endl;
int main() {
    testing::InitGoogleTest();
    std::ifstream f("../test.txt");
    Tokenizer t(f);
    while (!t.eof()) {
        cout << t.next_word().value() << endl;
    }
    return RUN_ALL_TESTS();
}

TEST(Cache, Base) {
    std::ifstream f("../alot.txt");
    IStreamCache cache(f);
    while(!cache.eof()) {
        cache.next_char();
    }
    ASSERT_TRUE(cache.eof());
    cache.roll_back();
    ASSERT_FALSE(cache.eof());
    int b = !cache.eof();
    ASSERT_TRUE(b);
}

TEST(Cache, NextChar) {
    std::ifstream f("../test_1.txt");
    IStreamCache cache(f);
    ASSERT_EQ(cache.next_char(),'i');
    ASSERT_EQ(cache.next_char(),'n');
    ASSERT_EQ(cache.next_char(),'t');
    ASSERT_TRUE(cache.eof());
    cache.roll_back();
    ASSERT_FALSE(cache.eof());
}
