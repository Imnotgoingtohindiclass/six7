#include <stdio.h>
#include <stdlib.h>
#include "six7.h"

int main(int argc, char* argv[]) {
    FILE* file = fopen(argv[1], "r"); // open file

    if (!file) { // existence check
        perror("couldnt open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* code = (char*)malloc(file_size + 1);
    if (!code) {
        perror("memalloc eror");
        fclose(file);
        return 1;
    }

    fread(code, 1, file_size, file);
    code[file_size] = '\0';
    fclose(file);

    execute_six7(code); // heres where the real fun lies

    free(code);
    return 0;
}