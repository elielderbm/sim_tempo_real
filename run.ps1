# run.ps1 - PowerShell equivalente do run.sh

param (
    [string]$argsLine = "",
    [switch]$delete
)

$ImageName = "sim_tempo_real"

function Show-Usage {
    Write-Host "`nUSO:"
    Write-Host "  ./run.ps1 [jd|j|d]         → Executa o container com os parâmetros"
    Write-Host "  ./run.ps1 --delete         → Remove a imagem Docker existente"
    Write-Host "  ./run.ps1 -?               → Mostra esta mensagem"
    exit
}

# Mostrar ajuda
if ($argsLine -eq "--help" -or $argsLine -eq "-h" -or $argsLine -eq "-?") {
    Show-Usage
}

# Excluir imagem, se solicitado
if ($delete -or $argsLine -eq "--delete") {
    Write-Host "[INFO] Removendo imagem Docker '$ImageName'..."
    docker rmi $ImageName -f
    exit
}

# Verifica se a imagem existe
$imageExists = docker images -q $ImageName

if (-not $imageExists) {
    Write-Host "[INFO] Imagem '$ImageName' não encontrada. Construindo..."
    docker build -t $ImageName .
} else {
    Write-Host "[INFO] Imagem '$ImageName' já existe."
}

# Executar container
Write-Host "[INFO] Executando container com argumentos: $argsLine"
docker run --rm --init -it $ImageName $argsLine
