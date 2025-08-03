#include "weather.h"
#include <stdlib.h>

float obter_temperatura_sao_paulo() {
    return 20 + rand() % 30; // Simula temperatura entre 20°C e 50°C
}