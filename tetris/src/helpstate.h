//Copyright [2020] <CSID-DGU/dobest>

#ifndef SRC_HELPSTATE_H_
#define SRC_HELPSTATE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "src/gamestate.h"
class Tetromino;
class Board;

class HelpState : public GameState {
 public:
    // Space between board border and window border.
    static const int GAME_OFFSET = 20;

    void init(GameEngine* game);
    void clean_up(GameEngine* game);

    void pause();
    void resume();
    void reset();

    void input(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static HelpState* Instance() { return &m_helpstate; }

    void render_help(GameEngine* game);

 protected:
    HelpState() { }

 private:
    static HelpState m_helpstate;
    // Game objects.
    Board* board;
    void create_button(GameEngine* game,
            int x, int y, int width, int height, int color[]);
    bool exit;
    
    // Fonts.
    SDL_Color       white;
    TTF_Font*       font_menu;
    TTF_Font*       font_quit;

    SDL_Texture* help;
    SDL_Texture*    font_image_menu;
    SDL_Texture*    font_image_quit;

    bool quitdown;  // True when player presses "Quit" button.
    bool quitup;  // True when player releases "Quit" button.
    // True when player presses "Menu" button.
    bool menudown;  
    // True when player releases "Menu" button.
    bool menuup;  

    // Buttons coordinates.
    int newgamex1;
    int newgamex2;
    int newgamey1;
    int newgamey2;
};

#endif  // SRC_HELPSTATE_H_