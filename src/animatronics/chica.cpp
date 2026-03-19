#include "chica.h"

void Chica::init(uint8_t ai_level)
{
    _ai_level = (ai_level > 20) ? 20 : ai_level;
    reset();
}

void Chica::reset()
{
    _pos          = ChicaPos::SHOW_STAGE;
    _move_timer   = 0;
    _attack_timer = 0;
}

void Chica::update(GameContext& ctx)
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

    // --- Lógica de puerta derecha ---
    if (_pos == ChicaPos::RIGHT_DOOR)
    {
        if (ctx.right_door_closed)
        {
            _attack_timer = 0;
        }
        else
        {
            update_attack(ctx);
        }
    }

    // Exponer posición al contexto
    ctx.chica.camera       = CHICA_CAM_ID[static_cast<uint8_t>(_pos)];
    ctx.chica.at_right_door = (_pos == ChicaPos::RIGHT_DOOR);
}

void Chica::try_move()
{
    if (_ai_level == 0)
        return;

    uint8_t roll = (uint8_t)(_rng.next() % 20);
    if (roll < _ai_level)
        move_forward();
}

void Chica::move_forward()
{
    if (_pos == ChicaPos::RIGHT_DOOR)
        return;

    _pos = static_cast<ChicaPos>(static_cast<uint8_t>(_pos) + 1);
    _attack_timer = 0;
}

void Chica::update_attack(GameContext& ctx)
{
    _attack_timer++;
    if (_attack_timer >= attack_threshold())
        ctx.game_over = true;
}

bool Chica::at_right_door() const
{
    return _pos == ChicaPos::RIGHT_DOOR;
}