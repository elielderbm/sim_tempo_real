#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "weather.h"

#define TAG "TEMPO_REAL"
#define UART_NUM UART_NUM_0

#define INTERVALO_MS 100
#define LIMITE_TEMPERATURA 40.0

static SemaphoreHandle_t mutex_temp;
static float temperatura_atual = 25.0;
static bool simular_jitter = true;
static bool simular_delay_api = true;
static bool encerrando = false;

void tarefa_terminal(void *arg) {
    uint8_t buf[64];
    while (!encerrando) {
        int len = uart_read_bytes(UART_NUM, buf, sizeof(buf) - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            buf[len] = 0;
            float temp = atof((char *)buf);
            xSemaphoreTake(mutex_temp, portMAX_DELAY);
            temperatura_atual = temp;
            xSemaphoreGive(mutex_temp);
            ESP_LOGI(TAG, "[ENTRADA] Temp manual: %.2f°C", temp);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}

void tarefa_weather(void *arg) {
    while (!encerrando) {
        if (simular_delay_api) {
            int delay = 500 + rand() % 1500;
            ESP_LOGW(TAG, "[API] Simulando atraso: %dms", delay);
            vTaskDelay(pdMS_TO_TICKS(delay));
        }

        float temp = obter_temperatura_sao_paulo();
        xSemaphoreTake(mutex_temp, portMAX_DELAY);
        temperatura_atual = temp;
        xSemaphoreGive(mutex_temp);

        ESP_LOGI(TAG, "[API] Temp externa: %.2f°C", temp);
        for (int i = 0; i < 30 && !encerrando; i++) vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

void tarefa_controle(void *arg) {
    TickType_t last_wake = xTaskGetTickCount();
    int ciclo = 0;

    while (!encerrando) {
        float temp;
        xSemaphoreTake(mutex_temp, portMAX_DELAY);
        temp = temperatura_atual;
        xSemaphoreGive(mutex_temp);

        ESP_LOGI(TAG, "[CICLO %d] Temp atual: %.2f°C", ciclo++, temp);

        if (temp > LIMITE_TEMPERATURA)
            ESP_LOGW(TAG, "[LED ON] Temperatura ALTA!");
        else
            ESP_LOGI(TAG, "[LED OFF] Temperatura OK.");

        if (simular_jitter) {
            int jitter = rand() % 150;
            vTaskDelay(pdMS_TO_TICKS(jitter));
            ESP_LOGW(TAG, "[JITTER] +%dms", jitter);
        }

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(INTERVALO_MS));
    }

    vTaskDelete(NULL);
}

void app_main(void) {
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);

    mutex_temp = xSemaphoreCreateMutex();
    if (!mutex_temp) {
        ESP_LOGE(TAG, "Erro ao criar mutex");
        return;
    }

    xTaskCreate(tarefa_controle, "controle", 4096, NULL, 2, NULL);
    xTaskCreate(tarefa_weather, "weather", 4096, NULL, 2, NULL);
    xTaskCreate(tarefa_terminal, "entrada", 4096, NULL, 1, NULL);

    ESP_LOGI(TAG, "Sistema iniciado. Use o terminal para enviar temperatura (ENTER para confirmar)");
}