#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string.h>

// simple warper
static inline char* next_token(char** rest) {
    return strtok_r(*rest, " \n\t\r", rest); // haha ntr thats funny
}

#endif // TOKENIZER_H
