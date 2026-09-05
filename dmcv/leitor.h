#ifndef LEITOR_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    int periodo;
    int deadline;
    int burst;

    int chegada_atual;
    int restante;
    int deadline_absoluta; // chegada_atual + deadline
    int concluida;
    int perdidas;
} Tarefa;

int ler_entrada(int argc, char *argv[], int *total_tarefas, int *tempo_total, Tarefa **tarefas);

#endif