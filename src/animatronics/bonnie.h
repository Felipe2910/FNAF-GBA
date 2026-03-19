#pragma once
#include <stdint.h>
#include "../core/game_context.h"
#include "../core/rng.h"

// Posiciones de Bonnie en el mapa de cámaras.
// El orden importa — move_forward() avanza en esta secuencia.
enum class BonniePos : uint8_t
{
    SHOW_STAGE    = 0,  // Cam 1A — inicio
    BACKSTAGE     = 1,  // Cam 5
    DINING_ROOM   = 2,  // Cam 1B
    WEST_HALL     = 3,  // Cam 2A
    SUPPLY_CLOSET = 4,  // Cam 2B
    LEFT_DOOR     = 5,  // fuera de cámaras — en la puerta
};

// Mapeo BonniePos → índice de cámara visible (0 = no visible en cámaras)
static constexpr uint8_t BONNIE_CAM_ID[] = {1, 5, 1, 2, 2, 0};

class Bonnie
{
public:
    void init(uint8_t ai_level);
    void reset();
    void update(GameContext& ctx);

    bool at_left_door() const;

private:
    void try_move();
    void move_forward();
    void update_attack(GameContext& ctx);

private:
    uint8_t  _ai_level    = 0;
    BonniePos _pos        = BonniePos::SHOW_STAGE;
    uint16_t _move_timer  = 0;
    uint16_t _attack_timer = 0;  // frames desde que está en puerta abierta
    RNG      _rng{42};

    // Fuente: JS CONFIG.AI_INTERVALS.bonnie = 4.97s × 60 FPS
    static constexpr uint16_t MOVE_INTERVAL = 298;

    // Fuente: JS CONFIG.ATTACK_BASE_TIME=15s, ATTACK_AI_MULTIPLIER=0.5
    // threshold = (15 - ai_level * 0.5) * 60 frames
    // Con ai_level=0 → 900 frames (15s), ai_level=20 → 300 frames (5s)
    uint16_t attack_threshold() const
    {
        // Evitamos float: (15*2 - ai_level) * 30 = misma fórmula en enteros
        uint8_t effective = (uint8_t)(30 - _ai_level); // mínimo 10 si ai=20
        return (uint16_t)effective * 30;
    }
};