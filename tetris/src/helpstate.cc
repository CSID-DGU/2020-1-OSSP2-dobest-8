// Copyright [2020] <CSID-DGU/dobest>

#include "src/helpstate.h"
#include "src/menustate.h"
#include "src/utilities.h"
#include "src/board.h"
#include "src/tetromino.h"

HelpState HelpState::m_helpstate;

void HelpState::init(GameEngine* game) {
    help = load_texture("resources/images/help.png", game->renderer);
    exit = false;

     // Fonts.
    TTF_Init();
    white = { 255, 255, 255 };
    font_menu = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);
    font_quit = TTF_OpenFont("resources/fonts/bitwise.ttf", 20);

    font_image_menu = render_text("Menu",
            white, font_menu, game->renderer);
    font_image_quit = render_text("Quit",
            white, font_quit, game->renderer);
    
    quitdown        = false;
    quitup          = false;
    menudown        = false;
    menuup          = false;

    // Buttons coordinates.
    newgamex1       = GAME_OFFSET+board->WIDTH+board->BLOCK_WIDTH;
    newgamex2       = GAME_OFFSET+board->WIDTH+8*board->BLOCK_WIDTH;
    newgamey1       = board->HEIGHT-4*board->BLOCK_HEIGHT;
    newgamey2       = board->HEIGHT-6*board->BLOCK_HEIGHT;
}

void HelpState::clean_up(GameEngine* game) {
    TTF_CloseFont(font_menu);
    TTF_CloseFont(font_quit);

    SDL_DestroyTexture(font_image_menu);
    SDL_DestroyTexture(font_image_quit);

    IMG_Quit();
}

void HelpState::pause() {}

void HelpState::resume() {}

void HelpState::reset() {}

void HelpState::input(GameEngine* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Clicking 'x' or pressing F4.
        if (event.type == SDL_QUIT)
            exit = true;

        // Key is pressed.
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exit = true;
                    break;
                default:
                    break;
            }
        }

        // Mouse button clicked.
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            switch (event.button.button) 
            {
                // Left mouse button clicked.
                case SDL_BUTTON_LEFT: 
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > newgamex1 &&
                        x < newgamex2) 
                    {
                        // And mouse cursor is on "Menu" button.
                        if (y > newgamey2+(2.5)*board->BLOCK_HEIGHT &&
                                   y < newgamey1+(2.5)*board->BLOCK_HEIGHT) 
                        {
                            menudown = true;
                        }
                        // And mouse cursor is on "Quit" button.
                        else if (y > newgamey2+5*board->BLOCK_HEIGHT &&
                                   y < newgamey1+5*board->BLOCK_HEIGHT) 
                        {
                            quitdown = true;
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
                case SDL_BUTTON_LEFT: 
                {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > newgamex1 && x < newgamex2) 
                    {
                        // And mouse cursor is on "Menu" button.
                        if (y > newgamey2+(2.5)*board->BLOCK_HEIGHT &&
                                 y < newgamey1+(2.5)*board->BLOCK_HEIGHT) 
                        {
                            menuup = true;
                        }
                        // And mouse cursor is on "Quit" button.
                        else if (y > newgamey2+5*board->BLOCK_HEIGHT &&
                                 y < newgamey1+5*board->BLOCK_HEIGHT) 
                        {
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

void HelpState::update(GameEngine* game) {
    // Menu button was pressed.
    // 메인메뉴로 돌아감
    if (menuup && menudown) {
        game->push_state(MenuState::Instance());
    }

    // Quit button or 'x'/F4 was pressed.
    if ((quitup && quitdown) || exit) {
        game->quit();
    }

}

void HelpState::render(GameEngine* game) {
    // Clear screen.
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    render_help(game);

    // Swap buffers.
    SDL_RenderPresent(game->renderer);
}

void HelpState::render_help(GameEngine* game) {
    SDL_Rect src, dst;
    src.x = 0;
    src.y = 0;
    SDL_QueryTexture(help, nullptr, nullptr, &src.w, &src.h);
    
    dst.w = 480;
    dst.h = 480;
    dst.x = game->width / 2 - dst.w / 2;
    dst.y = game->height / 2 - dst.h / 2 - 70;
    SDL_RenderCopy(game->renderer, help, &src, &dst);

    // Create "Menu" button.
    int pink[4] = {157, 73, 97, 255};
    create_button(game, newgamex1,
            newgamey2+(2.5)*board->BLOCK_HEIGHT, 7*board->BLOCK_WIDTH,
            2*board->BLOCK_HEIGHT, pink);

    // Render "Menu" font.
    render_texture(font_image_menu,
            game->renderer, newgamex1+10, newgamey2+(2.5)*board->BLOCK_HEIGHT+10);

    // Create "Quit" button.
    int red[4] = {219, 68, 85, 255};
    create_button(game, newgamex1,
            newgamey2+5*board->BLOCK_HEIGHT, 7*board->BLOCK_WIDTH,
            2*board->BLOCK_HEIGHT, red);

    // Render "Quit" font.
    render_texture(font_image_quit,
            game->renderer, newgamex1+10, newgamey2+5*board->BLOCK_HEIGHT+10);
}

// Create "Menu" and "Quit" buttons.
void HelpState::create_button(GameEngine* game,
        int x, int y, int width, int height, int color[]) {
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(game->renderer,
            color[0], color[1], color[2], color[3]);
    SDL_RenderFillRect(game->renderer, &rect);
}