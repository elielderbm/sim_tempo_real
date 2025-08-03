# sim_tempo_real

Simulação de um sistema de tempo real com múltiplas tarefas em C, utilizando `pthread`, `libcurl`, `CMake` e executado em ambiente Docker (Ubuntu 24.04). A aplicação simula:

- Controle periódico com deadline (tarefa de 100ms)
- Jitter artificial opcional
- Aquisição de temperatura real da cidade de São Paulo (via OpenWeatherMap API)
- Atraso de rede simulado na tarefa da API

---

## 🔧 Pré-requisitos

- Docker Desktop instalado
- Chave de API gratuita do [OpenWeatherMap](https://openweathermap.org/api)
- Ambiente compatível com shell: PowerShell, Git Bash, ou WSL

---

## 🔑 Configurar a chave da API

Antes de executar, você precisa definir a chave da API como uma variável de ambiente chamada `API_KEY`.


## 📦 Como executar

### 🟢 Git Bash / WSL / Linux (run.sh)

```bash
chmod +x run.sh         # Só precisa fazer isso uma vez
./run.sh                # Executa sem jitter ou delay
./run.sh j              # Executa com jitter
./run.sh d              # Executa com delay
./run.sh jd             # Executa com jitter e delay
./run.sh --delete       # Remove a imagem Docker
./run.sh --help         # Mostra ajuda
```


## 📂 Estrutura do Projeto

```
sim_tempo_real/
├── src/
│   └── ... (código-fonte em C)
├── CMakeLists.txt
├── Dockerfile
├── run.sh          # Script para Unix-like (Bash)
├── run.ps1         # Script para PowerShell (Windows)
└── README.md
```

---

## 📌 Observações

- A imagem Docker usa Ubuntu 24.04 e instala automaticamente todas as dependências (`cmake`, `libcurl`, etc.).
- O programa simula uma aplicação de tempo real com prioridades, jitter, delays e coleta de temperatura real via rede.
- Os parâmetros `j`, `d`, e `jd` controlam o comportamento simulado da tarefa crítica.
