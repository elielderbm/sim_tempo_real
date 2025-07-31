#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdatomic.h>
#include <sys/select.h>
#include <fcntl.h>

#include "weather.h"

#define INTERVALO_MS 100
#define LIMITE_TEMPERATURA 40.0

float temperatura_atual = 25.0;
int simular_jitter = 0;
int simular_delay_api = 0;
pthread_mutex_t mutex_temp;
atomic_int encerrando = 0;

void handle_sigint(int sig) {
    printf("\n[SINAL] SIGINT (%d) recebido. Encerrando...\n", sig);
    encerrando = 1;
}

// Entrada com timeout (não bloqueante)
int input_disponivel() {
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0;
}

void* tarefa_entrada_terminal(void* arg) {
    char buffer[64];
    while (!encerrando) {
        if (input_disponivel()) {
            if (fgets(buffer, sizeof(buffer), stdin)) {
                float nova_temp = atof(buffer);
                pthread_mutex_lock(&mutex_temp);
                temperatura_atual = nova_temp;
                pthread_mutex_unlock(&mutex_temp);
                printf("[ENTRADA] Temperatura manual: %.2f°C\n", nova_temp);
            }
        } else {
            usleep(100000); // 100ms
        }
    }
    printf("[THREAD] Entrada encerrada.\n");
    return NULL;
}

void* tarefa_weather(void* arg) {
    while (!encerrando) {
        if (simular_delay_api) {
            int delay = 500 + rand() % 2000;
            printf("[DELAY API SIMULADO] Aguardando %dms...\n", delay);
            usleep(delay * 1000);
        }

        float temp_ext = obter_temperatura_sao_paulo();
        if (temp_ext > -99.0) {
            pthread_mutex_lock(&mutex_temp);
            temperatura_atual = temp_ext;
            pthread_mutex_unlock(&mutex_temp);
            printf("[WEATHER API] São Paulo: %.2f°C\n", temp_ext);
        } else {
            printf("[WEATHER API] Erro ao obter temperatura!\n");
        }

        for (int i = 0; i < 30 && !encerrando; i++) sleep(1);
    }

    printf("[THREAD] API encerrada.\n");
    return NULL;
}

void* tarefa_controle(void* arg) {
    struct timespec inicio, esperado, atual, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    esperado = inicio;

    int ciclo = 0;

    while (!encerrando) {
        clock_gettime(CLOCK_MONOTONIC, &atual);

        pthread_mutex_lock(&mutex_temp);
        float temp = temperatura_atual;
        pthread_mutex_unlock(&mutex_temp);

        printf("\n[CICLO %d]\n", ciclo++);
        printf("[INFO] Temperatura Atual: %.2f°C\n", temp);

        if (temp > LIMITE_TEMPERATURA)
            printf("[LED ON] Temperatura ALTA!\n");
        else
            printf("[LED OFF] Temperatura OK.\n");

        clock_gettime(CLOCK_MONOTONIC, &fim);
        long exec_ms = (fim.tv_sec - atual.tv_sec) * 1000 +
                       (fim.tv_nsec - atual.tv_nsec) / 1000000;

        if (simular_jitter) {
            int jitter_ms = rand() % 150;
            usleep(jitter_ms * 1000);
            printf("[JITTER SIMULADO] +%dms\n", jitter_ms);
        }

        struct timespec antes_sleep;
        clock_gettime(CLOCK_MONOTONIC, &antes_sleep);

        long delta_ms = (antes_sleep.tv_sec - esperado.tv_sec) * 1000 +
                        (antes_sleep.tv_nsec - esperado.tv_nsec) / 1000000;

        printf("[MÉTRICAS] Execução: %ldms | Atraso: %ldms | Previsto: +%dms\n",
               exec_ms, delta_ms, INTERVALO_MS);

        esperado.tv_nsec += INTERVALO_MS * 1000000;
        if (esperado.tv_nsec >= 1000000000) {
            esperado.tv_sec += 1;
            esperado.tv_nsec -= 1000000000;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &esperado, NULL);
    }

    printf("[THREAD] Controle encerrado.\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    signal(SIGINT, handle_sigint);
    pthread_mutex_init(&mutex_temp, NULL);

    for (int i = 1; i < argc; ++i) {
        if (strchr(argv[i], 'j')) simular_jitter = 1;
        if (strchr(argv[i], 'd')) simular_delay_api = 1;
    }

    printf("[MODO] %s Jitter | %s Delay API\n",
           simular_jitter ? "Com" : "Sem",
           simular_delay_api ? "Com" : "Sem");

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, tarefa_controle, NULL);
    pthread_create(&t2, NULL, tarefa_weather, NULL);
    pthread_create(&t3, NULL, tarefa_entrada_terminal, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mutex_temp);
    printf("[MAIN] Finalizado com sucesso.\n");
    return 0;
}
