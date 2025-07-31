#!/bin/bash

IMAGE_NAME="sim_tempo_real"

# Função para mostrar uso
usage() {
    echo "Uso:"
    echo "  ./run.sh [jd|j|d]           → Executa o container com os parâmetros"
    echo "  ./run.sh --delete           → Remove a imagem Docker existente"
    echo "  ./run.sh --help             → Mostra esta mensagem"
    exit 1
}

# Excluir imagem, se solicitado
if [[ "$1" == "--delete" ]]; then
    echo "[INFO] Removendo imagem Docker: $IMAGE_NAME"
    docker rmi "$IMAGE_NAME"
    exit 0
fi

# Mostrar ajuda
if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    usage
fi

# Verifica se a imagem existe
if ! docker image inspect "$IMAGE_NAME" > /dev/null 2>&1; then
    echo "[INFO] Imagem não encontrada. Construindo imagem '$IMAGE_NAME'..."
    docker build -t "$IMAGE_NAME" .
else
    echo "[INFO] Imagem '$IMAGE_NAME' já existe."
fi

# Parâmetros para o programa (j, d, jd etc.)
ARGS="$1"

echo "[INFO] Executando o container com argumentos: $ARGS"
docker run --rm --init -it "$IMAGE_NAME" $ARGS
