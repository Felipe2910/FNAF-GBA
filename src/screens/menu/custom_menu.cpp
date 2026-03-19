#include "custom_menu.h"

#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "bn_music.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_music_items.h"

static void u8_to_str(uint8_t value, char* out)
{
    if(value >= 10)
    {
        out[0] = '0' + (value / 10);
        out[1] = '0' + (value % 10);
        out[2] = '\0';
    }
    else
    {
        out[0] = '0' + value;
        out[1] = '\0';
    }
}

CustomMenu::CustomMenu() {}

void CustomMenu::enter()
{
    _action = CustomAction::NONE;
    _next_screen = Screen::NONE;
    _selected = 0;
    _needs_redraw = true;
    draw();

    bn::music_items::fnaf_intro.play(0.3);
}

void CustomMenu::exit()
{
    _sprites.clear();
    bn::music::stop();
}

void CustomMenu::update()
{
    bool changed = handle_input();

    if(changed || _needs_redraw)
    {
        draw();
        _needs_redraw = false;
    }
}

Screen CustomMenu::next_screen() const
{
    return _next_screen;
}

bool CustomMenu::handle_input()
{
    bool changed = false;

    if(bn::keypad::up_pressed())
    {
        _selected = (_selected + 5) % 6;
        changed = true;
    }

    if(bn::keypad::down_pressed())
    {
        _selected = (_selected + 1) % 6;
        changed = true;
    }

    if(bn::keypad::left_pressed())
    {
        if(_selected == 0 && _settings.freddy_level > 0) { _settings.freddy_level--; changed = true; }
        if(_selected == 1 && _settings.bonnie_level > 0) { _settings.bonnie_level--; changed = true; }
        if(_selected == 2 && _settings.chica_level  > 0) { _settings.chica_level--;  changed = true; }
        if(_selected == 3 && _settings.foxy_level   > 0) { _settings.foxy_level--;   changed = true; }
    }

    if(bn::keypad::right_pressed())
    {
        if(_selected == 0 && _settings.freddy_level < 20) { _settings.freddy_level++; changed = true; }
        if(_selected == 1 && _settings.bonnie_level < 20) { _settings.bonnie_level++; changed = true; }
        if(_selected == 2 && _settings.chica_level  < 20) { _settings.chica_level++;  changed = true; }
        if(_selected == 3 && _settings.foxy_level   < 20) { _settings.foxy_level++;   changed = true; }
    }

    if(bn::keypad::a_pressed() && _selected == 4)
    {
        _action = CustomAction::START;
        _next_screen = Screen::GAME;
    }

    if(bn::keypad::b_pressed() || (_selected == 5 && bn::keypad::a_pressed()))
    {
        _action = CustomAction::BACK;
        _next_screen = Screen::MAIN_MENU;
    }

    return changed;
}

void CustomMenu::draw()
{
    _sprites.clear();

    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);

    const char* labels[] = {"Freddy", "Bonnie", "Chica", "Foxy", "Start", "Back"};
    int positions[] = {-60, -40, -20, 0, 40, 56};

    for(int i = 0; i < 6; i++)
    {
        gen.generate(-90, positions[i], labels[i], _sprites);
        
        if(i == _selected)
            gen.generate(-110, positions[i], ">", _sprites);
    }

    draw_values();
}

void CustomMenu::draw_values()
{
    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);

    char buffer[3];

    u8_to_str(_settings.freddy_level, buffer);
    gen.generate(40,-60,buffer,_sprites);

    u8_to_str(_settings.bonnie_level, buffer);
    gen.generate(40,-40,buffer,_sprites);

    u8_to_str(_settings.chica_level, buffer);
    gen.generate(40,-20,buffer,_sprites);

    u8_to_str(_settings.foxy_level, buffer);
    gen.generate(40,  0,buffer,_sprites);
}

CustomAction CustomMenu::action() const
{
    return _action;
}

const GameSettings& CustomMenu::settings() const
{
    return _settings;
}