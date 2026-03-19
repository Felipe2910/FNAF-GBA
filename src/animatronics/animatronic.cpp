#include "animatronic.h"
#include <stdlib.h>

void Animatronic::init(uint8_t level)
{
    _ai_level = level;
    _position = 0;
    _move_timer = 0;
}

void Animatronic::update(GameContext& ctx)
{
    if(ctx.game_over || ctx.win)
        return;

    _move_timer++;

    // Cada cierto tiempo intenta moverse
    if(_move_timer > 180) {  // ~3 segundos a 60fps
        _move_timer = 0;

        // Probabilidad según IA
        if((rand() % 20) < _ai_level) {
            if(_position < 2)
                _position++;
        }
    }

    // Si está en puerta y puerta abierta → jumpscare
    if(_position == 2) {
        if(!ctx.left_door_closed) {
            ctx.game_over = true;
        }
    }
}

bool Animatronic::at_left_door() const
{
    return _position == 2;
}

bool Animatronic::at_right_door() const
{
    return false;
}