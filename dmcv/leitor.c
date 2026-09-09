//Se deu tudo certo na leitura retorna 0, caso não retorna 1
#include "leitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int ler_entrada(int argc, char *argv[], int * total_tarefas, int * tempo_total, Tarefa ** tarefas) {
    FILE * entrada = fopen(argv[2], "r");
    
    if(entrada == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }

    int resultado_leitura = fscanf(entrada, "%d", tempo_total);
    //Verificação se tiver textp
    if(resultado_leitura < 1){
        fprintf(stderr,"Erro: valor não númerico passado\n");
        fclose(entrada);
        return 1;
    }

    //Verificação se é negativo
    if(*tempo_total <= 0){                                   
        fprintf(stderr, "Erro: tempo total deve ser positivo!\n");
        fclose(entrada);
        return 1;
    }

    *tarefas = NULL;
    
    while(1) {
        
        
        int i = *total_tarefas;
        
        Tarefa * temp = realloc(*tarefas, (i + 1) * sizeof(Tarefa));
        if(temp == NULL){
             free(*tarefas);
             return 1;
        }
 
        *tarefas = temp;
        
        resultado_leitura = fscanf(entrada, "%s %d %d %d", (*tarefas)[i].nome, &(*tarefas)[i].periodo, &(*tarefas)[i].deadline, &(*tarefas)[i].burst);


        if(resultado_leitura == EOF){
            fclose(entrada);
            return 0;
        }

        if(resultado_leitura < 4){
            fprintf(stderr,"Erro: valor não numérico ou formato inválido!\n");
            fclose(entrada);
            return 1;
        }

        if ((*tarefas)[i].periodo <= 0 || (*tarefas)[i].deadline <= 0 || (*tarefas)[i].burst <= 0) {
            fprintf(stderr, "Erro: valores inválidos para período, deadline ou burst!\n");
            fclose(entrada);
            return 1;
        }

        if( (*tarefas)[i].deadline > (*tarefas)[i].periodo || (*tarefas)[i].burst > (*tarefas)[i].deadline){
            fprintf(stderr, "Erro: valores inválidos para período, deadline ou burst!\n");
            fclose(entrada);
            return 1;
        }

        (*tarefas)[i].chegada_atual = 0; //quando a instância da tarefa chegou

        (*tarefas)[i].restante = (*tarefas)[i].burst; //quanto de trabalho ainda falta

        (*tarefas)[i].deadline_absoluta = (*tarefas)[i].chegada_atual + (*tarefas)[i].deadline;

        (*tarefas)[i].concluida = 0;

        (*tarefas)[i].perdidas = 0;

        (*tarefas)[i].ativa = false;

        (*tarefas)[i].restante_ultima_exec = 0;

        (*total_tarefas)++;

    }
}