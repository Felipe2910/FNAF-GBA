#include "bonnie.h"

void Bonnie::init(uint8_t ai_level)
{
    _ai_level = (ai_level > 20) ? 20 : ai_level;
    reset();
}

void Bonnie::reset()
{
    _pos          = BonniePos::SHOW_STAGE;
    _move_timer   = 0;
    _attack_timer = 0;
}

void Bonnie::update(GameContext& ctx)
{
    if (ctx.game_over || ctx.win)
        return;

    // --- Movimiento ---
    _move_timer++;
    if (_move_timer >= MOVE_INTERVAL)
    {
        _move_timer = 0;
        try_move();
    }

    // --- Lógica de puerta ---
    if (_pos == BonniePos::LEFT_DOOR)
    {
        if (ctx.left_door_closed)
        {
            // Puerta cerrada: Bonnie espera pero resetea el attack timer
            _attack_timer = 0;
        }
        else
        {
            // Puerta abierta: contar frames hasta jumpscare
            update_attack(ctx);
        }
    }

    // Exponer posición al contexto para las cámaras
    ctx.bonnie.camera      = BONNIE_CAM_ID[static_cast<uint8_t>(_pos)];
    ctx.bonnie.at_left_door = (_pos == BonniePos::LEFT_DOOR);
}

void Bonnie::try_move()
{
    if (_ai_level == 0)
        return;

    // Misma fórmula que el original: random 0-19, si < ai_level → mover
    uint8_t roll = (uint8_t)(_rng.next() % 20);
    if (roll < _ai_level)
        move_forward();
}

void Bonnie::move_forward()
{
    if (_pos == BonniePos::LEFT_DOOR)
        return;

    _pos = static_cast<BonniePos>(static_cast<uint8_t>(_pos) + 1);
    _attack_timer = 0; // reiniciar al llegar a nueva posición
}

void Bonnie::update_attack(GameContext& ctx)
{
    _attack_timer++;

    if (_attack_timer >= attack_threshold())
    {
        // Jumpscare — el game loop lo detectará en check_win_lose()
        ctx.game_over = true;
    }
}

bool Bonnie::at_left_door() const
{
    return _pos == BonniePos::LEFT_DOOR;
}