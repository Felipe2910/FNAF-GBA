#include "foxy.h"

void Foxy::init(uint8_t ai_level)
{
    _ai_level = (ai_level > 20) ? 20 : ai_level;
    reset();
}

void Foxy::reset()
{
    _phase        = FoxyPhase::CURTAIN_CLOSED;
    _state        = FoxyState::IN_COVE;
    _check_timer  = 0;
    _run_timer    = 0;
    _return_timer = 0;
    _watched      = false;
}

void Foxy::update(GameContext& ctx)
{
    if (ctx.game_over || ctx.win)
        return;

    // Marcar si el jugador está mirando Pirate Cove este frame.
    // La cámara 1C (Pirate Cove) tiene ID especial — usamos camera==0 como
    // "no visible" en el sistema general, pero Foxy vive siempre en 1C.
    // El OfficeScreen o el sistema de cámaras debe escribir esto al contexto.
    // Por ahora lo leemos del campo dedicado.
    _watched = (ctx.current_view == GameView::CAMERAS &&
                ctx.current_camera == 0);  // 0 = índice de Cam 1C en tu enum

    switch (_state)
    {
        case FoxyState::IN_COVE:      update_cove(ctx);     break;
        case FoxyState::RUNNING:      update_running(ctx);  break;
        case FoxyState::RETURNING:    update_returning();   break;
    }

    // Exponer al contexto
    ctx.foxy.at_left_door = (_state == FoxyState::RUNNING &&
                             _run_timer >= RUN_DURATION);
}

// ---------------------------------------------------------------------------
void Foxy::update_cove(GameContext& ctx)
{
    if (_ai_level == 0)
        return;

    _check_timer++;

    if (_check_timer >= CHECK_INTERVAL)
    {
        _check_timer = 0;

        // Si el jugador NO lo estaba mirando → puede avanzar de fase
        if (!_watched)
        {
            uint8_t roll = (uint8_t)(_rng.next() % 20);
            if (roll < _ai_level && _phase < FoxyPhase::CURTAIN_OPEN)
            {
                _phase = static_cast<FoxyPhase>(static_cast<uint8_t>(_phase) + 1);
            }
        }
        // Mirarlo demasiado tampoco es bueno: en el original, observarlo
        // repetidamente lo hace más agresivo en noches avanzadas.
        // Por ahora lo dejamos sin penalización — se puede añadir después.

        _watched = false;
    }

    // Si llegó a CURTAIN_OPEN, arranca el sprint
    if (_phase == FoxyPhase::CURTAIN_OPEN)
    {
        _state     = FoxyState::RUNNING;
        _run_timer = 0;
    }
}

// ---------------------------------------------------------------------------
void Foxy::update_running(GameContext& ctx)
{
    _run_timer++;

    if (_run_timer < RUN_DURATION)
        return; // todavía en tránsito

    // Llegó a la puerta izquierda
    if (ctx.left_door_closed)
    {
        // Puerta cerrada → drena energía y vuelve
        uint8_t dmg = door_damage();
        if (dmg >= ctx.power)
            ctx.power = 0;
        else
            ctx.power -= dmg;

        _state        = FoxyState::RETURNING;
        _return_timer = 0;
        _phase        = FoxyPhase::CURTAIN_CLOSED; // vuelve a empezar
    }
    else
    {
        // Puerta abierta → jumpscare
        ctx.game_over = true;
    }
}

// ---------------------------------------------------------------------------
void Foxy::update_returning()
{
    _return_timer++;
    if (_return_timer >= RETURN_DURATION)
    {
        _state     = FoxyState::IN_COVE;
        _run_timer = 0;
    }
}

bool Foxy::at_left_door() const
{
    return (_state == FoxyState::RUNNING && _run_timer >= RUN_DURATION);
}