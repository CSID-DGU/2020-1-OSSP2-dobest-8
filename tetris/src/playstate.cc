// Copyright [2015] <Chafic Najjar>

#include "src/playstate.h"

#include <random>

#include "src/game_engine.h"
#include "src/tetromino.h"
#include "src/board.h"
#include "src/utilities.h"


#include <stdio.h>

// This will prevent linker errors in case the same names are used
// in other files.
namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

PlayState PlayState::m_playstate;


void PlayState::initDobestUpdate() {
    // mouse area setting
    mouse_area_size.w = board->COLS - 2;    // 마우스 영역의 너비
    mouse_area_size.h = board->ROWS/2+5;    // 마우스 영역의 높이


    // 마우스 영역이 시작되는 위치 (왼쪽 위가 기준)
    mouse_area_size.x = (board->COLS - mouse_area_size.w) / 2;  
    mouse_area_size.y = 7;

    //  ==========================================

    mouse_area.w = board->BLOCK_WIDTH * mouse_area_size.w;
    mouse_area.h = board->BLOCK_HEIGHT * mouse_area_size.h;
    mouse_area.x = GAME_OFFSET + mouse_area_size.x * board->BLOCK_WIDTH;
    mouse_area.y = GAME_OFFSET + mouse_area_size.y * board->BLOCK_HEIGHT;

    // mouse setting
    mouse.pos.x = GAME_OFFSET + board->WIDTH/2;
    mouse.pos.y = mouse_area.y + mouse_area.h/2;
    mouse.state = 0;

    // item setting
    mouse_item.spawned = false;
    mouse_item.pos = {0, 0};

    // reset hold tetro
    hold_tetro = nullptr;
    hold_tetro_pos.x = board->COLS+5;
    hold_tetro_pos.y = static_cast<int>(0.5*board->ROWS);
}

void PlayState::init(GameEngine* game) {
    // Game objects.
    board        = new Board();
    tetro        = new Tetromino(rand()%7);       // Current tetromino.
    next_tetro   = new Tetromino(rand()%7);       // Next tetromino.

    initDobestUpdate();

    // Music.
    music_engine = irrklang::createIrrKlangDevice();
    music_engine->play2D("resources/sounds/Dubmood-Tetris.ogg", true);

    // Texture.
    block_texture = load_texture("resources/sprites/block.bmp", game->renderer);

    // Fonts.
    TTF_Init();
    white = { 255, 255, 255 };
    font_pause = TTF_OpenFont("resources/fonts/bitwise.ttf", 16);
    font_tetris = TTF_OpenFont("resources/fonts/bitwise.ttf", 16);
    font_score_text = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);
    font_score = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);
    font_new_game = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);
    font_quit = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);
    font_game_over = TTF_OpenFont("resources/fonts/bitwise.ttf", 64);

    font_image_pause = render_text("Pause",
            white, font_pause, game->renderer);
    font_image_tetris = render_text("Tetris Unleashed!",
            white, font_tetris, game->renderer);
    font_image_score_text = render_text("Score: ",
            white, font_score_text, game->renderer);
    font_image_score = render_text(std::to_string(board->get_score()),
        white, font_score, game->renderer);
    font_image_new_game = render_text("New game",
            white, font_new_game, game->renderer);
    font_image_quit = render_text("Quit",
            white, font_quit, game->renderer);
    font_image_tetrino_win = render_text("Tetrino Win!",
            white, font_game_over, game->renderer);
    font_image_mouse_win = render_text("Mouse Win!",
            white, font_game_over, game->renderer);
            
    // mouse item description
    font_item_description = TTF_OpenFont("resources/fonts/bitwise.ttf", 13);
    // 아이템 설명 텍스트
    item_description[0] = render_text("Red: Invincible", white, font_item_description, game->renderer);
    item_description[1] = render_text("Blue: destroy block", white, font_item_description, game->renderer);
    item_description[2] = render_text("white: generate block", white, font_item_description, game->renderer);

    // tetromino description
    font_tetro_description = TTF_OpenFont("resources/fonts/bitwise.ttf", 13);
    tetro_description[0] = render_text("next", white, font_tetro_description, game->renderer);
    tetro_description[1] = render_text("hold", white, font_tetro_description, game->renderer);

    // Frame rate.
    acceleration    = 0.005f;
    this_time       = 0;
    last_time       = 0;
    time_till_drop  = 0.3f;
    time_counter    = 0.0f;

    // Buttons status.
    newgamedown     = false;
    newgameup       = false;
    quitdown        = false;
    quitup          = false;

    // Buttons coordinates.
    newgamex1       = GAME_OFFSET+board->WIDTH+board->BLOCK_WIDTH;
    newgamex2       = GAME_OFFSET+board->WIDTH+8*board->BLOCK_WIDTH;
    newgamey1       = board->HEIGHT-4*board->BLOCK_HEIGHT;
    newgamey2       = board->HEIGHT-6*board->BLOCK_HEIGHT;

    paused          = false;
    game_over       = false;
    exit            = false;

    // At the start of the game:
    // x position of (0, 0) block of tetro is int(15/2) = 7
    // which is the exact horizontal middle of board.
    // y position of (0, 0) block of tetro is 0 which is the top of the board.
    tetro->set_position(static_cast<int>(board->COLS/2), 0);

    // Position next_tetro at the upper right of the window,
    // outside of the board.
    next_tetro->set_position(board->COLS+5, static_cast<int>(0.3*board->ROWS));
}

void PlayState::clean_up(GameEngine* game) {
    // Delete music engine.
    music_engine->drop();

    TTF_CloseFont(font_pause);
    TTF_CloseFont(font_tetris);
    TTF_CloseFont(font_score_text);
    TTF_CloseFont(font_score);
    TTF_CloseFont(font_new_game);
    TTF_CloseFont(font_quit);
    TTF_CloseFont(font_game_over);
    TTF_CloseFont(font_item_description);
    TTF_CloseFont(font_tetro_description);

    SDL_DestroyTexture(font_image_pause);
    SDL_DestroyTexture(font_image_tetris);
    SDL_DestroyTexture(font_image_score_text);
    SDL_DestroyTexture(font_image_score);
    SDL_DestroyTexture(font_image_new_game);
    SDL_DestroyTexture(font_image_quit);
    SDL_DestroyTexture(font_image_tetrino_win);
    SDL_DestroyTexture(font_image_mouse_win);
    SDL_DestroyTexture(item_description[0]);
    SDL_DestroyTexture(item_description[1]);
    SDL_DestroyTexture(item_description[2]);
    SDL_DestroyTexture(tetro_description[0]);
    SDL_DestroyTexture(tetro_description[1]);

    IMG_Quit();

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void PlayState::pause() {
    music_engine->setAllSoundsPaused(true);
    paused = true;
}

void PlayState::resume() {
    music_engine->setAllSoundsPaused(false);
    paused = false;
}

// Restarts game.
void PlayState::reset() {
    // Empty the board.
    for (int i = 0; i < board->ROWS; i++) {
        for (int j = 0; j < board->COLS; j++) {
            board->color[i][j] = -1;
        }
    }

    // Delete objects.
    delete [] board;
    delete [] tetro;
    delete [] next_tetro;

    // Recreate objects.
    board = new Board();
    tetro = new Tetromino(rand()%7);
    next_tetro = new Tetromino(rand()%7);
    tetro->set_position(static_cast<int>(board->COLS/2), 0);
    next_tetro->set_position(board->COLS+5, static_cast<int>(0.3*board->ROWS));

    // Restart music.
    music_engine->stopAllSounds();
    music_engine->play2D("resources/sounds/Dubmood-Tetris.ogg", true);

    game_over       = false;
    winner          = 0;
    newgameup       = false;
    newgamedown     = false;

    paused = false;

    initDobestUpdate();
}

// Handle player input.
void PlayState::input(GameEngine *game) {
    // Queuing events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Clicking 'x' or pressing F4.
        if (event.type == SDL_QUIT) {
            exit = true;
        }

        // Key is pressed.
        if (event.type == SDL_KEYDOWN) {
            // Pause/Resume.
            if (event.key.keysym.sym == SDLK_p) {
                if (paused) {
                    resume();
                } else {
                    pause();
                }
            }

            if (!paused && !tetro->free_fall) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit = true;
                        break;
                    case SDLK_a: case SDLK_LEFT:
                        tetro->movement = tetro->LEFT;
                        tetro->shift = true;
                        break;
                    case SDLK_d: case SDLK_RIGHT:
                        tetro->movement = tetro->RIGHT;
                        tetro->shift = true;
                        break;
                    case SDLK_w: case SDLK_UP:
                        if (tetro->type != 2)  // type 3 is O-Block.
                            tetro->rotate = true;
                        break;
                    case SDLK_s: case SDLK_DOWN:
                        tetro->speed_up = true;
                        break;
                    case SDLK_SPACE:
                        tetro->free_fall = true;
                        break;
                    // 여기 바꾸면 홀드 키 변경 가능 
                    case SDLK_LSHIFT:           // hold
                        holdSwap();
                        break;
                    default:
                        break;
                }
            }
        }

        // Key is released.
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_s: case SDLK_DOWN:
                    tetro->speed_up = false;
                    break;
                default:
                    break;
            }
        }

        // Mouse moves.
        if (event.type == SDL_MOUSEMOTION && !paused) {
            // Outside of the board.
            if (event.motion.x > board->WIDTH + GAME_OFFSET)
                SDL_ShowCursor(1);  // Show cursor.

            // Inside the board.
            else
                SDL_ShowCursor(0);  // Don't show cursor.

            // move mousepos
            if(!game_over && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
                mouse.pos.x += mouse.motion_scale * event.motion.xrel;
                mouse.pos.y += mouse.motion_scale * event.motion.yrel;
            }
        }

        // Mouse button clicked.
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            switch (event.button.button) {
                // Left mouse button clicked.
                case SDL_BUTTON_LEFT: {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > newgamex1 &&
                        x < newgamex2) {
                        // And mouse cursor is on "New Game" button.
                        if (y > newgamey2 &&
                            y < newgamey1) {
                            newgamedown = true;
                        // And mouse cursor is on "Quit" button.
                        } else if (y > newgamey2+4*board->BLOCK_HEIGHT &&
                                   y < newgamey1+4*board->BLOCK_HEIGHT) {
                            quitdown = true;
                        }
                    }
                    break;
                }

                // when right click : generate block if mousestate == 3
                case SDL_BUTTON_RIGHT: {
                    if(mouse.state == 3) {
                        int clickx, clicky;
                        SDL_GetMouseState(&clickx, &clicky);
                        SDL_Point click_boardp = toBoardCoordinate(clickx, clicky);
                        
                        bool already_exist = false;
                        for(int i=0; i<tetro->SIZE; ++i) {
                            int tetrox = tetro->get_block_x(i);
                            int tetroy = tetro->get_block_y(i);
                            if(click_boardp.x == tetrox && click_boardp.y == tetroy) {
                                already_exist = true;
                                break;
                            }
                        }
                        if(board->color[click_boardp.y][click_boardp.x] != -1)
                            already_exist = true; 

                        if(!already_exist) {
                            board->color[click_boardp.y][click_boardp.x] = 10;
                            mouse.state = 0;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        // Mouse button released.
        if (event.type == SDL_MOUSEBUTTONUP) {
            switch (event.button.button) {
                // Left mouse button released.
                case SDL_BUTTON_LEFT: {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > newgamex1 && x < newgamex2) {
                        // And mouse cursor is on "New Game" button.
                        if (y > newgamey2 && y < newgamey1) {
                            newgameup = true;
                        // And mouse cursor is on "Quit" button.
                        } else if (y > newgamey2+4*board->BLOCK_HEIGHT &&
                                 y < newgamey1+4*board->BLOCK_HEIGHT) {
                            quitup = true;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void PlayState::release_tetromino() {
    Tetromino* new_tetro = new Tetromino(rand()%7);
    new_tetro->set_position(next_tetro->x, next_tetro->y);

    delete[] tetro;
    tetro = next_tetro;
    tetro->set_position(static_cast<int>(board->COLS/2), 0);

    next_tetro = new_tetro;

    tetro->drop();
}

// Update game values.
void PlayState::update(GameEngine* game) {
    static float deltatime = 0.0f;

    // New Game button was pressed.
    if (newgameup && newgamedown) {
        reset();
    }

    // Quit button or 'x'/F4 was pressed.
    if ((quitup && quitdown) || exit) {
        game->quit();
    }

    if (game_over || paused) {
        return;
    }
    
    // check if tetromino over mouse_area
    for(int i=0; i<board->COLS; ++i) {
        if(board->color[mouse_area_size.y-1][i] != -1) {
            game_over = true;
            winner = 2;
            return;
        }
    }

    // clamp mousepos
    {
        float xmin = mouse_area.x + mouse.size/2, xmax = xmin + mouse_area.w - mouse.size;
        float ymin = mouse_area.y + mouse.size/2, ymax = ymin + mouse_area.h - mouse.size;
        mouse.pos.x = mouse.pos.x > xmax ? xmax : (mouse.pos.x < xmin ? xmin : mouse.pos.x);
        mouse.pos.y = mouse.pos.y > ymax ? ymax : (mouse.pos.y < ymin ? ymin : mouse.pos.y);
    }


    // Tetromino has landed.
    if (tetro->has_landed()) {
        tetro->free_fall = false;

        // Add fallen tetromino to the board and check if tetromino.
        // has crossed over the top border.
        if (!board->add(tetro)) {
            game_over = true;
            winner = 2;
            return;
        }

        // Drop stored tetromino and replace by newly-generated tetromino.
        release_tetromino();

        // spawn item 
        if(!mouse_item.spawned && drand48() < item_spawn_prob && 0 == mouse.state) {
            int cnt = countMouseAreaBlocks();

            int xmin = mouse_area_size.x, xmax = xmin + mouse_area_size.w;
            int ymin = mouse_area_size.y, ymax = ymin + mouse_area_size.h;
            
            int remain = mouse_area_size.w * mouse_area_size.h - cnt - 1;   // -1 : mouse player position
            if(remain > 0) {
                srand((unsigned int)time(NULL));
                int randn = rand()%remain;
                SDL_Point mouse_player_pos = toBoardCoordinate(mouse.pos.x, mouse.pos.y);            
                for(int y=ymin; y<ymax; ++y) {
                    for(int x=xmin; x<xmax; ++x) {
                        // check dropped block
                        bool drop_exist = false;
                        for(int i=0; i < tetro->SIZE; ++i) {
                            int drop_x = tetro->get_block_x(i);
                            int drop_y = tetro->get_block_y(i);
                            if(drop_x == x && drop_y == y) {
                                drop_exist = true;
                                break;
                            }
                        }
                        if(drop_exist) continue;
                        if(x == mouse_player_pos.x && y == mouse_player_pos.y) continue;
                        if(board->color[y][x] == -1) {
                            if(0 == randn) {
                                // spawn
                                mouse_item.spawned = true;
                                mouse_item.pos.x = x;
                                mouse_item.pos.y = y;
                                // god_prob : des_block_prob : gen_block_prob = 1 : 1 : 1
                                mouse_item.type = drand48() > 0.33f ? 1 : (drand48() < 0.5f ? 2 : 3);
                                mouse_item.remain_time = item_remain_time;
                                break;
                            } else --randn;
                        }
                    }
                    if(0 == randn) break;
                }
            }
        }
    } else if (tetro->free_fall) {
        tetro->y++;  // Maximum speed.
    } else {  // Rotations and translations.
        // Rotation.
        if (tetro->rotate)
            tetro->rotate_left();

        // Update tetromino position on the x-axis.
        tetro->add_to_x(tetro->movement);

        // Assign the time required for tetromino to fall down one block.
        if (tetro->speed_up) {
            time_till_drop = 0.02f;  // 2x slower than free fall.
        } else {
            // Drop speed proportional to score.
            time_till_drop = 0.3f - board->get_score()*acceleration;
        }

        // Add time elapsed.
        deltatime = frame_rate(game, &last_time, &this_time);
        time_counter += deltatime;

        // time_till_drop = 0.3;
        // delta_time ~ 0.017 seconds.
        // Tetromino initially falls one block unit for
        // every 0.3/0.017 ~ 17 game loops.

        if (time_counter >= time_till_drop) {
            tetro->y++;  // Update tetromino position on the y-axis.
            time_counter = 0.0f;
        }
    }

    // Collision detection.
    // Check if tetromino is in an acceptable position,
    // if not, undo previous move(s).
    for (int i = 0; i < tetro->SIZE; i++) {
        // Coordinates of each block.
        int x = tetro->get_block_x(i);
        int y = tetro->get_block_y(i);

        // Block crosses wall after rotation and/or translation.
        if (x < 0 || x >= board->COLS) {
            // Because of rotation.
            if (tetro->rotate)
                tetro->rotate_right();  // Neutralize the left rotation.

            // Because of translation.
            if (tetro->shift)
                tetro->x -= tetro->movement;  // Neutralize shift.

            break;
        } else if (y >= board->ROWS) {  // Block touches ground.
            tetro->lands();
            // Change the value of Y so that block(s) of the (old)
            // tetromino is/are above the blue line.
            tetro->set_block_y(i, board->ROWS-1);
        } else if (y >= 0) {  // Block is on the board.
            // Block touched another block.
            if (board->color[y][x] != -1) {
                // Tetromino rotates and collides with a block.
                if (tetro->rotate || tetro->shift) {
                    if (tetro->rotate) {
                        tetro->rotate_right();  // Neutralize.
                    }
                    // Tetromino is shifted into another block.
                    if (tetro->shift) {
                        tetro->x -= tetro->movement;  // Neutralize.
                    }
                    break;
                } else {  // Block falls into another block.
                    tetro->y--;  // Neutralize: tetromino goes up.
                    tetro->lands();
                }
            }
        }
    }

    // reduce item's remain time
    if(mouse_item.spawned) {
        mouse_item.remain_time -= deltatime;
        if(mouse_item.remain_time < 0.0f)
            mouse_item.spawned = false;
    }

    // check if mouse_item collide with block 
    if(mouse_item.spawned) {
        if(board->color[mouse_item.pos.y][mouse_item.pos.x] != -1) {
            mouse_item.spawned = false;
        }
    }

    // check mouse player and item collision
    if(mouse_item.spawned) {
        SDL_Point mp = toBoardCoordinate(mouse.pos.x, mouse.pos.y);
        if(mp.x == mouse_item.pos.x && mp.y == mouse_item.pos.y) {
            mouse.state = mouse_item.type;
            mouse_item.spawned = false;
            mouse.state_remain_time = state_remain_times[mouse.state]; 
        }
    }

    // reduce state time 
    if(mouse.state != 0) {
        mouse.state_remain_time -= deltatime;
        if(mouse.state_remain_time < 0.0f)
            mouse.state = 0;
    }

    // Collision detection : mouse 
    {
        SDL_Point mp = toBoardCoordinate(mouse.pos.x, mouse.pos.y);
        // with dropped blocks
        for(int i=0; i < tetro->SIZE; ++i) {
            int x = tetro->get_block_x(i);
            int y = tetro->get_block_y(i);
            if(x == mp.x && y == mp.y) {
                if(1 != mouse.state) {
                    game_over = true;
                    winner = 1;
                    return;
                }
                // god : do nothing
            }
        }

        // with landed blocks
        if(board->color[mp.y][mp.x] != -1 &&
           board->color[mp.y][mp.x] != 10) {    // 10: mouse player generate block
            if(0 == mouse.state || 3 == mouse.state) {
                game_over = true;
                winner = 1;
                return;
            } else if(2 == mouse.state) {   // destroy block
                board->color[mp.y][mp.x] = -1;
                mouse.state = 0;
            }
            // god : do nothing
        }
    }

    // check if tetrino fill 80% 
    {
        int limit = mouse_area_size.w * mouse_area_size.h * 8 / 10;
        int cnt = countMouseAreaBlocks();
        
        if(cnt >= limit) {
            game_over = true;
            winner = 2;
            return;
        }
    }

    board->delete_full_rows();
    tetro->rotate = false;
    tetro->shift = false;
    tetro->movement = tetro->NONE;
}

// Render result.
void PlayState::render(GameEngine* game) {
    // Clear screen.
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    // Render "Tetris" text.
    int x = (next_tetro->x-3)*board->BLOCK_WIDTH;
    int y = GAME_OFFSET;

    render_texture(font_image_tetris, game->renderer, x, y);

    // Render "Pause" text if game is paused.
    if (paused)
        render_texture(font_image_pause, game->renderer, x, y+40);

    // Render score text.
    render_texture(font_image_score_text,
            game->renderer, x, y + board->BLOCK_WIDTH);

    // Render score.
    if (board->render_score) {
        font_image_score = render_text(std::to_string(board->get_score()),
                white, font_score, game->renderer);
        board->render_score = false;
    }
    render_texture(font_image_score,
            game->renderer, x + 60, y + board->BLOCK_WIDTH);

    int tetro_x, tetro_y;

    // Prepare textures.
    int iW, iH;
    SDL_QueryTexture(block_texture, nullptr, nullptr, &iW, &iH);

    SDL_Rect clips[7];
    for (int i = 0; i < 7; i++) {
        clips[i].x = 0;
        clips[i].y = i*24;
        clips[i].w = 20;
        clips[i].h = 20;
    }

    // Draw tetromino squares.
    for (int i = 0; i < tetro->SIZE; i++) {
        // Get new coordinates.
        tetro_x = tetro->get_block_x(i)*board->BLOCK_WIDTH + GAME_OFFSET;
        tetro_y = tetro->get_block_y(i)*board->BLOCK_HEIGHT + GAME_OFFSET;

        draw_block(game, tetro_x, tetro_y, tetro->type, clips);
    }

    // Draw shadow tetromino.
    int shadow_y[4];
    tetro->get_shadow(board, shadow_y);
    for (int i = 0; i < tetro->SIZE; i++) {
        if (shadow_y[i] < 0)
            break;
        int x = tetro->get_block_x(i)*board->BLOCK_WIDTH + GAME_OFFSET;
        int y = shadow_y[i]*board->BLOCK_WIDTH + GAME_OFFSET;

        // Draw block.
        SDL_SetRenderDrawColor(game->renderer, 180, 180, 180, 255);
        SDL_Rect shadow_block = {x, y, board->BLOCK_WIDTH, board->BLOCK_HEIGHT};
        SDL_RenderFillRect(game->renderer, &shadow_block);
    }

    if (!game_over) {
        // Draw next tetromino.
        for (int i = 0; i < next_tetro->SIZE; i++) {
            // Get new coordinates.
            tetro_x = next_tetro->get_block_x(i)*board->BLOCK_WIDTH;
            tetro_y = next_tetro->get_block_y(i)*board->BLOCK_HEIGHT;

            draw_block(game, tetro_x, tetro_y, next_tetro->type, clips);
        }

        // Draw hold tetro
        if(hold_tetro) {
            for(int i=0; i<hold_tetro->SIZE; ++i) {
                tetro_x = hold_tetro->get_block_x(i)*board->BLOCK_WIDTH;
                tetro_y = hold_tetro->get_block_y(i)*board->BLOCK_HEIGHT;

                draw_block(game, tetro_x, tetro_y, hold_tetro->type, clips);
            }
        }
    }

    // This is the board. Non-active tetrominos live here.
    for (int i = 0; i < board->ROWS; i++)
        for (int j = 0; j < board->COLS; j++)
            if (board->color[i][j] != -1) {
                // Get new coordinates.
                tetro_x = j*board->BLOCK_WIDTH + GAME_OFFSET;
                tetro_y = i*board->BLOCK_HEIGHT + GAME_OFFSET;

                if(board->color[i][j] == 10) {
                    SDL_Rect block_dst;
                    block_dst.x = tetro_x; block_dst.y = tetro_y;
                    block_dst.w = board->BLOCK_WIDTH; block_dst.h = board->BLOCK_HEIGHT;
                    SDL_SetRenderDrawColor(game->renderer, 
                                item_colors[3].r, item_colors[3].g, item_colors[3].b, item_colors[3].a);
                    SDL_RenderFillRect(game->renderer, &block_dst);
                } else {
                    draw_block(game, tetro_x, tetro_y, board->color[i][j], clips);
                }
            }


    // draw mouse point
    SDL_Rect mouse_img_dst;
    mouse_img_dst.x = (int)mouse.pos.x - mouse.size/2; mouse_img_dst.y = (int)mouse.pos.y - mouse.size/2;
    mouse_img_dst.w = mouse_img_dst.h = mouse.size;
    SDL_Color lerped_color = mouse_colors[0];
    if(mouse.state != 0) {
        lerped_color = lerpColor(mouse_color_mid, mouse_colors[mouse.state], 
                                 mouse.state_remain_time/state_remain_times[mouse.state]);
    }
    SDL_SetRenderDrawColor(game->renderer, 
                           lerped_color.r, 
                           lerped_color.g, 
                           lerped_color.b, 
                           lerped_color.a);
    SDL_RenderFillRect(game->renderer, &mouse_img_dst);

    // draw item
    if(mouse_item.spawned) {
        SDL_Rect mouse_item_draw_dst;
        mouse_item_draw_dst.x = GAME_OFFSET + mouse_item.pos.x*(board->BLOCK_WIDTH) + (board->BLOCK_WIDTH)/2 - mouse_item.size/2;
        mouse_item_draw_dst.y = GAME_OFFSET + mouse_item.pos.y*(board->BLOCK_HEIGHT) + (board->BLOCK_HEIGHT)/2 - mouse_item.size/2;
        mouse_item_draw_dst.w = mouse_item_draw_dst.h = mouse_item.size;
        SDL_SetRenderDrawColor(game->renderer,
                            item_colors[mouse_item.type].r,
                            item_colors[mouse_item.type].g,
                            item_colors[mouse_item.type].b,
                            item_colors[mouse_item.type].a);
        SDL_RenderFillRect(game->renderer, &mouse_item_draw_dst);
    }

    // Mouse Area Box
    SDL_SetRenderDrawColor(game->renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(game->renderer, &mouse_area);

    // Box surrounding board.

    // Set color to white.
    SDL_SetRenderDrawColor(game->renderer, 180, 180, 180, 255);

    // Draw left border.
    SDL_RenderDrawLine(game->renderer,
        GAME_OFFSET, GAME_OFFSET, GAME_OFFSET, GAME_OFFSET+board->HEIGHT);

    // Draw right border.
    SDL_RenderDrawLine(game->renderer,
            GAME_OFFSET+board->WIDTH, GAME_OFFSET,
            GAME_OFFSET+board->WIDTH, GAME_OFFSET+board->HEIGHT);

    // Draw upper border.
    SDL_RenderDrawLine(game->renderer,
            GAME_OFFSET, GAME_OFFSET, GAME_OFFSET+board->WIDTH, GAME_OFFSET);

    // Draw bottom border.
    SDL_RenderDrawLine(game->renderer,
            GAME_OFFSET, GAME_OFFSET+board->HEIGHT,
            GAME_OFFSET+board->WIDTH, GAME_OFFSET+board->HEIGHT);

    // If game is over, display "Game Over!".
    if (game_over) {
        render_texture(winner == 1 ? font_image_tetrino_win : font_image_mouse_win,
                game->renderer, 
                0, 150);
    }

    // display item description
    {
        int starty = 400;
        int each_description_height=15;
        for(int i=0; i<3; ++i) {
            render_texture(item_description[i], game->renderer,
                           newgamex1,
                           starty + each_description_height*i);
        }
    }

    // display tetro description
    {
        int render_x = GAME_OFFSET + (next_tetro->x+2)*board->BLOCK_WIDTH;
        int render_y = GAME_OFFSET + (next_tetro->y+1)*board->BLOCK_HEIGHT;
        render_texture(tetro_description[0], game->renderer, 
                       render_x, render_y);
        render_x = GAME_OFFSET + (hold_tetro_pos.x+2)*board->BLOCK_WIDTH;
        render_y = GAME_OFFSET + (hold_tetro_pos.y+1)*board->BLOCK_HEIGHT;
        render_texture(tetro_description[1], game->renderer,
                       render_x, render_y);
    }

    // Create "New Game" button.
    int blue[4] = {0, 0, 255, 255};
    create_button(game, newgamex1, newgamey2,
            7*board->BLOCK_WIDTH, 2*board->BLOCK_HEIGHT, blue);

    // Render "New Game" font.
    render_texture(font_image_new_game,
            game->renderer, newgamex1+10, newgamey2+10);

    // Create "Quit" button.
    int red[4] = {255, 0, 0, 255};
    create_button(game, newgamex1,
            newgamey2+4*board->BLOCK_HEIGHT, 7*board->BLOCK_WIDTH,
            2*board->BLOCK_HEIGHT, red);

    // Render "Quit" font.
    render_texture(font_image_quit,
            game->renderer, newgamex1+10, newgamey2+4*board->BLOCK_HEIGHT+10);

    // Swap buffers.
    SDL_RenderPresent(game->renderer);
}

// Create "New Game" and "Quit" buttons.
void PlayState::create_button(GameEngine* game,
        int x, int y, int width, int height, int color[]) {
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(game->renderer,
            color[0], color[1], color[2], color[3]);
    SDL_RenderFillRect(game->renderer, &rect);
}

// Render Tetromino block.
void PlayState::draw_block(GameEngine* game,
        int x, int y, int k, SDL_Rect clips[]) {
    render_texture(block_texture, game->renderer, x, y, &clips[k]);
}

float PlayState::frame_rate(GameEngine* game, int* last_time, int* this_time) {
    // Get number of milliseconds since SDL_Init() of the previous frame.
    *last_time = *this_time;

    // Get number of milliseconds since SDL_Init().
    *this_time = SDL_GetTicks();

    // Variation of time between each game iteration.
    // Dividing by 1000 to convert in seconds.
    return ((*this_time - *last_time) / 1000.0f);
}

SDL_Point PlayState::toBoardCoordinate(float x, float y) {
    SDL_Point p; p.x = (int)x; p.y = (int)y;
    p.x -= GAME_OFFSET;
    p.y -= GAME_OFFSET;
    p.x /= board->BLOCK_WIDTH;
    p.y /= board->BLOCK_HEIGHT;
    return p;
}

int PlayState::countMouseAreaBlocks() {
    int cnt = 0;

    int xmin = mouse_area_size.x, xmax = xmin + mouse_area_size.w;
    int ymin = mouse_area_size.y, ymax = ymin + mouse_area_size.h;

    // count dropped block
    for(int i=0; i < tetro->SIZE; ++i) {
        int x = tetro->get_block_x(i);
        int y = tetro->get_block_y(i);
            
        if(x >= xmin && x < xmax && y >= ymin && y < ymax) ++cnt;
    }

    // count landed blocks
    for(int y=ymin; y < ymax; ++y) {
        for(int x=xmin; x < xmax; ++x) {
            if(board->color[y][x] != -1) ++cnt;
        }
    }

    return cnt;
}

SDL_Color PlayState::lerpColor(SDL_Color a, SDL_Color b, float t) {
    SDL_Color result;
    result.r = (int)((1.0f-t)*a.r + t*b.r);
    result.g = (int)((1.0f-t)*a.g + t*b.g);
    result.b = (int)((1.0f-t)*a.b + t*b.b);
    result.a = (int)((1.0f-t)*a.a + t*b.a);
    return result;
}

void PlayState::holdSwap() {
    Tetromino *last_hold_tetro = hold_tetro;
    hold_tetro = tetro;
    tetro->set_position(hold_tetro_pos.x, hold_tetro_pos.y);
    tetro->status = Tetromino::Status::INACTIVE;

    if(last_hold_tetro) {
        tetro = last_hold_tetro;
    } else {
        Tetromino* new_tetro = new Tetromino(rand()%7);
        new_tetro->set_position(next_tetro->x, next_tetro->y);

        tetro = next_tetro;
        next_tetro = new_tetro;
    }
    tetro->set_position(static_cast<int>(board->COLS/2), 0);
    tetro->drop();
}