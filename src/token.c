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

void TokenType_write(const TokenType tokenType, const i32 fileDescriptor) {
    switch (tokenType) {
        case NO_TOKEN:
            writeString(fileDescriptor, "NO_TOKEN");
            break;
        case UNEXPECTED:
            writeString(fileDescriptor, "UNEXPECTED");
            break;
        case SINGLE_LINE_COMMENT:
            writeString(fileDescriptor, "SINGLE_LINE_COMMENT");
            break;
        case MULTI_LINE_COMMENT:
            writeString(fileDescriptor, "MULTI_LINE_COMMENT");
            break;
        case DOCUMENTATION:
            writeString(fileDescriptor, "DOCUMENTATION");
            break;
        case WORD:
            writeString(fileDescriptor, "WORD");
            break;
        case NUMBER:
            writeString(fileDescriptor, "NUMBER");
            break;
        case STRING:
            writeString(fileDescriptor, "STRING");
            break;
        case CHAR:
            writeString(fileDescriptor, "CHAR");
            break;
        case OPEN_ROUND_BRACKET:
            writeString(fileDescriptor, "OPEN_ROUND_BRACKET");
            break;
        case CLOSE_ROUND_BRACKET:
            writeString(fileDescriptor, "CLOSE_ROUND_BRACKET");
            break;
        case OPEN_SQUARE_BRACKET:
            writeString(fileDescriptor, "OPEN_SQUARE_BRACKET");
            break;
        case CLOSE_SQUARE_BRACKET:
            writeString(fileDescriptor, "CLOSE_SQUARE_BRACKET");
            break;
        case OPEN_CURLY_BRACKET:
            writeString(fileDescriptor, "OPEN_CURLY_BRACKET");
            break;
        case CLOSE_CURLY_BRACKET:
            writeString(fileDescriptor, "CLOSE_CURLY_BRACKET");
            break;
        case COLON:
            writeString(fileDescriptor, "COLON");
            break;
        case SEMICOLON:
            writeString(fileDescriptor, "SEMICOLON");
            break;
        case COMMA:
            writeString(fileDescriptor, "COMMA");
            break;
        case EQUAL:
            writeString(fileDescriptor, "EQUAL");
            break;
        case EXCLAMATION:
            writeString(fileDescriptor, "EXCLAMATION");
            break;
        case PLUS:
            writeString(fileDescriptor, "PLUS");
            break;
        case MINUS:
            writeString(fileDescriptor, "MINUS");
            break;
        case STAR:
            writeString(fileDescriptor, "STAR");
            break;
        case SLASH:
            writeString(fileDescriptor, "SLASH");
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

void KeywordType_write(const KeywordType keywordType, const i32 fileDescriptor) {
    switch (keywordType) {
        case NaK:
            writeString(fileDescriptor, "NaK");
            break;
        case LET:
            writeString(fileDescriptor, "LET");
            break;
        case FUNCTION:
            writeString(fileDescriptor, "FUNCTION");
            break;
        case STRUCT:
            writeString(fileDescriptor, "STRUCT");
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

void Token_write(const Token* token, const i32 fileDescriptor) {
    writeString(fileDescriptor, "Token(type=");
    TokenType_write(token->type, fileDescriptor);
    writeString(fileDescriptor, ", keywordType=");
    KeywordType_write(token->keywordType, fileDescriptor);
    writeString(fileDescriptor, ", start=");
    Position_write(&token->start, fileDescriptor);
    writeString(fileDescriptor, ", end=");
    Position_write(&token->end, fileDescriptor);
    writeString(fileDescriptor, ")");
}

#endif