#include "library.h"
#include <array>
#include <cctype>

Token::Token(string word, TokenType tp) : word(std::move(word)), type(tp){ }

TokenType Token::get_type() const {
    return type;
}


/**
 * @warning 确保调用此函数之前，流没有达到`eof`
 * @return 一个token
 */
std::optional<Token> Tokenizer::next_word() {
    string lexeme;
    TokenType state;
    char ch = next_char();
    while(is_white(ch) && !eof()) {
        ch = next_char();
    }
    if(is_white(ch)) {
        return std::nullopt;
    }
    if(std::isalpha(ch)) {
        state = TokenType::Ident;
    } else if (is_num(ch)) {
        state = TokenType::Number;
    } else if (is_op(ch)) {
        state = TokenType::Op;
    } else if (is_bs(ch)) {
        state = TokenType::Bs;
    } else {
        return std::nullopt;
    }
    
    lexeme.push_back(ch);
    
    while(!cache.eof()) {
        ch = next_char();
        lexeme.push_back(ch);
        switch (state) {
            case TokenType::Ident:
                if(std::isalnum(ch)) {
                    continue;
                } else {
                    roll_back();
                    lexeme.pop_back();
                    if(is_keyword(lexeme)) {
                        return std::make_optional<Token>(lexeme, TokenType::KeyWord);
                    } else {
                        return std::make_optional<Token>(lexeme, TokenType::Ident);
                    }
                }
            case TokenType::Number:
                if(is_num(ch)) {
                    continue;
                } else {
                    lexeme.pop_back();
                    roll_back();
                    return std::make_optional<Token>(lexeme,TokenType::Number);
                }
            case TokenType::Op:
                if(is_op(ch)) {
                    if(ch == '=') {
                        return std::make_optional<Token>(lexeme,TokenType::Op);
                    }
                } else {
                    roll_back();
                    lexeme.pop_back();
                    return std::make_optional<Token>(lexeme,TokenType::Op);
                }
                break;
            case TokenType::Bs:
                roll_back();
                lexeme.pop_back();
                return std::make_optional<Token>(lexeme,TokenType::Bs);
            default:
                break;
        }
    }
    return std::make_optional<Token>(lexeme,state);
}

Tokenizer::Tokenizer(std::istream & is)
    : cache(is)
{ }

char Tokenizer::next_char() {
    return cache.next_char();
}

void Tokenizer::roll_back() {
    cache.roll_back();
}

/**
 * 填充缓冲区从`input_stream`中
 * @param size 填充的个数
 * @param where 偏移量
 * @return 实际填充的个数
 * @note 如果实际填充的个数小于size，那么说明`input_stream`已经到达了`eof`
 */
IStreamCache::size_type IStreamCache::fill(size_type size, size_type where) {
    input_stream.read(cache.get() + where, size);
    return input_stream.gcount();
}

IStreamCache::IStreamCache(std::istream &is)
    : input_stream(is), pos(0), fence(0)
{
    cache = std::make_unique<char[]>(BufferSize);
    fill();
}


char IStreamCache::next_char() {
    char ch = cache[pos++];
    pos %= (2 * BufferSize);
    if(pos % BufferSize == 0) {
        fill(BufferSize,pos);
        fence = (pos + BufferSize) % 2 * BufferSize;
    }
    return ch;
}

void IStreamCache::roll_back() {
    if(pos == fence) {
        //todo
    }
    pos--;
    pos %= (2 * BufferSize);
}
//#define TEST
#include <iostream>
using std::cout;
using std::endl;
bool IStreamCache::eof() const {
    size_type idx = pos;
    idx %= 2 * BufferSize;
    idx %= BufferSize;
#ifdef TEST
    cout << "function: " << __FUNCTION__ << endl;
    cout << "input: " << input_stream.eof() << endl;
    cout << "gcount: " << input_stream.gcount() << endl;
    cout << "index: " << idx << endl;
#endif
    return input_stream.eof() &&
           input_stream.gcount() != BufferSize &&
           idx == input_stream.gcount();
}


Tokenizer::table_type Tokenizer::key_words = {
    "alignas",
    "alignof",
    "auto",
    "bool",
    "break",
    "case",
    "char",
    "const",
    "constexpr",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "false",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "nullptr",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "static_assert",
    "struct",
    "switch",
    "thread_local",
    "true",
    "typedef",
    "typeof",
    "typeof_unqual",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    "_Alignas",
    "_Alignof",
    "_Atomic",
    "_BitInt",
    "_Bool",
    "_Complex",
    "_Decimal128",
    "_Decimal32",
    "_Decimal64",
    "_Generic",
    "_Imaginary",
    "_Noreturn",
    "_Static_assert",
    "_Thread_local",
};

Tokenizer::table_type Tokenizer::ops = {
    "+", "-", "*", "/",
    ">", "<", "=", "!",
};

Tokenizer::table_type Tokenizer::boundary_symbols = {
    "[", "]",
    "(", ")",
    "{", "}",
    ".", ";", ","
};

bool Tokenizer::is_bs(char ch) {
    string s (1,ch);
    if(boundary_symbols.contains(s)) {
        return true;
    } else {
        return false;
    }
}

bool Tokenizer::is_keyword(const string &w) {
    if(key_words.contains(w)) {
        return true;
    } else {
        return false;
    }
}

bool Tokenizer::is_num(char ch) {
    return ch >= '0' && ch <= '9';
}

bool Tokenizer::is_op(char ch) {
    string s(1,ch);
    if(ops.contains(s)) {
        return true;
    } else {
        return false;
    }
}

bool Tokenizer::is_white(char ch) {
    return ch == ' ' || ch == '\n' ||
           ch == '\t'|| ch == '\r';
}

bool Tokenizer::eof() const {
   return cache.eof();
}

std::ostream & operator<<(std::ostream & os, const Token & token) {
    os << "[" << token.word << ", ";
    switch (token.type) {
        case TokenType::KeyWord:
            os << "keyword";
            break;
        case TokenType::Op:
            os << "operator";
            break;
        case TokenType::Ident:
            os << "ident";
            break;
        case TokenType::Bs:
            os << "boundary symbols";
            break;
        case TokenType::Number:
            os << "number";
            break;
    }
    os << "]";
    return os;
}