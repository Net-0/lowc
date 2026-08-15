#ifndef TOKEN_C
#define TOKEN_C

#include "./position.c"

typedef enum TokenType TokenType;
enum TokenType {
    NO_TOKEN,
    UNEXPECTED,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    DOCUMENTATION,
    NUMBER,
    WORD,
    STRING,
    CHAR,
    OPEN_ROUND_BRACKET,
    CLOSE_ROUND_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_CURLY_BRACKET,
    CLOSE_CURLY_BRACKET,
    COLON,
    SEMICOLON,
    COMMA,
    EQUAL,
    EXCLAMATION,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    // TODO: add colon, semicolon, comma and other stuf
};

void TokenType_write(const TokenType tokenType, Writer* writer) {
    switch (tokenType) {
        case NO_TOKEN:
            Writer_writeString(writer, "NO_TOKEN");
            break;
        case UNEXPECTED:
            Writer_writeString(writer, "UNEXPECTED");
            break;
        case SINGLE_LINE_COMMENT:
            Writer_writeString(writer, "SINGLE_LINE_COMMENT");
            break;
        case MULTI_LINE_COMMENT:
            Writer_writeString(writer, "MULTI_LINE_COMMENT");
            break;
        case DOCUMENTATION:
            Writer_writeString(writer, "DOCUMENTATION");
            break;
        case WORD:
            Writer_writeString(writer, "WORD");
            break;
        case NUMBER:
            Writer_writeString(writer, "NUMBER");
            break;
        case STRING:
            Writer_writeString(writer, "STRING");
            break;
        case CHAR:
            Writer_writeString(writer, "CHAR");
            break;
        case OPEN_ROUND_BRACKET:
            Writer_writeString(writer, "OPEN_ROUND_BRACKET");
            break;
        case CLOSE_ROUND_BRACKET:
            Writer_writeString(writer, "CLOSE_ROUND_BRACKET");
            break;
        case OPEN_SQUARE_BRACKET:
            Writer_writeString(writer, "OPEN_SQUARE_BRACKET");
            break;
        case CLOSE_SQUARE_BRACKET:
            Writer_writeString(writer, "CLOSE_SQUARE_BRACKET");
            break;
        case OPEN_CURLY_BRACKET:
            Writer_writeString(writer, "OPEN_CURLY_BRACKET");
            break;
        case CLOSE_CURLY_BRACKET:
            Writer_writeString(writer, "CLOSE_CURLY_BRACKET");
            break;
        case COLON:
            Writer_writeString(writer, "COLON");
            break;
        case SEMICOLON:
            Writer_writeString(writer, "SEMICOLON");
            break;
        case COMMA:
            Writer_writeString(writer, "COMMA");
            break;
        case EQUAL:
            Writer_writeString(writer, "EQUAL");
            break;
        case EXCLAMATION:
            Writer_writeString(writer, "EXCLAMATION");
            break;
        case PLUS:
            Writer_writeString(writer, "PLUS");
            break;
        case MINUS:
            Writer_writeString(writer, "MINUS");
            break;
        case STAR:
            Writer_writeString(writer, "STAR");
            break;
        case SLASH:
            Writer_writeString(writer, "SLASH");
            break;
    }
}

////////////////////////

typedef enum KeywordType KeywordType;
enum KeywordType {
    NaK, // Not a keyword
    LET,
    FUNCTION,
    STRUCT,
};

void KeywordType_write(const KeywordType keywordType, Writer* writer) {
    switch (keywordType) {
        case NaK:
            Writer_writeString(writer, "NaK");
            break;
        case LET:
            Writer_writeString(writer, "LET");
            break;
        case FUNCTION:
            Writer_writeString(writer, "FUNCTION");
            break;
        case STRUCT:
            Writer_writeString(writer, "STRUCT");
            break;
    }
}

////////////////////////

typedef struct Token Token;
struct Token {
    TokenType type;
    KeywordType keywordType;
    Position start;
    Position end;
};

const Token _NO_TOKEN_ = { .type = NO_TOKEN, .start = NO_POSITION, .end = NO_POSITION };

void Token_write(const Token* token, Writer* writer) {
    Writer_writeString(writer, "Token(type=");
    TokenType_write(token->type, writer);
    Writer_writeString(writer, ", keywordType=");
    KeywordType_write(token->keywordType, writer);
    Writer_writeString(writer, ", start=");
    Position_write(&token->start, writer);
    Writer_writeString(writer, ", end=");
    Position_write(&token->end, writer);
    Writer_writeString(writer, ")");
}

#endif