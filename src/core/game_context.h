#pragma once

#include <stdint.h>

enum class NightType : uint8_t
{
    NORMAL,
    NIGHT_6,
    CUSTOM
};

enum class GameView : uint8_t
{
    OFFICE_LEFT,
    OFFICE_CENTER,
    OFFICE_RIGHT,
    CAMERAS
};

// Estado público de cada animatrónico — lo leen las cámaras y el renderer
struct AnimatronicState
{
    uint8_t camera;         // ID de cámara donde está (0 = no visible)
    bool    at_left_door;
    bool    at_right_door;
};

struct GameContext
{
    // Tipo de noche
    NightType night_type;

    // Progreso
    uint8_t night;      // 1–6 (7 = custom)
    uint8_t hour;       // 0–6  (0 = 12 AM)

    // Energía
    uint8_t  power;         // 0–100
    uint8_t  power_usage;   // consumo actual (0–5 aprox)
    bool     power_out;     // se quedó sin energía

    // Vista actual
    GameView current_view;
    uint8_t current_camera;     // índice de cámara activa
    bool cameras_open;

    // Oficina
    bool left_door_closed;
    bool right_door_closed;
    bool left_light_on;
    bool right_light_on;

    // Estado de animatrónicos (para cámaras y renderer)
    AnimatronicState bonnie;
    AnimatronicState chica;
    AnimatronicState foxy;
    AnimatronicState freddy;

    // Estado general
    bool game_over;
    bool win;
};