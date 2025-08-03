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


## 📦 Como executar

### 🔵 PowerShell (Windows) – run.ps1

Se o WSL não estiver instalado, execute:
```
wsl --update
```

```powershell
# Defina a variável da API
$env:API_KEY = "sua_chave_aqui"

# Executar a aplicação
.\run.ps1               # Executa sem jitter ou delay
.\run.ps1 j             # Executa com jitter
.\run.ps1 d             # Executa com delay
.\run.ps1 jd            # Executa com jitter e delay
.\run.ps1 --delete      # Remove a imagem Docker
.\run.ps1 --help        # Mostra ajuda
```

> ⚠️ Se você receber um erro de permissão, execute:
> ```powershell
> Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
> ```

---

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
