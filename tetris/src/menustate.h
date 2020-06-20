// Copyright [2015] <Chafic Najjar>

#ifndef SRC_MENUSTATE_H_
#define SRC_MENUSTATE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "src/gamestate.h"

class MenuState : public GameState {
 public:
    void init(GameEngine* game);
    void clean_up(GameEngine* game);

    void pause();
    void resume();
    void reset();

    void input(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    // Navigate through menu items.
    void select_up();
    void select_down();

    static MenuState* Instance() { return &m_menustate; }

 protected:
    MenuState() { }

 private:
    static MenuState m_menustate;

    bool play;
    bool play2; //기본 테트리스모드
    bool help; //도움말
    bool exit;

    // Font textures.
    SDL_Color       white;
    TTF_Font*       font_title;
    TTF_Font*       font_play;
    TTF_Font*       font_play2; //기본 테트리스모드
    TTF_Font*       font_help; //도움말
    TTF_Font*       font_quit;
    SDL_Texture*    font_image_title;
    SDL_Texture*    font_image_play;
    SDL_Texture*    font_image_play2; //기본 테트리스모드
    SDL_Texture*    font_image_help; //도움말
    SDL_Texture*    font_image_quit;

    // Text position.
    int title_width, title_height;
    int play_width, play_height;
    int play2_width, play2_height; //기본 테트리스모드
    int help_width, help_height; //도움말
    int quit_width, quit_height;

    // Currently selected menu item.
    int currently_selected;

    // Number of items.
    int items;
};

#endif  // SRC_MENUSTATE_H_
