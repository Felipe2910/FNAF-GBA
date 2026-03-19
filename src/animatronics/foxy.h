#pragma once
#include <stdint.h>
#include "../core/game_context.h"
#include "../core/rng.h"

// Foxy tiene dos estados ortogonales:
//   FoxyPhase — qué tan abierta está la cortina de Pirate Cove
//   FoxyState — si está en Cove, corriendo o regresando
//
// Fuente: JS class Foxy, CONFIG.FOXY_CHECK_INTERVAL=12s, FOXY_BASE_DAMAGE=5

enum class FoxyPhase : uint8_t
{
    CURTAIN_CLOSED = 0,  // "1C" cerrado, no se ve
    PEEKING_1      = 1,  // asoma un poco
    PEEKING_2      = 2,  // asoma bastante
    CURTAIN_OPEN   = 3,  // cortina abierta, listo para correr
};

enum class FoxyState : uint8_t
{
    IN_COVE,    // dentro de Pirate Cove (1C)
    RUNNING,    // corriendo hacia la puerta izquierda
    RETURNING,  // regresando tras ser bloqueado o tras jumpscare
};

class Foxy
{
public:
    void init(uint8_t ai_level);
    void reset();
    void update(GameContext& ctx);

    FoxyPhase phase() const { return _phase; }
    FoxyState state() const { return _state; }
    bool      at_left_door() const;

private:
    void update_cove(GameContext& ctx);
    void update_running(GameContext& ctx);
    void update_returning();

    uint8_t   _ai_level    = 0;
    FoxyPhase _phase       = FoxyPhase::CURTAIN_CLOSED;
    FoxyState _state       = FoxyState::IN_COVE;

    uint16_t  _check_timer  = 0;  // timer para avanzar fase
    uint16_t  _run_timer    = 0;  // duración del sprint (frames hasta llegar a puerta)
    uint16_t  _return_timer = 0;  // frames hasta volver a Cove tras ser bloqueado
    bool      _watched      = false; // ¿el jugador lo miró este tick?

    RNG       _rng{99};

    // Fuente: JS CONFIG.FOXY_CHECK_INTERVAL = 12s × 60 FPS
    static constexpr uint16_t CHECK_INTERVAL = 720;

    // Foxy tarda ~25 frames en llegar a la puerta desde que empieza a correr.
    // En el original es prácticamente instantáneo — si abres la cámara lo ves
    // llegar en el pasillo y tienes < 1 segundo para cerrar.
    static constexpr uint16_t RUN_DURATION = 25;

    // Tiempo que tarda en volver a Cove tras ser bloqueado (JS: reset inmediato,
    // aquí damos 180 frames = 3s para coherencia visual con las cámaras)
    static constexpr uint16_t RETURN_DURATION = 180;

    // Daño a la energía cuando la puerta está cerrada y Foxy llega.
    // Fuente: JS FOXY_BASE_DAMAGE=5 + ai_level*0.5 → en enteros: 5 + ai_level/2
    uint8_t door_damage() const { return 5 + (_ai_level >> 1); }
};