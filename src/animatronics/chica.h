#pragma once
#include <stdint.h>
#include "../core/game_context.h"
#include "../core/rng.h"

// Ruta: Show Stage → Dining Room → Restrooms → East Hall → E. Hall Corner → Right Door
// Fuente: JS PATHS.chica = ["1A","1B","7","6","4A","4B","right-door"]
enum class ChicaPos : uint8_t
{
    SHOW_STAGE    = 0,  // Cam 1A
    DINING_ROOM   = 1,  // Cam 1B
    RESTROOMS     = 2,  // Cam 7
    KITCHEN       = 3,  // Cam 6  (solo audio, nunca se ve)
    EAST_HALL     = 4,  // Cam 4A
    EAST_CORNER   = 5,  // Cam 4B
    RIGHT_DOOR    = 6,  // fuera de cámaras
};

// Mapeo ChicaPos → camera ID visible (0 = no visible / solo audio)
static constexpr uint8_t CHICA_CAM_ID[] = {1, 1, 7, 0, 4, 4, 0};

class Chica
{
public:
    void init(uint8_t ai_level);
    void reset();
    void update(GameContext& ctx);

    bool at_right_door() const;

private:
    void try_move();
    void move_forward();
    void update_attack(GameContext& ctx);

    uint16_t attack_threshold() const
    {
        // Igual que Bonnie: (15 - ai_level * 0.5) * 60 frames en enteros
        uint8_t effective = (uint8_t)(30 - _ai_level);
        return (uint16_t)effective * 30;
    }

private:
    uint8_t   _ai_level     = 0;
    ChicaPos  _pos          = ChicaPos::SHOW_STAGE;
    uint16_t  _move_timer   = 0;
    uint16_t  _attack_timer = 0;
    RNG       _rng{77};

    // Fuente: JS CONFIG.AI_INTERVALS.chica = 4.98s × 60 FPS
    static constexpr uint16_t MOVE_INTERVAL = 299;
};