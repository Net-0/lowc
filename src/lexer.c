#ifndef LEXER_C
#define LEXER_C

#include "./io.c"
#include "./types.c"
#include "./token.c"

boolean isSpace(const byte b) {
    return b == ' '  ||  // space
           b == '\t' ||  // horizontal tab
           b == '\n' ||  // newline
           b == '\r' ||  // carriage return
           b == '\v' ||  // vertical tab
           b == '\f';    // form feed
}

boolean isDigit(const byte b) {
    return b >= '0' && b <= '9';
}

boolean isWord(const byte b) {
    return (b >= 'a' && b <= 'z') ||
           (b >= 'A' && b <= 'Z') ||
           isDigit(b) ||
           b == '_';
}

typedef struct Lexer Lexer;
struct Lexer {
    Reader* reader;
};

Lexer Lexer_new(Reader* reader) {
    return (Lexer) { .reader = reader };
}

Token Lexer_next(Lexer* lexer) {
    TokenType type;
    KeywordType keywordType = NaK;

    // Just return NO_TOKEN if there is no remaining data to read
    if (!Reader_hasNext(lexer->reader))
        return (Token) { .type = NO_TOKEN, .start = NO_POSITION, .end = NO_POSITION };

    // Just skip spaces
    if (isSpace(Reader_peek(lexer->reader)))
        for (Reader_next(lexer->reader); Reader_hasNext(lexer->reader) && isSpace(Reader_peek(lexer->reader)); Reader_next(lexer->reader));

    // Just return NO_TOKEN if there is no remaining data to read
    if (!Reader_hasNext(lexer->reader))
        return (Token) { .type = NO_TOKEN, .start = NO_POSITION, .end = NO_POSITION };

    const Position start = lexer->reader->position;
    const byte b = Reader_peek(lexer->reader);
    // TODO: adicionar o lexing de comentários e doc
    if (isDigit(b)) {
        type = NUMBER;
        for (Reader_next(lexer->reader); Reader_hasNext(lexer->reader) && isDigit(Reader_peek(lexer->reader)); Reader_next(lexer->reader));
    } else if (isWord(b)) {
        type = WORD;

        byte keywordBuffer[10]; // Buffer with the capacity equals to the max keyword length, so we can use to check the keyword
        for (u8 i = 0; Reader_hasNext(lexer->reader) && isWord(Reader_peek(lexer->reader)) && i < sizeof(keywordBuffer); keywordBuffer[i++] = Reader_next(lexer->reader));
        for (; Reader_hasNext(lexer->reader) && isWord(Reader_peek(lexer->reader)); Reader_next(lexer->reader));

        const Position end = lexer->reader->position;
        if (start.line == end.line) {
            const u32 length = end.column - start.column;
            if (length == 3 && memeq("let", keywordBuffer, 3))
                keywordType = LET;
            else if (length == 8 && memeq("function", keywordBuffer, 8))
                keywordType = FUNCTION;
            else if (length == 6 && memeq("struct", keywordBuffer, 6))
                keywordType = STRUCT;
        }
    } else if (b == '"') {
        type = STRING;
        for (Reader_next(lexer->reader); Reader_hasNext(lexer->reader) && Reader_peek(lexer->reader) != '"'; Reader_next(lexer->reader));
        if (Reader_peek(lexer->reader) == '"') Reader_next(lexer->reader);
    } else if (b == '\'') {
        type = CHAR;
        for (Reader_next(lexer->reader); Reader_hasNext(lexer->reader) && Reader_peek(lexer->reader) != '\''; Reader_next(lexer->reader));
        if (Reader_peek(lexer->reader) == '\'') Reader_next(lexer->reader);
    } else if (b == '(') {
        type = OPEN_ROUND_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == ')') {
        type = CLOSE_ROUND_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == '[') {
        type = OPEN_SQUARE_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == ']') {
        type = CLOSE_SQUARE_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == '{') {
        type = OPEN_CURLY_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == '}') {
        type = CLOSE_CURLY_BRACKET;
        Reader_next(lexer->reader);
    } else if (b == ':') {
        type = COLON;
        Reader_next(lexer->reader);
    } else if (b == ';') {
        type = SEMICOLON;
        Reader_next(lexer->reader);
    } else if (b == ',') {
        type = COMMA;
        Reader_next(lexer->reader);
    } else if (b == '=') {
        type = EQUAL;
        Reader_next(lexer->reader);
    } else if (b == '!') {
        type = EXCLAMATION;
        Reader_next(lexer->reader);
    } else if (b == '+') {
        type = PLUS;
        Reader_next(lexer->reader);
    } else if (b == '-') {
        type = MINUS;
        Reader_next(lexer->reader);
    } else if (b == '*') {
        type = STAR;
        Reader_next(lexer->reader);
    } else if (b == '/') {
        type = SLASH;
        Reader_next(lexer->reader);
    } else {
        type = UNEXPECTED;
        for (Reader_next(lexer->reader); Reader_hasNext(lexer->reader) && !isSpace(Reader_peek(lexer->reader)); Reader_next(lexer->reader));
    }

    const Position end = lexer->reader->position;
    return (Token) { .type = type, .keywordType = keywordType, .start = start, .end = end };
}

void Lexer_lex(Lexer* lexer, u64* tokenCount, Token* tokenOutput) {
    u64 _tokenCount = 0;
    while (true) {
        const Token token = Lexer_next(lexer);
        if (token.type == NO_TOKEN)
            break;
        tokenOutput[_tokenCount] = token;
    }
    *tokenCount = _tokenCount;
}

////////////////////////

#endif