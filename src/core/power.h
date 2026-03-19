#pragma once
#include <stdint.h>

class Power
{
public:
    explicit Power(uint8_t night);

    void reset();
    void update(uint8_t usage); // 0-5 items activos

    uint8_t value()    const;   // 0-100
    bool    is_empty() const;

private:
    // Drain base por noche expresado como "1 tick de -1% cada N frames".
    // Fuente: JS CONFIG.BASE_POWER_DRAIN convertido:
    //   noche 1: 1/9.6 %/s  → 1 tick cada 578 frames aprox. (9.6 * 60)
    //   noche 2: 1/8.0 %/s  → 1 tick cada 480 frames
    //   noche 3: 1/7.2 %/s  → 1 tick cada 432 frames
    //   noche 4-6: 1/6.4 %/s → 1 tick cada 384 frames
    static constexpr uint16_t BASE_DRAIN_FRAMES[6] = {578, 480, 432, 384, 384, 384};

    // Multiplicador de consumo según número de items activos (clamped a 4).
    // Fuente: JS CONFIG.POWER_MULTIPLIERS {0:1.0, 1:1.6, 2:2.8, 3:4.6, 4:8.0}
    // En GBA evitamos float: representamos como fracción num/den.
    //   1.0 → 10/10,  1.6 → 16/10,  2.8 → 28/10,  4.6 → 46/10,  8.0 → 80/10
    static constexpr uint8_t MULT_NUM[5] = {10, 16, 28, 46, 80};
    // Denominador fijo = 10 para todos, no necesita array.

    uint8_t  _power;        // 0–100
    uint8_t  _night_idx;    // 0-based (noche 1 = índice 0)
    uint16_t _frame_accum;  // frames acumulados desde el último tick
};