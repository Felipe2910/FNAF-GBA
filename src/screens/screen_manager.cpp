#include "screen_manager.h"

ScreenManager::ScreenManager() {}

void ScreenManager::register_screen(Screen id, ScreenBase* screen)
{
    int index = static_cast<int>(id);
    if (index >= 0 && index < MAX_SCREENS)
    {
        _screens[index] = screen;
    }
}

void ScreenManager::change_to(Screen id)
{
    if (id == _current)
        return;
    
    int index = static_cast<int>(id);
    if (index < 0 || index >= MAX_SCREENS || _screens[index] == nullptr)
        return;
    
    // Salir de la pantalla actual
    if (_active_screen != nullptr)
    {
        _active_screen->exit();
    }
    
    // Cambiar a la nueva pantalla
    _previous = _current;
    _current = id;
    _active_screen = _screens[index];
    
    // Entrar a la nueva pantalla
    _active_screen->enter();
}

void ScreenManager::update()
{
    _active_screen->update();

    Screen next = _active_screen->next_screen();
    if (next != Screen::NONE)
    {
        change_to(next);  // ← exit() + enter() en el mismo frame
    }
}