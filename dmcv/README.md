# Escalonamento-de-tarefas-criticas-de-voo


Simulador em C para comparação de dois algoritmos clássicos de escalonamento preemptivo em tempo real (*hard real-time*): **Rate-Monotonic (RM)** e **Earliest Deadline First (EDF)**. O projeto simula o comportamento da CPU no processamento de tarefas aviônicas periódicas e críticas.

## Descrição

O programa lê um cenário de execução a partir de um arquivo de texto, simula a alocação de CPU unidade a unidade de tempo ($u.t.$) e gera um arquivo de log com o histórico da execução. 

Principais características da simulação:
* **Preempção Rígida:** Se uma tarefa de maior prioridade fica pronta, a tarefa atual é interrompida imediatamente e retoma de onde parou quando voltar à CPU.
* **Desempate:** Em caso de igualdade de prioridade, a preferência é da tarefa que aparece primeiro no arquivo de entrada.
* **Perda de Deadline (*Deadline Miss*):** Se uma tarefa atinge seu deadline absoluto sem concluir a rajada ($burst$), a instância é descartada no exato instante do estouro. A tarefa só volta a concorrer quando chegar seu próximo período.

## Compilação

A compilação é automatizada via `Makefile`:

```bash
make

```

Isso gera o executável scheduler

Para remover os arquivos objetos e executáveis gerados:

```bash
make clean

```

## Requisitos

1\. Compilador gcc

1\. Utilitário make

1\. Ambiente Linux/Unix ou macOS

## Uso

```bash

./scheduler <algoritmo> <arquivo_de_entrada>

```

| Parâmetro | Valores Aceitos | Descrição |
| --------- | --------------- | --------- |
| Algoritmo | rate ou edf     | Define a política de prioridade (Rate-Monotonic ou Earliest Deadline First)|
| Arquivo Entrada | caminho do .txt | Arquivo contendo a duração da simulação e o conjunto de tarefas |

Exemplos: 

```bash

./scheduler rate voo.txt
./scheduler edf voo.txt

```

Formato do arquivo de entrada: 

[TEMPO TOTAL]  
[NOME] [PERÍODO] [DEADLINE] [BURST]

Todos os valores numéricos devem ser inteiros positivos respeitando a regra C <= D <= P(Burst, Deadline, Período)

Exemplo(voo.txt):

100  
ATT 20 12 8  
NAV 50 30 15  

Saída Gerada: 

O programa roda de forma silenciosa na saída padrão (stdout). Os resultados da execução são gravados diretamente no arquivo correspondente ao algoritmo escolhido:


| Comando | Arquivo Gerado |
| :--- | :--- |
| `./scheduler rate voo.txt` | `rate_dmcv.out` |
| `./scheduler edf voo.txt` | `edf_dmcv.out` |

(Onde dmcv são as iniciais do autor)

Erros de leitura ou argumentos inválidos são direcionados para a saída de erro padrão (stderr)

## Estrutura do Código

1\. main.c: Ponto de entrada, validação de argumentos e orquestração do fluxo

1\. leitor.c / leitor.h: Leitura e validação sintática e semântica do arquivo de entrada

1\. simulador.c / simulador.h: Loop principal de simulação e aplicação das regras de preempção e perda de deadline

1\. dados_tarefas.h: Definição da struct da tarefa e seus estados de controle

1\. Makefile: Script de compilação do projeto

## Autor

__Davi Maltez Costa Vasconcelos__