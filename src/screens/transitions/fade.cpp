#include "fade.h"

#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"
#include "bn_colors.h"

void Fade::start_in()
{
    _fade_in = true;
    _active = true;
    _alpha = 1;
}

void Fade::start_out()
{
    _fade_in = false;
    _active = true;
    _alpha = 0;
}

void Fade::update()
{
    if(!_active)
        return;

    if(_fade_in)
    {
        _alpha -= _speed;

        if(_alpha <= 0)
        {
            _alpha = 0;
            _active = false;
        }
    }
    else
    {
        _alpha += _speed;

        if(_alpha >= 1)
        {
            _alpha = 1;
            _active = false;
        }
    }

    bn::bg_palettes::set_fade(bn::colors::black, _alpha);
    bn::sprite_palettes::set_fade(bn::colors::black, _alpha);
}

bool Fade::finished() const
{
    return !_active;
}