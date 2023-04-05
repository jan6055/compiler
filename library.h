#ifndef COMPILER_LIBRARY_H
#define COMPILER_LIBRARY_H
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>
#include <optional>

using std::string;
enum class TokenType {
    KeyWord,    //关键字
    Op,         //运算符
    Number,     //数字
    Ident,      //标识符
    Bs,         //界符
};



//一个双缓冲优化的输入流缓冲区
class IStreamCache {
public:
    using pointer = std::unique_ptr<char[]>;
    using size_type = std::streamsize;
private:
    pointer cache;
    std::istream & input_stream;
    size_type pos, fence;
    constexpr static size_type BufferSize = 1024;
private:
    size_type fill(size_type size = BufferSize, size_type where = 0);
public:
    char next_char();
    void roll_back();
    explicit IStreamCache(std::istream & is);
    [[nodiscard]] bool eof() const;
};


class Token {
private:
    string word;
    TokenType type;
public:
    Token(string word, TokenType tp);
    [[nodiscard]] TokenType get_type() const;
    friend std::ostream & operator<<(std::ostream & os, const Token & token);
};

class Tokenizer {
public:
    using table_type = const std::unordered_set<string>;
private:
     static table_type key_words;
     static table_type ops;
     static table_type boundary_symbols;
     [[nodiscard]] static bool is_bs(char ch);
     [[nodiscard]] static bool is_keyword(const string & w);
     [[nodiscard]] static bool is_num(char ch);
     [[nodiscard]] static bool is_op(char ch);
     [[nodiscard]] static bool is_white(char ch);
private:
    IStreamCache cache;
public:
    [[nodiscard]] bool eof() const;
    [[nodiscard]] std::optional<Token> next_word();
    explicit Tokenizer(std::istream & is);
    char next_char();
    void roll_back();
};

#endif //COMPILER_LIBRARY_H
