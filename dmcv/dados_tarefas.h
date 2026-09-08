#ifndef DADOS_TAREFAS_H
#define DADOS_TAREFAS_H

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
    bool ativa;
    int restante_ultima_exec;
} Tarefa;

#endif