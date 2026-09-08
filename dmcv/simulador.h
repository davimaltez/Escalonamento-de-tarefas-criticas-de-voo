#ifndef SIMULADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dados_tarefas.h"


void verificar_deadlines_vencidas(Tarefa ** tarefas, int total_tarefas, int t_atual);
void verificar_chegadas_novas(Tarefa ** tarefas, int total_tarefas, int t_atual);
int tarefa_a_ser_executada_RM(Tarefa ** tarefas, int total_tarefas);
void RM(Tarefa ** tarefas, int total_tarefas, int tempo_total);

#endif