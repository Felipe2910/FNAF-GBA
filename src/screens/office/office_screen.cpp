#include "office_screen.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_regular_bg_items_office.h"

void OfficeScreen::enter()
{
    _bg_pos = 0;
    _bg_target = 0;
    _bg = bn::regular_bg_items::office.create_bg(0, 0);
    _bg->set_x(_bg_pos);
    bn::music_items::ambience2.play(true);
}

void OfficeScreen::update(const GameContext &ctx)
{
    if (!_bg)
        return;

    // Scroll horizontal según vista
    switch (ctx.current_view)
    {
    case GameView::OFFICE_LEFT:
        _bg_target = 40;
        break;

    case GameView::OFFICE_CENTER:
        _bg_target = 0;
        break;

    case GameView::OFFICE_RIGHT:
        _bg_target = -40;
        break;

    default:
        break;
    }

    int diff = _bg_target - _bg_pos;

    if (diff != 0)
    {
        int step = (diff > 0) ? _bg_speed : -_bg_speed;

        if (std::abs(diff) <= std::abs(step)) _bg_pos = _bg_target;
        else _bg_pos += step;
        
        _bg->set_x(_bg_pos);
    }
    
}

void OfficeScreen::exit()
{
    _bg.reset();
}