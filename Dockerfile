FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /app

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN cmake -S . -B build && cmake --build build

CMD ["./build/sim_tempo_real"]
