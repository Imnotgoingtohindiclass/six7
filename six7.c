#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "six7.h"
#include "tokenizer.h"

#define belt 128
#define max_tokens 16384 // max tokens

void execute_six7(char* code) {
    int tape[belt] = {0};
    int pointer = 0; // the all important pointer

    char* tokens[max_tokens];
    int token_count = 0;

    // tokenize! tokenize! tokenize!
    char* rest = code; // the unparsed part
    char* token_start;

    while ((token_start = next_token(&rest)) != NULL) {
        if (token_count >= max_tokens) {
            fprintf(stderr, "toomany tokens. only 16384 pls");
            for (int i = 0; i < token_count; i++) free(tokens[i]);
            return;
        }

        tokens[token_count] = (char*)malloc(strlen(token_start) + 1);
        if (!tokens[token_count]) {
            perror("memalloc error for token");
            for (int i = 0; i < token_count; i++) free(tokens[i]);
            return;
        }

        strcpy(tokens[token_count], token_start);
        token_count++;
    }

    // premap loop jumps
    int loop_stack[max_tokens];
    int stack_ptr = 0; // stack pointer
    int* jump_targets = (int*)malloc(token_count * sizeof(int));

    // handle error for jump target
    if (!jump_targets) {
        perror("memalloc error for jump targets");
        for (int i = 0; i < token_count; i++) free(tokens[i]);
        return;
    }

    for (int i = 0; i < token_count; i++) {
        jump_targets[i] = 0;

        if (strcmp(tokens[i], "6776") == 0) { //loop start
            if (stack_ptr >= max_tokens) {
                fprintf(stderr, "stack overflow somewhere.\n");
                for (int j = 0; j < token_count; j++) free(tokens[j]);
                free(jump_targets);
                return;
            }

            loop_stack[stack_ptr++] = i;
        } else if (strcmp(tokens[i], "7667") == 0) { //loop end
            if (stack_ptr == 0) {
                fprintf(stderr, "unclossed 7667 %d\n", i);
                for (int j = 0; j < token_count; j++) free(tokens[j]);
                free(jump_targets);
                return;
            }

            int open_bracket_ip = loop_stack[--stack_ptr];
            jump_targets[open_bracket_ip] = i;
            jump_targets[i] = open_bracket_ip;
        }
    }

    if (stack_ptr != 0) { //if the ouroborous isnt eating iteself
        fprintf(stderr, "unclosed loop.\n");
        for (int j = 0; j < token_count; j++) free(tokens[j]);
        free(jump_targets);
        return;
    }

    // EXECUTE !!!!!!
    int ip = 0;
    while (ip < token_count) {
        char* current_token = tokens[ip];

        if (strcmp(current_token, "67") == 0) { // pointer increment
            tape[pointer]++;
        } else if (strcmp(current_token, "76") == 0) { // pointer decrement
            tape[pointer]--;
        } else if (strcmp(current_token, "6767") == 0) { // pointer right
            pointer = (pointer + 1) % belt;
        } else if (strcmp(current_token, "7676") == 0) { //pointer left
            pointer = (pointer - 1 + belt) % belt;
        } else if (strcmp(current_token, "666777") == 0) { // yes daddy input whatever you say
            tape[pointer] = getchar();
        } else if (strcmp(current_token, "777666") == 0) { // talk
            putchar(tape[pointer]);
        } else if (strcmp(current_token, "6776") == 0) { //skip loop
            if (tape[pointer] == 0) ip = jump_targets[ip];
        } else if (strcmp(current_token, "7667") == 0) { //jump back
            if (tape[pointer] != 0) ip = jump_targets[ip];
        } else {
            fprintf(stderr, "unkown token '%s' @ instruction pointer %d\n", current_token, ip);
            for (int i = 0; i < token_count; i++) free(tokens[i]);
            free(jump_targets);
            return;
        }
        ip++;
    }

    // clean up user mem blocks
    for (int i = 0; i < token_count; i++) free(tokens[i]);
    free(jump_targets);
}