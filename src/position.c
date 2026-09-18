#ifndef POSITION_C
#define POSITION_C

#include "./types.c"

typedef struct Position Position;
struct Position {
    u32 index;
    u32 line;
    u32 column;
};

const Position NO_POSITION = { .index = 0, .line = 0, .column = 0 };
const Position INITIAL_POSITION = { .index = 0, .line = 1, .column = 1 };

#include "./io.c"

void Position_write(const Position* position, Writer* writer) {
    Writer_writeString(writer, "Position(index=");
    Writer_writeU32(writer, position->index);
    Writer_writeString(writer, ", line=");
    Writer_writeU32(writer, position->line);
    Writer_writeString(writer, ", column=");
    Writer_writeU32(writer, position->column);
    Writer_writeString(writer, ")");
}

#endif