# sim_tempo_real

Simulação de um sistema de tempo real com múltiplas tarefas em C, utilizando `pthread`, `libcurl`, `CMake` e executado em ambiente Docker (Ubuntu 24.04). A aplicação simula:

- Controle periódico com deadline (tarefa de 100ms)
- Jitter artificial opcional
- Aquisição de temperatura real da cidade de São Paulo (via OpenWeatherMap API)
- Atraso de rede simulado na tarefa da API

---

## 🔧 Pré-requisitos

- Docker instalado na máquina
- Chave de API gratuita do [OpenWeatherMap](https://openweathermap.org/api)

---

## 📦 Como construir

```bash
chmod +x run.sh
```

## Como executar

```bash
./run.sh             # Executa sem jitter ou delay
./run.sh j           # Com jitter
./run.sh d           # Com delay
./run.sh jd          # Com ambos
./run.sh --delete    # Exclui imagem Docker
./run.sh --help      # Mostra ajuda
```
