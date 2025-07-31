cmake_minimum_required(VERSION 3.10)
project(sim_tempo_real C)

set(CMAKE_C_STANDARD 99)

add_executable(sim_tempo_real
    src/main.c
    src/weather.c
)

find_package(Threads REQUIRED)
target_link_libraries(sim_tempo_real PRIVATE Threads::Threads curl)
