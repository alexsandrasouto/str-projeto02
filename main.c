#include <stdio.h>
#include <json-c/json.h>
#include <math.h>
#include <string.h>


typedef struct {
    char id[10];
    int period;
    int execution_time;
    double utilization;
    int priority;
} Task;

void order_tasks(Task tasks[], int n) {
    // Esta função realiza a ordenação das tarefas, periodos menores primeiro
    int i, j;
    Task aux;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (tasks[j].period > tasks[j+1].period){
                aux = tasks[j];
                tasks[j] = tasks[j+1];
                tasks[j+1] = aux;
            }
        }
    }
}


void set_priority(Task tasks[], int n){
    // Esta função atribui a prioridade das tarefas ordenadas por periodo de forma crescente
    // Tarefas com períodos menores recebem maior prioridade
    int i;
    tasks[0].priority = 1;
    for (i = 1; i < n; i++){
        if (tasks[i].period == tasks[i-1].period)
            tasks[i].priority = tasks[i-1].priority;
        else 
            tasks[i].priority = i + 1;
    }

}

double calculate_utilization(Task tasks[], int n) {
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
    struct json_object *parsed_json; 
    struct json_object *tasks_array; 
    struct json_object *task_obj; 
    struct json_object *id, *period, *execution_time;
    
    size_t n_tasks;
    size_t i;

    // Abertura do arquivo json
    fp = fopen("tasks2.json", "r");
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

    // Cria o array de structs referente as tarefas 
    Task tasks[n_tasks];

    // Loop para percorrer as tarefas para extrar as informações 
    for (i = 0; i < n_tasks; i++) {
        task_obj = json_object_array_get_idx(tasks_array, i);

        json_object_object_get_ex(task_obj, "id", &id);
        json_object_object_get_ex(task_obj, "period", &period);
        json_object_object_get_ex(task_obj, "execution_time", &execution_time);

        strcpy(tasks[i].id, json_object_get_string(id));
        tasks[i].period = json_object_get_int(period);
        tasks[i].execution_time = json_object_get_int(execution_time);
    }

    // ------------------------------ CÁLCULO DE UTILIZAÇÃO -------------------------------------

    double utilization = calculate_utilization(tasks, n_tasks);           // Cálculo da utilização pelas tarefas
    double utilization_limit = n_tasks * (pow(2, 1.0/n_tasks) - 1);   // Cálculo do limite de utilização

    order_tasks(tasks, n_tasks); // Orndeação das tarefas
    set_priority(tasks, n_tasks); // Atribui a priporidade das tarefas

    struct json_object *output_json = json_object_new_object();  // Criação do objeto JSON de saída

    // Verifica se a utilização é menor ou igual ao limite e retorna a viabilidade em string
    struct json_object *schedulability = json_object_new_string(utilization <= utilization_limit ? "viable" : "not viable"); 
    json_object_object_add(output_json, "schedulability", schedulability);  

    // ------------------------------------ ORDENAÇÃO -------------------------------------------

    // Criação da sugestão de escalonamento, onde a prioridade das tarefas é determinada pela ordem resultante da função de ordenação. 
    // Cada tarefa recebe uma prioridade, que é baseada em sua posição após a ordenação.

    struct json_object *suggested_schedule = json_object_new_array();
    for (i = 0; i < n_tasks; i++) {
        struct json_object *task_json = json_object_new_object();
        json_object_object_add(task_json, "id", json_object_new_string(tasks[i].id));
        json_object_object_add(task_json, "priority", json_object_new_int(tasks[i].priority));
        json_object_array_add(suggested_schedule, task_json); 
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
    const char *json_str = json_object_to_json_string(output_json);
    fprintf(fp_saida, "%s", json_str);
    fclose(fp_saida);

    json_object_put(parsed_json); 
    json_object_put(output_json);

    return 0;
}

