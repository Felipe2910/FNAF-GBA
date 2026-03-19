#pragma once

#include <stdint.h>
#include "bn_string.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_music_item.h"
#include "../screen_manager.h"
#include "../../core/rng.h"
#include "../../effects/static_noise.h"

struct MenuContext
{
    bool has_save;
    uint8_t max_night;
    uint8_t stars;
    bool custom_unlocked;
};

enum class MenuAction
{
    NONE,
    CONTINUE,
    NEW_GAME,
    NIGHT_6,
    CUSTOM_NIGHT
};

class MainMenu : public ScreenBase
{
public:
    MainMenu(const MenuContext &context);

    // Implementación de ScreenBase
    void enter() override;
    void exit() override;
    void update() override;
    Screen next_screen() const override;

    MenuAction action() const;

private:
    void handle_input();
    void build_options();
    void redraw();
    void destroy_sprites();
    void draw_title();
    void draw_options();
    void draw_stars();
    bn::string<32> option_text(MenuAction action) const;

    void update_background();
    void change_background(int bg_index);
    void do_quick_flicker();

private:
    const MenuContext &_ctx;
    bool _needs_redraw = true;
    int _selected = 0;
    int _option_count = 0;
    MenuAction _action = MenuAction::NONE;
    Screen _next_screen = Screen::NONE;

    MenuAction _options[4];

    bn::optional<bn::regular_bg_ptr> _bg;
    bn::vector<bn::sprite_ptr, 96> _menu_sprites;
    bn::vector<bn::sprite_ptr, 8> _cursor_sprites;
    bn::vector<bn::sprite_ptr, 8> _star_sprites;

    // Sistema de backgrounds
    RNG _rng;
    int _current_bg = 0;
    int _bg_timer = 0;
    int _flicker_timer = 0;
    bool _is_flickering = false;

    int _face_timer = 0;
    int _face_alpha_timer = 0;
    bn::fixed _dark_alpha = 0.5;
    int _scanline_y = 0;

    static constexpr int BG_COUNT = 4;
    static constexpr int FLICKER_DURATION = 10; // frames

    StaticNoise _static_effect;
};