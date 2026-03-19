#include "bn_core.h"

#include "screens/intro_screen.h"
#include "screens/newspaper_screen.h"
#include "screens/menu/main_menu.h"
#include "screens/menu/custom_menu.h"
#include "screens/end/win_screen.h"
#include "screens/end/gameover_screen.h"
#include "screens/screen_manager.h"

#include "save/save_data.h"
#include "game/game.h"

int main()
{
    // -------------------------
    // INIT GBA
    // -------------------------
    bn::core::init();

    // -------------------------
    // SAVE / CONTEXT
    // -------------------------
    save_init();

    SaveData save;
    MenuContext menu_context{};

    if(save_exists())
    {
        save = load_save();

        menu_context.has_save = true;
        menu_context.max_night = save.max_night;
        menu_context.stars = save.stars;
        menu_context.custom_unlocked = (save.flags & FLAG_CUSTOM_UNLOCK) != 0;
    }
    else
    {
        save = make_new_save();

        menu_context.has_save = false;
        menu_context.max_night = 0;
        menu_context.stars = 0;
        menu_context.custom_unlocked = false;
    }

    // -------------------------
    // SCREEN MANAGER
    // -------------------------
    ScreenManager screen_manager;

    IntroScreen intro_screen;
    NewspaperScreen newspaper_screen;

    MainMenu main_menu(menu_context);
    CustomMenu custom_menu;

    Game game;

    WinScreen win_screen;
    GameOverScreen gameover_screen;

    screen_manager.register_screen(Screen::INTRO, &intro_screen);
    screen_manager.register_screen(Screen::NEWSPAPER, &newspaper_screen);
    screen_manager.register_screen(Screen::MAIN_MENU, &main_menu);
    screen_manager.register_screen(Screen::CUSTOM_MENU, &custom_menu);
    screen_manager.register_screen(Screen::GAME, &game);
    screen_manager.register_screen(Screen::WIN, &win_screen);
    screen_manager.register_screen(Screen::GAMEOVER, &gameover_screen);

    // Comenzar con el intro
    screen_manager.change_to(Screen::INTRO);

    // -------------------------
    // MAIN LOOP
    // -------------------------
    Screen previous = screen_manager.current_screen();

    while(true)
    {
        screen_manager.update();

        Screen current = screen_manager.current_screen();

        if(current != previous)
        {
            Screen from = screen_manager.previous_screen();

            // -------------------------
            // ENTER GAME
            // -------------------------
            if(current == Screen::GAME)
            {
                if(from == Screen::NEWSPAPER)
                {
                    game.start_normal(1);
                }
                else if(from == Screen::MAIN_MENU)
                {
                    switch(main_menu.action())
                    {
                        case MenuAction::CONTINUE:
                            game.start_normal(save.max_night);
                            break;

                        case MenuAction::NIGHT_6:
                            game.start_normal(6);
                            break;

                        default:
                            game.start_normal(1);
                            break;
                    }
                }
                else if(from == Screen::CUSTOM_MENU)
                {
                    game.start_custom(custom_menu.settings());
                }
            }

            previous = current;
        }

        bn::core::update();
    }
}