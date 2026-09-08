#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "leitor.h"
#include "simulador.h"

int main(int argc, char *argv[]) {
    
    if(argc != 3) {
        fprintf(stderr, "Quantidade de argumentos inválida!\n");
        return 1;
    }

    if(strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Argumento inválido!\n");
        return 1;
    }

    int algoritmo;

    if(strcmp(argv[1], "rate") == 0){
        algoritmo = 0;
    }
    else{
        algoritmo = 1;
    }

    int total_tarefas;
    int tempo_total;

    Tarefa * tarefas = NULL;

    int resultado_entrada = ler_entrada(argc, argv,&total_tarefas,&tempo_total,&tarefas);
    if(resultado_entrada == 1){
        return 1;
    }

    simular(&tarefas,total_tarefas,tempo_total, algoritmo);

    free(tarefas);
    
    return 0;
}