typedef struct {
    char nome[50];
    int periodo;
    int deadline;
    int burst;

    int chegada_atual;
    int restante;
    int deadline_absoluta; //chegada_atual + deadline
    int concluida;
    int perdidas;
} Tarefa;

Tarefa * tarefas;
void ler_entrada(int argc, char *argv[]);