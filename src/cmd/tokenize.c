#ifndef TOKENIZER_C
#define TOKENIZER_C

#include "../lexer.c"
#include "../types.c"

// Tokenize from a input fileDescriptor and write the binary content of the tokens into a output fileDescriptr.
void tokenize(Reader* reader, Writer* writer) {
    // Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD
    Lexer lexer = Lexer_new(reader);

    for (;;) {
        const Token token = Lexer_next(&lexer);

        // Stop if no token left
        if (token.type == NO_TOKEN) break;

        if (token.type == UNEXPECTED) {
            Writer_writeString(stderr, "Unexpected token: ");
            Token_write(&token, stderr);
            Writer_writeString(stderr, "\n");
            Writer_flush(stderr);
            exit(EXIT_TOKENIZE_UNEXPECTED_TOKEN);
        }

        Writer_writeN(writer, (byte*) &token, sizeof(token));
    }
    Writer_flush(writer);
}

// Binary entrypoint if we want only to test the tokenization
void _start(void) {
    tokenize(stdin, stdout);
    exit(EXIT_SUCCESS);
}

#endif