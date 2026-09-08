#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulador.h"

void verificar_deadlines_vencidas(Tarefa ** tarefas, int total_tarefas, int t_atual){

    for(int i = 0; i < total_tarefas; i++){

        if((*tarefas)[i].ativa == true){

            if((*tarefas)[i].deadline_absoluta == t_atual && (*tarefas)[i].restante > 0){

                (*tarefas)[i].perdidas++;
                (*tarefas)[i].ativa = false;

                printf("[%s] for %d units - L\n",(*tarefas)[i].nome, (*tarefas)[i].restante_ultima_exec - (*tarefas)[i].restante);
                (*tarefas)[i].restante = 0;
            }
        }
    }
}

void verificar_chegadas_novas(Tarefa ** tarefas, int total_tarefas, int t_atual){

    if(t_atual == 0){

        for(int i = 0; i < total_tarefas; i++){
            (*tarefas)[i].ativa = true;
            (*tarefas)[i].restante = (*tarefas)[i].burst;
            (*tarefas)[i].deadline_absoluta  = (*tarefas)[i].chegada_atual + (*tarefas)[i].deadline;
        }
    }
    else
    {
        for(int i = 0; i < total_tarefas; i++){

            //Significa que tá no tempo de iniciar um novo ciclo
            if(((*tarefas)[i].chegada_atual + (*tarefas)[i].periodo) == t_atual){

                (*tarefas)[i].ativa = true;
                (*tarefas)[i].restante = (*tarefas)[i].burst;
                (*tarefas)[i].chegada_atual = t_atual;
                (*tarefas)[i].deadline_absoluta = (*tarefas)[i].chegada_atual + (*tarefas)[i].deadline;

            }
        }
    }
}

int tarefa_a_ser_executada_RM(Tarefa ** tarefas, int total_tarefas){

    int indice_maior_prioridade = 0;
    int periodo_maior_prioridade = 9999;
    bool tem_ativa = false;

    for(int i = 0; i < total_tarefas; i++){

        if((*tarefas)[i].ativa == true){

            tem_ativa = true;

            if((*tarefas)[i].periodo < periodo_maior_prioridade){
                indice_maior_prioridade = i;
                periodo_maior_prioridade = (*tarefas)[i].periodo;
            }
        }
    }

    if(tem_ativa == false){
        return -1;
    }

    return indice_maior_prioridade;
}

void RM(Tarefa ** tarefas, int total_tarefas, int tempo_total){

    FILE * saida_RM = fopen("rate_dmcv.out","w");

    if(saida_RM == NULL){
        fprintf(stderr, "Falha ao abrir o arquivo de saída\n");
        return NULL;
    }
    
    int contador_idle = 0;
    int indice_ultima_tarefa = 0;


    for(int i = 0; i < tempo_total; i++){

        verificar_deadlines_vencidas(tarefas,total_tarefas,i);

        verificar_chegadas_novas(tarefas,total_tarefas,i);

        int indice_tarefa_atual = tarefa_a_ser_executada_RM(tarefas, total_tarefas);

        if(i == 0){
            indice_ultima_tarefa = indice_tarefa_atual;
        }

        if(indice_tarefa_atual == -1){

            contador_idle++;
            indice_ultima_tarefa = -1;

        } 
        
        else{
            
            if(contador_idle > 0){
                printf("idle for %d units\n",contador_idle);
                contador_idle = 0;
            }
            //Completou uma execução
            if((*tarefas)[indice_tarefa_atual].restante == 0){
    
                (*tarefas)[indice_tarefa_atual].ativa = false;
                (*tarefas)[indice_tarefa_atual].concluida++;


                //Não teve troca de tarefa, e a tarefa conseguiu completar seu burst
                if((*tarefas)[indice_tarefa_atual].restante_ultima_exec == 0){
                    
                    printf("[%s] for %d units - F\n",(*tarefas)[indice_tarefa_atual].nome, (*tarefas)[indice_tarefa_atual].burst);
                }
                
                else{

                    printf("[%s] for %d units - F\n",(*tarefas)[indice_tarefa_atual].nome, (*tarefas)[indice_tarefa_atual].restante_ultima_exec);
                }

                indice_tarefa_atual = tarefa_a_ser_executada_RM(tarefas, total_tarefas);

                
                if(indice_tarefa_atual == -1){
                    contador_idle++;
                    indice_ultima_tarefa = -1;
                    continue;
                }
                
                //Só altero a chegada atual se a tarefa ainda não tinha execução pendente
                if((*tarefas)[indice_tarefa_atual].restante == 0){

                    (*tarefas)[indice_tarefa_atual].chegada_atual = i;
            
                }
                
                
            }
    
            //Última tarefa teve que parar
            else if(indice_ultima_tarefa != -1 && indice_ultima_tarefa != indice_tarefa_atual && (*tarefas)[indice_ultima_tarefa].restante > 0){
    
                printf("[%s] for %d units - H\n",(*tarefas)[indice_ultima_tarefa].nome, (*tarefas)[indice_ultima_tarefa].burst - (*tarefas)[indice_ultima_tarefa].restante);
                
                (*tarefas)[indice_ultima_tarefa].restante_ultima_exec = (*tarefas)[indice_ultima_tarefa].restante;
            }
            
            (*tarefas)[indice_tarefa_atual].restante--; 
            indice_ultima_tarefa = indice_tarefa_atual;
        }

        if(i == tempo_total - 1){
            //Caso termine em idle
            if(contador_idle > 0){
                printf("idle for %d units\n",contador_idle);
            }
        }

        
    }

    printf("\nLOST DEADLINES\n");
    for(int i = 0; i < total_tarefas; i++){
        printf("[%s] %d\n",(*tarefas)[i].nome,(*tarefas)[i].perdidas);
    }
    printf("\nCOMPLETE EXECUTION\n");
    for(int i = 0; i < total_tarefas; i++){
        printf("[%s] %d\n",(*tarefas)[i].nome,(*tarefas)[i].concluida);
    }

    printf("\nKILLED\n");
    //Verificar se não tem nenhuma tarefa ativa, pois caso esteja ela vai pra Killed
    for(int j = 0; j < total_tarefas; j++){

        if((*tarefas)[j].ativa == true){
            printf("[%s] 1\n",(*tarefas)[j].nome);
        }
        else{
            printf("[%s] 0\n",(*tarefas)[j].nome);
        }
    }

}
