#ifndef TOKEN_C
#define TOKEN_C

#include "./position.c"
#include "./io.c"
#include "./memory.c"

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

////////////////////////////////////////////////////

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

////////////////////////////////////////////////////

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

////////////////////////////////////////////////////

// Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD

#define TOKEN_IO_BUFFER_CAPACITY 256

typedef struct TokenReader TokenReader;
struct TokenReader {
    Reader* reader;
    Token buffer[TOKEN_IO_BUFFER_CAPACITY];
    u16 bufferLength;
    u16 bufferIndex;
};

TokenReader TokenReader_new(Reader* reader) {
    return (TokenReader) { .reader = reader, .bufferLength = 0, .bufferIndex = 1 };
}

void TokenReader_fill(TokenReader* tokenReader) {
    // 1. Move unread buffered data to the begin of the buffer
    const u16 remaining = tokenReader->bufferLength - min(tokenReader->bufferLength, tokenReader->bufferIndex);
    memmove((byte*) tokenReader->buffer, (byte*) tokenReader->buffer+(tokenReader->bufferIndex*sizeof(Token)), remaining*sizeof(Token));

    // 2. Fill the remaining space in the buffer
    const u32 n = Reader_nextN(tokenReader->reader, (byte*) tokenReader->buffer+(remaining*sizeof(Token)), sizeof(Token)*(TOKEN_IO_BUFFER_CAPACITY-remaining));
    if (n % sizeof(Token) != 0)
        exit(EXIT_TOKEN_READER_MALFORMED_DATA);
    tokenReader->bufferLength = remaining + (n / sizeof(Token));
    tokenReader->bufferIndex = 0;
}

boolean TokenReader_hasNext(TokenReader* tokenReader) {
    if (tokenReader->bufferLength < tokenReader->bufferIndex)
        TokenReader_fill(tokenReader);

    return tokenReader->bufferIndex < tokenReader->bufferLength;
}

Token TokenReader_peek(TokenReader* tokenReader) {
    if (!TokenReader_hasNext(tokenReader)) // check and load buffer if needed!
        exit(EXIT_TOKEN_READER_NO_REMAINING_DATA);

    return ((Token*) &tokenReader->buffer)[tokenReader->bufferIndex];
}

Token TokenReader_next(TokenReader* tokenReader) {
    if (!TokenReader_hasNext(tokenReader)) // check and load buffer if needed!
        exit(EXIT_TOKEN_READER_NO_REMAINING_DATA);

    return ((Token*) &tokenReader->buffer)[tokenReader->bufferIndex++];
}

typedef struct TokenWriter TokenWriter;
struct TokenWriter {
    Writer* writer;
    Token buffer[TOKEN_IO_BUFFER_CAPACITY];
    u16 bufferIndex;
};

TokenWriter TokenWriter_new(Writer* writer) {
    return (TokenWriter) { .writer = writer, .bufferIndex = 0 };
}

void TokenWriter_flush(TokenWriter* tokenWriter) {
    Writer_writeN(tokenWriter->writer, (byte*) tokenWriter->buffer, tokenWriter->bufferIndex*sizeof(Token));
    Writer_flush(tokenWriter->writer);
}

void TokenWriter_write(TokenWriter* tokenWriter, const Token token) {
    if (TOKEN_IO_BUFFER_CAPACITY < tokenWriter->bufferIndex)
        TokenWriter_flush(tokenWriter);
    tokenWriter->buffer[tokenWriter->bufferIndex++] = token;
}

#endif