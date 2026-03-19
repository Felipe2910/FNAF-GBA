#include "power.h"

Power::Power(uint8_t night)
    : _power(100),
      _night_idx(night >= 1 && night <= 6 ? night - 1 : 0),
      _frame_accum(0)
{
}

void Power::reset()
{
    _power       = 100;
    _frame_accum = 0;
}

void Power::update(uint8_t usage)
{
    if (_power == 0)
        return;

    // El tick base es "bajar 1%" cada BASE_DRAIN_FRAMES[noche] frames.
    // Con múltiples items activos el intervalo se divide por el multiplicador,
    // lo que equivale a multiplicar la frecuencia de ticks.
    //
    // Para evitar float hacemos:
    //   intervalo_efectivo = BASE_DRAIN_FRAMES[noche] * 10 / MULT_NUM[usage_clamped]
    //
    // Ejemplo noche 1, usage=2 (2 items activos):
    //   578 * 10 / 28 = 5780 / 28 ≈ 206 frames entre ticks  (vs 578 en reposo)

    const uint8_t  usage_idx = (usage > 4) ? 4 : usage;
    const uint16_t base      = BASE_DRAIN_FRAMES[_night_idx];
    const uint16_t interval  = (uint16_t)((uint32_t)base * 10 / MULT_NUM[usage_idx]);

    _frame_accum++;

    if (_frame_accum >= interval)
    {
        _frame_accum = 0;
        _power--;
    }
}

uint8_t Power::value() const
{
    return _power;
}

bool Power::is_empty() const
{
    return _power == 0;
}