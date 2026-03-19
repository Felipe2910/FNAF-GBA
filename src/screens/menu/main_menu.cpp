#include "main_menu.h"

#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "bn_music.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_regular_bg_items_menu1.h"
#include "bn_regular_bg_items_menu2.h"
#include "bn_regular_bg_items_menu3.h"
#include "bn_regular_bg_items_menu4.h"
#include "bn_sprite_items_star_16x16.h"
#include "bn_music_items.h"

MainMenu::MainMenu(const MenuContext &context) : _ctx(context),
                                                 _needs_redraw(true),
                                                 _selected(0),
                                                 _action(MenuAction::NONE)
{
    build_options();
}

void MainMenu::enter()
{
    _bg = bn::nullopt;
    _action = MenuAction::NONE;
    _next_screen = Screen::NONE;
    _needs_redraw = true;

    _current_bg = 0;
    change_background(_current_bg);

    _face_timer = 0;
    _face_alpha_timer = 0;
    _scanline_y = 0;
    _dark_alpha = 0.6;

    _is_flickering = false;
    _flicker_timer = 0;

    _static_effect.show(true, 0.15);

    bn::music_items::darkness.play(true);

    redraw();
}

void MainMenu::exit()
{
    destroy_sprites();
    _bg = bn::nullopt;
    _static_effect.hide();
    bn::music::stop();
}

void MainMenu::update()
{
    handle_input();
    update_background();

    if (_needs_redraw)
    {
        redraw();
        _needs_redraw = false;
    }
}

void MainMenu::update_background()
{
    _static_effect.update();

    // === Oscurecimiento dinámico tipo N64 ===
    _face_alpha_timer++;
    if (_face_alpha_timer > 18)
    {
        _face_alpha_timer = 0;
        _dark_alpha = bn::fixed(0.4 + (_rng.next() % 30) / 100.0);
        if (_bg)
            _bg->set_blending_enabled(true);
    }

    // === Scanline descendente ===
    _scanline_y += 2;
    if (_scanline_y > 160)
        _scanline_y = -10;

    // Aquí podrías dibujar una rectángulo con blending si quieres
    // usando un sprite blanco fino reciclado como scanline

    // === Flicker tipo interferencia ===
    if (_is_flickering)
    {
        _flicker_timer++;

        if (_flicker_timer == 1)
        {
            int glitch = _rng.next() % BG_COUNT;
            change_background(glitch);
        }

        if (_flicker_timer == 3)
        {
            change_background(_current_bg); // Volver al fondo base
        }

        if (_flicker_timer >= 4)
        {
            _is_flickering = false;
            _flicker_timer = 0;
        }

        return;
    }

    // Activar flicker aleatorio ocasional
    if ((_rng.next() % 600) == 0)
    {
        _is_flickering = true;
        _flicker_timer = 0;
    }
}

void MainMenu::do_quick_flicker()
{
    // Iniciar el efecto de parpadeo
    _is_flickering = true;
    _flicker_timer = 0;
}

void MainMenu::change_background(int bg_index)
{
    _bg.reset();

    switch (bg_index)
    {
    case 0:
        _bg = bn::regular_bg_items::menu1.create_bg(0, 0);
        break;
    case 1:
        _bg = bn::regular_bg_items::menu2.create_bg(0, 0);
        break;
    case 2:
        _bg = bn::regular_bg_items::menu3.create_bg(0, 0);
        break;
    case 3:
        _bg = bn::regular_bg_items::menu4.create_bg(0, 0);
        break;
    default:
        _bg = bn::regular_bg_items::menu1.create_bg(0, 0);
        break;
    }
}

Screen MainMenu::next_screen() const
{
    return _next_screen;
}

void MainMenu::handle_input()
{
    bool moved = false;

    if (bn::keypad::up_pressed())
    {
        _selected = (_selected - 1 + _option_count) % _option_count;
        moved = true;
    }

    if (bn::keypad::down_pressed())
    {
        _selected = (_selected + 1) % _option_count;
        moved = true;
    }

    if (moved)
        _needs_redraw = true;

    if (bn::keypad::a_pressed())
    {
        _action = _options[_selected];

        // Mapear acción a pantalla
        switch (_action)
        {
        case MenuAction::CUSTOM_NIGHT:
            _next_screen = Screen::CUSTOM_MENU;
            break;
        case MenuAction::NEW_GAME:
            _next_screen = Screen::NEWSPAPER;
            break;
        case MenuAction::CONTINUE:
        case MenuAction::NIGHT_6:
            _next_screen = Screen::GAME;
            break;
        default:
            break;
        }
    }
}

void MainMenu::build_options()
{
    _option_count = 0;

    if (_ctx.has_save)
        _options[_option_count++] = MenuAction::CONTINUE;

    _options[_option_count++] = MenuAction::NEW_GAME;

    if (_ctx.max_night >= 5)
        _options[_option_count++] = MenuAction::NIGHT_6;

    if (_ctx.custom_unlocked)
        _options[_option_count++] = MenuAction::CUSTOM_NIGHT;
}

bn::string<32> MainMenu::option_text(MenuAction action) const
{
    bn::string<32> result;

    switch (action)
    {
    case MenuAction::CONTINUE:
        result = "Continue";

        if (_ctx.has_save)
        {
            result += " ";
            result += bn::to_string<4>(_ctx.max_night);
        }
        break;

    case MenuAction::NEW_GAME:
        result = "New Game";
        break;

    case MenuAction::NIGHT_6:
        result = "6th Night";
        break;

    case MenuAction::CUSTOM_NIGHT:
        result = "Custom Night";
        break;

    default:
        break;
    }

    return result;
}

void MainMenu::redraw()
{
    destroy_sprites();

    draw_title();
    draw_options();
    draw_stars();
}

void MainMenu::draw_title()
{
    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);

    gen.generate(-110, -70, "Five", _menu_sprites);
    gen.generate(-110, -60, "Nights", _menu_sprites);
    gen.generate(-110, -50, "at", _menu_sprites);
    gen.generate(-110, -40, "Freddy's", _menu_sprites);
}

void MainMenu::draw_options()
{
    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);

    for (int i = 0; i < _option_count; ++i)
    {
        int y = 20 + i * 16;

        bn::string<32> text = option_text(_options[i]);
        gen.generate(-90, y, text, _menu_sprites);

        if (i == _selected)
            gen.generate(-110, y, ">>", _cursor_sprites);
    }
}

void MainMenu::draw_stars()
{
    int stars_to_draw = _ctx.stars;
    if (stars_to_draw > 3)
        stars_to_draw = 3;

    for (int i = 0; i < stars_to_draw; ++i)
    {
        _star_sprites.push_back(
            bn::sprite_items::star_16x16.create_sprite(
                -100 + i * 16,
                -20));
    }
}

void MainMenu::destroy_sprites()
{
    _menu_sprites.clear();
    _cursor_sprites.clear();
    _star_sprites.clear();
}

MenuAction MainMenu::action() const
{
    return _action;
}