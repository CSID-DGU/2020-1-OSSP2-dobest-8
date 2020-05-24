// Copyright [2015] <Chafic Najjar>


#ifndef SRC_PLAYSTATE_H_
#define SRC_PLAYSTATE_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <irrKlang.h>


#include "src/gamestate.h"

class Tetromino;
class Board;


class PlayState : public GameState {
 public:
    static const int NCOLORS = 7;

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


    static PlayState* Instance() { return &m_playstate; }

 protected:
    PlayState() { }


 private:
    static PlayState m_playstate;


    void release_tetromino();
    void draw_block(GameEngine* game, int x, int y, int k, SDL_Rect clips[]);
    void create_button(GameEngine* game,
            int x, int y, int width, int height, int color[]);
    float frame_rate(GameEngine* game, int *last_time, int *this_time);



    // ===================== For Dobest update =====================
    /* TODO:
    */ 
    void initDobestUpdate();

    // mouse area
    SDL_Rect mouse_area_size;
    SDL_Rect mouse_area;
    
    // mouse object
    struct {
      float motion_scale = 1.0f; // 마우스가 움직이는 속도를 조절 높을수록 빨리움직임
      int size = 7;              // 마우스의 그려지는 크기
      struct {
         float x;
         float y;
      } pos;
      int state = 0;             // 0: none, 1: god, 2: destroy block, 3: generate block
      float state_remain_time = 0.0f;       // state remain time
    } mouse;
    const float state_remain_times[4] = {  // 아이템 지속시간 (초)
       0.0f,      // dummy
       5.0f,      // god            무적
       8.0f,      // destroy block  블럭 파괴
       7.0f       // generate block 블럭 생성
    };
    const SDL_Color mouse_color_mid = {0, 110, 0, 255};  // 아이템 지속시간이 끝나기 직전의 색
    const SDL_Color mouse_colors[4] = {                  // 아이템 지속시간 중의 색 위의 변수랑 색이 블렌드 돼서 지속시간이 얼마나 남았는지 알려줌
       {0, 210, 0, 255},     // none
       {255, 210, 0, 255},   // god             무적
       {0, 210, 255, 255},   // destroy block   블럭 파괴
       {255, 210, 255, 255}, // generate block  블럭 생성
    };

    // item
    struct {
       int size = 7;       // 아이템의 그려지는 크기
       int spawned = false;
       struct {
          int x;
          int y;
       } pos;
       // 0: not initialized, 1: god, 2: destroy block, 3: generate block
       // generate block's generated block (color : 10)
       int type = 0;            
       float remain_time;
    } mouse_item;
    const float item_remain_time = 3.0f;     // 아이템이 화면에 존재하는 시간
    const SDL_Color item_colors[4] = {       // 아이템 색깔
       {0, 0, 0, 255}, // dummy
       {255, 0, 0, 255},                     // 무적
       {0, 0, 255, 255},                     // 블럭 파괴
       {255, 255, 255, 255}                  // 블럭 생성
    };
    float item_spawn_prob = 0.8f;            // 블럭생성시 아이템이 생성될 확률
    TTF_Font* font_item_description;
    SDL_Texture* item_description[3];
    TTF_Font *font_tetro_description;
    SDL_Texture* tetro_description[2];

    // hold tetromino
    Tetromino* hold_tetro;
    SDL_Point hold_tetro_pos;

    SDL_Point toBoardCoordinate(float x, float y);
    int countMouseAreaBlocks();
    SDL_Color lerpColor(SDL_Color a, SDL_Color b, float t); // t=0: a, t=1: b
    void holdSwap();
    // ============================================================

    // Game objects.
    Board* board;
    Tetromino* tetro;
    Tetromino* next_tetro;

    // Music.
    irrklang::ISoundEngine* music_engine;

    // Texture.
    SDL_Texture* block_texture;

    // Fonts.
    SDL_Color       white;
    TTF_Font*       font_pause;
    TTF_Font*       font_tetris;
    TTF_Font*       font_score_text;
    TTF_Font*       font_score;
    TTF_Font*       font_new_game;
    TTF_Font*       font_quit;
    TTF_Font*       font_game_over;

    SDL_Texture*    font_image_pause;
    SDL_Texture*    font_image_tetris;
    SDL_Texture*    font_image_score_text;
    SDL_Texture*    font_image_score;
    SDL_Texture*    font_image_new_game;
    SDL_Texture*    font_image_quit;
    SDL_Texture*    font_image_tetrino_win;
    SDL_Texture*    font_image_mouse_win;

    // Frame rate.
    float acceleration;  // Multiplied by score to provide falling speed.
    int this_time;  // Time since SDL_Init() of the current game loop.
    int last_time;  // Time since SDL_Init() of the previous game loop.
    float time_till_drop;  // Tetromino falls down 1 block every
                           // time_till_drop seconds.
    float time_counter;  // Counts number of game loops to allow
                         // tetromino to fall down.

    // Buttons status.
    bool newgamedown;  // True when player presses "New Game" button.
    bool newgameup;  // True when player releases "New Game" button.
    bool quitdown;  // True when player presses "Quit" button.
    bool quitup;  // True when player releases "Quit" button.

    // Buttons coordinates.
    int newgamex1;
    int newgamex2;
    int newgamey1;
    int newgamey2;

    bool paused;
    bool game_over;  // True when game end
    int winner;      // 0: game not ended, 1: tetrino, 2: mouse 
    bool exit;  // True when player exits game.
};

#endif  // SRC_PLAYSTATE_H_
