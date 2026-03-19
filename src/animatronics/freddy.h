#pragma once
#include <stdint.h>
#include "../core/game_context.h"
#include "../core/rng.h"

// Ruta: Show Stage → Dining Room → Restrooms → E. Hall Corner → Right Door
// Fuente: JS PATHS.freddy = ["1A","1B","7","4B","right-door"]
// IMPORTANTE: Cam 6 (Kitchen) es solo audio — Freddy no aparece visualmente allí.
// En esta implementación Kitchen no está en la ruta porque no tenemos audio aún.

enum class FreddyPos : uint8_t
{
    SHOW_STAGE  = 0,  // Cam 1A
    DINING_ROOM = 1,  // Cam 1B
    RESTROOMS   = 2,  // Cam 7
    EAST_CORNER = 3,  // Cam 4B
    RIGHT_DOOR  = 4,  // fuera de cámaras
};

static constexpr uint8_t FREDDY_CAM_ID[] = {1, 1, 7, 4, 0};

// Freddy también tiene un modo especial cuando se va la luz (power_out).
// En ese caso aparece en la oficina y suena el Toreador March antes del jumpscare.
enum class FreddyMode : uint8_t
{
    NORMAL,      // comportamiento estándar
    POWER_OUT,   // luz apagada, esperando para atacar
};

class Freddy
{
public:
    void init(uint8_t ai_level);
    void reset();
    void update(GameContext& ctx);

    bool at_right_door()   const;
    bool in_power_out_mode() const { return _mode == FreddyMode::POWER_OUT; }

    // Llamado desde Game cuando power_out se activa
    void trigger_power_out();

private:
    void update_normal(GameContext& ctx);
    void update_power_out(GameContext& ctx);
    void try_move(GameContext& ctx);
    void update_attack(GameContext& ctx);

    uint16_t attack_threshold() const
    {
        uint8_t effective = (uint8_t)(30 - _ai_level);
        return (uint16_t)effective * 30;
    }

private:
    uint8_t    _ai_level     = 0;
    FreddyPos  _pos          = FreddyPos::SHOW_STAGE;
    FreddyMode _mode         = FreddyMode::NORMAL;

    uint16_t   _move_timer   = 0;
    uint16_t   _attack_timer = 0;

    // Timer para el power-out: Freddy espera entre 10-20s antes de atacar.
    // Fuente: JS CONFIG.POWEROUT_MIN_DELAY=10000ms, POWEROUT_MAX_DELAY=20000ms
    // En frames: 600–1200
    uint16_t   _powerout_timer    = 0;
    uint16_t   _powerout_threshold = 0;  // se sortea al entrar en este modo

    RNG        _rng{13};

    // Fuente: JS CONFIG.AI_INTERVALS.freddy = 3.02s × 60 FPS
    // Freddy es el más rápido en sus intentos de movimiento
    static constexpr uint16_t MOVE_INTERVAL = 181;

    // Rango del delay de power-out en frames (600 = 10s, 1200 = 20s a 60FPS)
    static constexpr uint16_t POWEROUT_MIN = 600;
    static constexpr uint16_t POWEROUT_MAX = 1200;
};