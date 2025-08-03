# sim_tempo_real

Simulação de um sistema de tempo real em C com ESP32 e FreeRTOS.

A aplicação simula:

- Controle periódico com deadline (tarefa de 100ms)
- Jitter artificial opcional
- Atraso artificial opcional


## 🔧 Pré-requisitos

- VS Code instalado
- Extensão ESP-IDF (v5.4.2)
- Criação de novo projeto com ESP32
- ESP-IDF instalado e configurado (`idf.py`)
- ESP32 devidamente conectado


## 🎯 Funcionalidades

- Tarefa periódica de controle (100ms)
- Tarefa de leitura de temperatura externa simulada
- Tarefa de entrada UART (sem bloqueio)
- Mutex para sincronização
- Simulação de jitter e delay
- Log por `ESP_LOG`


## ⌨️ Entrada manual

Digite uma temperatura no terminal e pressione ENTER para atualizar o valor atual. Ex:

```
45
```

## ⚙️ Configuração interna

Você pode ativar/desativar jitter e delay diretamente no `main.c`:

```
simular_jitter = true;
simular_delay_api = true;
```


## 📦 Como executar

- No VS Code, substitua a pasta main do projeto criado (pode ser um exemplo hello_world), pela pasta main deste repositório.
- Compile, grave e veja o monitor através dos botões rápidos da extensão do VS Code


## 📂 Estrutura

```
esp32_sim_tempo_real/
├── main/
│   ├── main.c
│   ├── weather.c
│   └── weather.h
├── README.md
```
