#pragma once

#include <stdint.h>
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"

enum class Screen
{
    NONE,
    INTRO,
    TRAILER,
    MAIN_MENU,
    CUSTOM_MENU,
    NEWSPAPER,
    GAME,
    WIN,
    GAMEOVER
};

class ScreenBase
{
public:
    virtual ~ScreenBase() = default;
    
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update() = 0;
    
    // Retorna la siguiente pantalla solicitada, o NONE si no hay cambio
    virtual Screen next_screen() const { return Screen::NONE; }
};

class ScreenManager
{
public:
    ScreenManager();
    
    void register_screen(Screen id, ScreenBase* screen);
    void change_to(Screen id);
    void update();
    
    Screen previous_screen() const { return _previous; }
    Screen current_screen() const { return _current; }

private:
    static constexpr int MAX_SCREENS = 8;
    
    Screen _current = Screen::NONE;
    Screen _previous = Screen::NONE;
    
    ScreenBase* _screens[MAX_SCREENS] = {nullptr};
    ScreenBase* _active_screen = nullptr;
};