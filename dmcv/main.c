#include <stdio.h>
#include <string.h> 
#include "leitor.h"


int main(int argc, char *argv[]) {
    
    if(argc != 3) {
        fprintf(stderr, "Quantidade de argumentos inválida!\n");
        return 1;
    }

    if(strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Argumento inválido!\n");
        return 1;
    }

    ler_entrada(argc, argv);

    
    return 0;
}