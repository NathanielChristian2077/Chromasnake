#pragma once

class Config {
public:
    // Grid
    static constexpr  int GRID_W = 40;
    static constexpr  int GRID_H = 30;
    static constexpr  int CS = 20;

    // Ticks (velocidade do loop do jogo basicamente)
    static constexpr float BASE_TICK = 0.12f;

    // Atributos base
    static constexpr float SPEED_MULT_BASE = 1.0f;
    static constexpr int VISION_BASE = 6;
    static constexpr float AGGR_BASE = 0.0f;
    static constexpr float CAUT_BASE = 0.0f;

    // Fator de enfraquecimento de cores secundárias
    static constexpr float WEAK_FACTOR = 0.6f;
};
