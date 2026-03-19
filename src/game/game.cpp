#include "game.h"

#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_usage_bar.h"
#include "common_variable_8x16_sprite_font.h"

Game::Game() {}

void Game::enter()
{
    _next_screen = Screen::NONE;

    _office.enter();

    if (_pending_custom)
    {
        _ctx.night_type = NightType::CUSTOM;
        _ctx.night = 7; // o el número que quieras mostrar
    }
    else
    {
        _ctx.night_type = (_pending_night == 6)
                              ? NightType::NIGHT_6
                              : NightType::NORMAL;

        _ctx.night = _pending_night;
    }

    _ctx.hour = 0;
    _ctx.power = 100;
    _ctx.left_door_closed = false;
    _ctx.right_door_closed = false;
    _ctx.left_light_on = false;
    _ctx.right_light_on = false;
    _ctx.cameras_open = false;
    _ctx.current_camera = 0;
    _ctx.game_over = false;
    _ctx.win = false;
    _ctx.current_view = GameView::OFFICE_CENTER;

    // Inicializar estado visible de animatrónicos
    _ctx.bonnie = {1, false, false}; // empieza en Cam 1A (Show Stage)
    _ctx.chica = {1, false, false};
    _ctx.foxy = {0, false, false}; // Pirate Cove no tiene ID en nuestro enum aún
    _ctx.freddy = {1, false, false};

    _clock.reset();
    _power = Power(_pending_custom ? 6 : _pending_night);
    _power.reset();

    _bonnie.init(_settings.bonnie_level);
    _chica.init(_settings.chica_level);
    _foxy.init(_settings.foxy_level);
    _freddy.init(_settings.freddy_level);
}

void Game::update()
{
    handle_input();
    update_clock();
    update_power();
    update_animatronics();
    _office.update(_ctx);
    check_win_lose();

    _sprites.clear(); // MUY IMPORTANTE para evitar overflow

    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);

    bn::string<32> text;
    text = "Power: ";
    text += bn::to_string<3>(_ctx.power);
    text += "%";

    gen.generate(-110, 50, text, _sprites);

    for (int i = 0; i < _ctx.power_usage; ++i)
    {
        auto bar = bn::sprite_items::usage_bar.create_sprite(
            -110 + (i * 6),
            70);

        _sprites.push_back(bar);
    }

    uint8_t display_hour = (_ctx.hour == 0) ? 12 : _ctx.hour;
    bn::string<32> hour_text;
    hour_text = bn::to_string<2>(display_hour);
    hour_text += " AM";

    gen.generate(80, -70, hour_text, _sprites);

    bn::string<32> night_text;
    night_text = "Night ";
    night_text += bn::to_string<1>(_ctx.night + 1); // +1 para mostrar 1-6 en vez de 0-5, o 7 para custom

    gen.generate(70, -60, night_text, _sprites);
}

void Game::exit()
{
    _office.exit();
}

void Game::handle_input()
{
    if (_ctx.current_view != GameView::CAMERAS)
    {
        if (bn::keypad::left_pressed())
        {
            switch (_ctx.current_view)
            {
            case GameView::OFFICE_CENTER:
                _ctx.current_view = GameView::OFFICE_LEFT;
                break;

            case GameView::OFFICE_RIGHT:
                _ctx.current_view = GameView::OFFICE_CENTER;
                break;

            default:
                break;
            }
        }

        if (bn::keypad::right_pressed())
        {
            switch (_ctx.current_view)
            {
            case GameView::OFFICE_CENTER:
                _ctx.current_view = GameView::OFFICE_RIGHT;
                break;

            case GameView::OFFICE_LEFT:
                _ctx.current_view = GameView::OFFICE_CENTER;
                break;

            default:
                break;
            }
        }
    }

    if (bn::keypad::l_pressed())
        _ctx.left_door_closed = !_ctx.left_door_closed;

    if (bn::keypad::r_pressed())
        _ctx.right_door_closed = !_ctx.right_door_closed;

    if (bn::keypad::a_pressed())
    {
        _ctx.current_view = (_ctx.current_view == GameView::CAMERAS)
                                ? GameView::OFFICE_CENTER
                                : GameView::CAMERAS;
    }

    if (bn::keypad::b_held())
    {
        _ctx.left_light_on = (_ctx.current_view == GameView::OFFICE_LEFT);
        _ctx.right_light_on = (_ctx.current_view == GameView::OFFICE_RIGHT);
    }
    else
    {
        _ctx.left_light_on = false;
        _ctx.right_light_on = false;
    }
}

void Game::update_clock()
{
    _clock.update();
    _ctx.hour = _clock.hour();
}

void Game::update_power()
{
    // usage = número de items activos (0 = solo existir).
    // La tabla BASE_DRAIN_FRAMES ya cubre el consumo base de estar encendido.
    // Cada item extra sube el multiplicador.
    uint8_t usage = 0;

    if (_ctx.left_door_closed)
        usage++;
    if (_ctx.right_door_closed)
        usage++;
    if (_ctx.left_light_on)
        usage++;
    if (_ctx.right_light_on)
        usage++;
    if (_ctx.current_view == GameView::CAMERAS)
        usage++;

    _power.update(usage);
    _ctx.power = _power.value();
    _ctx.power_usage = usage;

    if (_power.is_empty() && !_ctx.power_out)
    {
        _ctx.left_door_closed = false;
        _ctx.right_door_closed = false;
        _ctx.left_light_on = false;
        _ctx.right_light_on = false;
        _ctx.power_out = true;

        // Freddy entra en modo power-out: sortea su delay y empieza la cuenta
        _freddy.trigger_power_out();
    }
}

void Game::update_animatronics()
{
    _bonnie.update(_ctx);
    _chica.update(_ctx);
    _foxy.update(_ctx);
    _freddy.update(_ctx);
}

void Game::check_win_lose()
{
    if (_clock.is_finished())
    {
        _ctx.win = true;
        _next_screen = Screen::WIN;
    }
    else if (_ctx.game_over)
    {
        _next_screen = Screen::GAMEOVER;
    }
}

Screen Game::next_screen() const
{
    return _next_screen;
}

void Game::start_normal(uint8_t night)
{
    _pending_night = night;
    _pending_custom = false;

    // Cargar dificultad base según LUT
    // Fuente: JS CONFIG.AI_LEVELS (valores verificados contra el original)
    static const uint8_t ai_lut[6][4] = {
        //  Freddy  Bonnie  Chica   Foxy
        {0, 0, 0, 0},    // Noche 1
        {0, 3, 1, 1},    // Noche 2
        {1, 0, 5, 2},    // Noche 3
        {1, 2, 4, 6},    // Noche 4
        {3, 5, 7, 5},    // Noche 5
        {4, 10, 12, 16}, // Noche 6
    };

    if (night >= 1 && night <= 6)
    {
        const uint8_t *row = ai_lut[night - 1];

        _settings.freddy_level = row[0];
        _settings.bonnie_level = row[1];
        _settings.chica_level = row[2];
        _settings.foxy_level = row[3];
    }
}

void Game::start_custom(const GameSettings &settings)
{
    _pending_custom = true;
    _settings = settings;
}

const GameContext &Game::context() const
{
    return _ctx;
}