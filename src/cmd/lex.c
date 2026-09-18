#ifndef LEX_C
#define LEX_C

#include "../lexer.c"
#include "../types.c"

// Tokenize from a input fileDescriptor and write the binary content of the tokens into a output fileDescriptr.
void lex(Reader* reader, Writer* writer) {
    // Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD
    Lexer lexer = Lexer_new(reader);
    TokenWriter tokenWriter = TokenWriter_new(writer);

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

        TokenWriter_write(&tokenWriter, token);
    }
    TokenWriter_flush(&tokenWriter);
}

// Binary entrypoint if we want only to test the tokenization
void _start(void) {
    __asm__ __volatile__ ("and $-16, %%rsp" ::: "memory"); // Fix the stack for the System V constraints
    lex(stdin, stdout);
    exit(EXIT_SUCCESS);
}

#endif