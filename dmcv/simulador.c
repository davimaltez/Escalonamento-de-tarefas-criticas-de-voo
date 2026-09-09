#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulador.h"

void verificar_deadlines_vencidas(Tarefa ** tarefas, int total_tarefas, int t_atual, FILE * saida){

    for(int i = 0; i < total_tarefas; i++){

        if((*tarefas)[i].ativa == true){

            if((*tarefas)[i].deadline_absoluta == t_atual && (*tarefas)[i].restante > 0){

                int unidades = (*tarefas)[i].restante_ultima_exec - (*tarefas)[i].restante;

                (*tarefas)[i].perdidas++;
                (*tarefas)[i].ativa = false;

                //Para caso uma tarefa nunca chegue a rodar
                if(unidades > 0){
                    
                    fprintf(saida,"[%s] for %d units - L\n",(*tarefas)[i].nome, (*tarefas)[i].restante_ultima_exec - (*tarefas)[i].restante);
                }

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
            (*tarefas)[i].restante_ultima_exec = (*tarefas)[i].burst;
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
                (*tarefas)[i].restante_ultima_exec = (*tarefas)[i].burst;

            }
        }
    }
}

int tarefa_a_ser_executada(Tarefa ** tarefas, int total_tarefas,int algoritmo){

    int indice_maior_prioridade = 0;
    int periodo_maior_prioridade = 9999;

    int menor_deadline_absoluto = 99999;
    bool tem_ativa = false;

    for(int i = 0; i < total_tarefas; i++){

        if((*tarefas)[i].ativa == true){

            tem_ativa = true;

            if(algoritmo == 0){
                
                if((*tarefas)[i].periodo < periodo_maior_prioridade){
                    indice_maior_prioridade = i;
                    periodo_maior_prioridade = (*tarefas)[i].periodo;
                }
            }
            else
            {
                if((*tarefas)[i].deadline_absoluta < menor_deadline_absoluto){
                    indice_maior_prioridade = i;
                    menor_deadline_absoluto = (*tarefas)[i].deadline_absoluta;
                }
            }
        }
    }

    if(tem_ativa == false){
        return -1;
    }

    return indice_maior_prioridade;
}

//0 - RM
//1 - EDF

void simular(Tarefa ** tarefas, int total_tarefas, int tempo_total, int algoritmo){

    FILE * saida;

    if(algoritmo == 0){
        
        saida = fopen("rate_dmcv.out","w");
    
        if(saida == NULL){
            fprintf(stderr, "Falha ao abrir o arquivo de saída\n");
            return;
        }

        fprintf(saida, "EXECUTION BY RATE\n");
    }
    else{

        saida = fopen("edf_dmcv.out","w");
    
        if(saida == NULL){
            fprintf(stderr, "Falha ao abrir o arquivo de saída\n");
            return;
        }
        fprintf(saida, "EXECUTION BY EDF\n");
    }
    
    int contador_idle = 0;
    int indice_ultima_tarefa = 0;


    for(int i = 0; i < tempo_total; i++){

        verificar_deadlines_vencidas(tarefas,total_tarefas,i,saida);

        verificar_chegadas_novas(tarefas,total_tarefas,i);

        int indice_tarefa_atual = tarefa_a_ser_executada(tarefas, total_tarefas,algoritmo);

        if(i == 0){
            indice_ultima_tarefa = indice_tarefa_atual;
        }

        if(indice_tarefa_atual == -1){

            contador_idle++;
            indice_ultima_tarefa = -1;

        } 
        
        else{

            if(contador_idle > 0){
                fprintf(saida,"idle for %d units\n",contador_idle);
                contador_idle = 0;
            }

            // detecta preempção ANTES de decrementar (compara com quem rodou no tempo anterior)
            if(indice_ultima_tarefa != -1 && indice_ultima_tarefa != indice_tarefa_atual && (*tarefas)[indice_ultima_tarefa].restante > 0){

                int unidades_h = (*tarefas)[indice_ultima_tarefa].restante_ultima_exec - (*tarefas)[indice_ultima_tarefa].restante;

                if(unidades_h > 0){

                    fprintf(saida,"[%s] for %d units - H\n", (*tarefas)[indice_ultima_tarefa].nome, (*tarefas)[indice_ultima_tarefa].restante_ultima_exec - (*tarefas)[indice_ultima_tarefa].restante);
                }

                (*tarefas)[indice_ultima_tarefa].restante_ultima_exec = (*tarefas)[indice_ultima_tarefa].restante;
            }

            (*tarefas)[indice_tarefa_atual].restante--;

            // conclusao detectada NO MESMO tick do decremento final, nao no proximo
            if((*tarefas)[indice_tarefa_atual].restante == 0){
                (*tarefas)[indice_tarefa_atual].ativa = false;
                (*tarefas)[indice_tarefa_atual].concluida++;

                fprintf(saida,"[%s] for %d units - F\n",
                    (*tarefas)[indice_tarefa_atual].nome,
                    (*tarefas)[indice_tarefa_atual].restante_ultima_exec);
            }

            indice_ultima_tarefa = indice_tarefa_atual;
        }

        if(i == tempo_total - 1){
                //Caso termine em idle
                if(contador_idle > 0){
                    fprintf(saida,"idle for %d units\n",contador_idle);
                }
            }

            
    }

    // Fecha a rajada que ficou em aberto quando a simulacao terminou

    if(indice_ultima_tarefa != -1 && (*tarefas)[indice_ultima_tarefa].ativa == true && (*tarefas)[indice_ultima_tarefa].restante > 0){

        fprintf(saida, "[%s] for %d units - K\n", (*tarefas)[indice_ultima_tarefa].nome, 
        (*tarefas)[indice_ultima_tarefa].restante_ultima_exec - (*tarefas)[indice_ultima_tarefa].restante);

    }

    fprintf(saida,"\nLOST DEADLINES\n");
    for(int i = 0; i < total_tarefas; i++){
        fprintf(saida,"[%s] %d\n",(*tarefas)[i].nome,(*tarefas)[i].perdidas);
    }
    fprintf(saida,"\nCOMPLETE EXECUTION\n");
    for(int i = 0; i < total_tarefas; i++){
        fprintf(saida,"[%s] %d\n",(*tarefas)[i].nome,(*tarefas)[i].concluida);
    }

    fprintf(saida,"\nKILLED\n");
    //Verificar se não tem nenhuma tarefa ativa, pois caso esteja ela vai pra Killed
    for(int j = 0; j < total_tarefas; j++){

        if((*tarefas)[j].ativa == true){
            fprintf(saida,"[%s] 1\n",(*tarefas)[j].nome);
        }
        else{
            fprintf(saida,"[%s] 0\n",(*tarefas)[j].nome);
        }
    }

    fclose(saida);

}
