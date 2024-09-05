#include <stdio.h>
#include <json-c/json.h>
#include <math.h>
#include <string.h>


typedef struct {
    char id[10];
    int period;
    int execution_time;
    double utilization;
} Task;

void swap(Task *xp, Task *yp) {
    // Função que troca a posição de duas tarefas
    Task temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void ordenar_tarefas(Task tasks[], int n) {
    // Esta função realiza a ordenação das tarefas
    // Tarefas com períodos menores recebem maior prioridade
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (tasks[j].period > tasks[j+1].period)
                swap(&tasks[j], &tasks[j+1]);
        }
    }
}

double calcula_utilizacao(Task tasks[], int n) {
    // Esta função recebe uma lista de struct Task e calcula a 
    // taxa total de utilização
    double total_utilization = 0;
    for (int i = 0; i < n; i++) {
        tasks[i].utilization = (double) tasks[i].execution_time / tasks[i].period;
        total_utilization += tasks[i].utilization;
    }
    return total_utilization;
}

int main() {
    // ------------------------------- LEITURA E ANÁLISE DO ARQUIVO JSON --------------------------------------------

    FILE *fp; // Apontador para arquivo, para manipular o arquivo JSON
    struct json_object *parsed_json; // Apontador para o objeto JSON
    struct json_object *tasks_array; // Apontador para lista de tarefas
    struct json_object *task_obj; // Apontador para uma tarefa especifica
    struct json_object *id, *period, *execution_time; // Apontadores para as informações das tarefas
    
    size_t n_tasks;
    size_t i;

    // Abertura do arquivo json
    fp = fopen("tasks.json", "r");
    if (fp == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo.\n");
        return 1;
    }

    // Leitura do arquivo json
    char buffer[2048];
    fread(buffer, sizeof(buffer), 1, fp); // Salva as informações no buffer
    fclose(fp);

    parsed_json = json_tokener_parse(buffer); // Converte os dados em um objeto JSON 
    json_object_object_get_ex(parsed_json, "tasks", &tasks_array); // Extrai as tarefas 
    n_tasks = json_object_array_length(tasks_array); // Aramazena o número de tarefas


    // --------------------- PREENCHIMENTO E LEITURA DA STRUCT TASK ----------------------------------

    Task tasks[n_tasks];  // Cria o array de structs referente às tarefas 

    // Loop para percorrer as tarefas para extrar as informações 
    for (i = 0; i < n_tasks; i++) {
        task_obj = json_object_array_get_idx(tasks_array, i);  // recebe a task atual

        json_object_object_get_ex(task_obj, "id", &id);
        json_object_object_get_ex(task_obj, "period", &period);
        json_object_object_get_ex(task_obj, "execution_time", &execution_time);

        strcpy(tasks[i].id, json_object_get_string(id));
        tasks[i].period = json_object_get_int(period);
        tasks[i].execution_time = json_object_get_int(execution_time);
    }

    // ------------------------------ CÁLCULO DE UTILIZAÇÃO -------------------------------------

    double utilization = calcula_utilizacao(tasks, n_tasks);           // Cálculo da utilização pelas tarefas
    double utilization_limit = n_tasks * (pow(2, 1.0/n_tasks) - 1);   // Cálculo do limite de utilização

    ordenar_tarefas(tasks, n_tasks); // Orndeação das tarefas

    struct json_object *output_json = json_object_new_object();  // Criação do objeto JSON de saída

    // Verifica se a utilização é menor ou igual ao limite e retorna a viabilidade em string
    struct json_object *schedulability = json_object_new_string(utilization <= utilization_limit ? "viable" : "not viable"); 
    json_object_object_add(output_json, "schedulability", schedulability);  // Adiciona a informação de viabilidade ao objeto JSON de saída

    // ------------------------------------ ORDENAÇÃO -------------------------------------------

    // Criação da sugestão de escalonamento, onde a prioridade das tarefas é determinada pela ordem resultante da função de ordenação. 
    // Cada tarefa recebe uma prioridade, que é baseada em sua posição após a ordenação.

    struct json_object *suggested_schedule = json_object_new_array();
    for (i = 0; i < n_tasks; i++) {
        struct json_object *task_json = json_object_new_object(); // Um objeto JSON vazio é criado para armazenar as informações da tarefa atual
        json_object_object_add(task_json, "id", json_object_new_string(tasks[i].id));
        json_object_object_add(task_json, "priority", json_object_new_int(i + 1));
        json_object_array_add(suggested_schedule, task_json); // Adiciona à tarefa ao JSON de sugestão na ordem que está no arrayy
    }
    json_object_object_add(output_json, "suggested_schedule", suggested_schedule); // Adiciona a sugestão de escalonamento ao JSON de saída

    // ---------------------------------- SAÍDA -------------------------------------

    printf("%s\n", json_object_to_json_string(output_json)); // Exibe o JSON de saída

    // Arquivo JSON de saída 
    FILE *fp_saida = fopen("dados.json", "w+");  // "w+" cria o arquivo se ele não existir
    if (fp_saida == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Escrever o JSON no arquivo
    fprintf(fp_saida, "%s", output_json);
    fclose(fp_saida);

    json_object_put(parsed_json); 
    json_object_put(output_json);

    return 0;
}
