// Copyright [2015] <Chafic Najjar>

#include "src/menustate.h"
#include "src/helpstate.h"
#include "src/playstate.h"
#include "src/utilities.h"

MenuState MenuState::m_menustate;

void MenuState::init(GameEngine* game) {
    play = false;
    play2 = false; //기본 테트리스 모드
    help = false; //도움말
    exit = false;

    // Initialize fonts and font color.
    TTF_Init();
    white = { 255, 255, 255 };

    // Load fonts and font textures.
    font_title = TTF_OpenFont("resources/fonts/Basica.ttf", 32);
    font_play = TTF_OpenFont("resources/fonts/Basica.ttf", 16);
    font_play2 = TTF_OpenFont("resources/fonts/Basica.ttf", 16); //기본 테트리스 모드
    font_help = TTF_OpenFont("resources/fonts/Basica.ttf", 16); //도움말
    font_quit = TTF_OpenFont("resources/fonts/Basica.ttf", 16);

    font_image_title = render_text("Tetris", white, font_title, game->renderer);
    font_image_play = render_text("MultiPlay", white, font_play, game->renderer);
    font_image_play2 = render_text("SinglePlay", white, font_play2, game->renderer); //기본 테트리스 모드
    font_image_help = render_text("Help", white, font_help, game->renderer); //도움말
    font_image_quit = render_text("Quit", white, font_quit, game->renderer);

    // Text position.
    SDL_QueryTexture(font_image_title,
            nullptr, nullptr, &title_width, &title_height);
    SDL_QueryTexture(font_image_play,
            nullptr, nullptr, &play_width, &play_height);
    SDL_QueryTexture(font_image_play2,
            nullptr, nullptr, &play2_width, &play2_height); //기본 테트리스 모드
    SDL_QueryTexture(font_image_help,
            nullptr, nullptr, &help_width, &help_height); //도움말
    SDL_QueryTexture(font_image_quit,
            nullptr, nullptr, &quit_width, &quit_height);

    currently_selected = 0;
    //items = 2;
    //items = 3;
    items = 4; //다음 메뉴 개수
}

void MenuState::clean_up(GameEngine* game) {
    // Close all fonts.
    TTF_CloseFont(font_title);
    TTF_CloseFont(font_play);
    TTF_CloseFont(font_play2); //기본 테트리스 모드
    TTF_CloseFont(font_help); //도움말
    TTF_CloseFont(font_quit);

    // Destroy all textures.
    SDL_DestroyTexture(font_image_title);
    SDL_DestroyTexture(font_image_play);
    SDL_DestroyTexture(font_image_play2); //기본 테트리스 모드
    SDL_DestroyTexture(font_image_help); //도움말
    SDL_DestroyTexture(font_image_quit);

    IMG_Quit();
}

void MenuState::pause() {}

void MenuState::resume() {}

void MenuState::reset() {}

void MenuState::input(GameEngine* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Clicking 'x' or pressing F4.
        if (event.type == SDL_QUIT)  {
            exit = true;
        }

        // Key is pressed.
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exit = true;
                    break;
                case SDLK_UP:
                    select_up();
                    break;
                case SDLK_DOWN:
                    select_down();
                    break;
                case SDLK_RETURN:
                    if (currently_selected == 0)
                        play = true;
                    else if (currently_selected == 1) //기본 테트리스 모드
                        play2 = true;
                    else if (currently_selected == 2) //도움말
                        help = true;
                    else if (currently_selected == 3)
                        exit = true;
                    break;
                default:
                    break;
            }
        }
    }
}

void MenuState::update(GameEngine* game) {
    if (play) {
        game->multi(); //멀티모드 실행
        game->push_state(PlayState::Instance());
    } else if (play2) { //기본 테트리스 모드
        game->single(); //싱글모드 실행
        game->push_state(PlayState::Instance());
    } else if (help) {
        game->push_state(HelpState::Instance()); //도움말
    } else if (exit) {
        game->quit();
    }
}

void MenuState::render(GameEngine* game) {
    // Clear screen.
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    // Width and height of each text and space between menu items.
    int height, width;
    int space = 30;

    // Draw logo.
    SDL_QueryTexture(font_image_title, nullptr, nullptr, &width, &height);
    render_texture(font_image_title, game->renderer,
            (game->width - title_width)/2,
            (game->height - title_height)/2-space*2);

    // Underline the selected item.
    if (currently_selected == 0) {
        TTF_SetFontStyle(font_play, TTF_STYLE_UNDERLINE);
        font_image_play = render_text("MultiPlay", white, font_play, game->renderer);
    } else if (currently_selected == 1) { //기본 테트리스 모드
        TTF_SetFontStyle(font_play2, TTF_STYLE_UNDERLINE);
        font_image_play2 = render_text("SinglePlay", white, font_play2, game->renderer);
    } else if (currently_selected == 2) { //도움말
        TTF_SetFontStyle(font_help, TTF_STYLE_UNDERLINE);
        font_image_help = render_text("Help", white, font_help, game->renderer);
    } else if (currently_selected == 3) {
        TTF_SetFontStyle(font_quit, TTF_STYLE_UNDERLINE);
        font_image_quit = render_text("Quit", white, font_quit, game->renderer);
    }

    // Draw menu items (centered).
    render_texture(font_image_play, game->renderer,
            (game->width - play_width)/2, (game->height - play_height)/2);
    render_texture(font_image_play2, game->renderer, //기본 테트리스 모드
            (game->width - play2_width)/2,
            (game->height - play2_height)/2+space);
    render_texture(font_image_help, game->renderer,
            (game->width - help_width)/2,
            (game->height - help_height)/2+space*2); //도움말
    render_texture(font_image_quit, game->renderer,
            (game->width - quit_width)/2,
            (game->height - quit_height)/2+space*3);

    // Remove underline again.
    if (currently_selected == 0) {
        TTF_SetFontStyle(font_play, TTF_STYLE_NORMAL);
        font_image_play = render_text("MultiPlay", white, font_play, game->renderer);
    } else if (currently_selected == 1) {
        TTF_SetFontStyle(font_play2, TTF_STYLE_NORMAL);
        font_image_play2 = render_text("SinglePlay", white, font_play2, game->renderer); //기본 테트리스 모드
    } else if (currently_selected == 2) {
        TTF_SetFontStyle(font_help, TTF_STYLE_NORMAL);
        font_image_help = render_text("Help", white, font_help, game->renderer); //도움말
    } else if (currently_selected == 3) {
        TTF_SetFontStyle(font_quit, TTF_STYLE_NORMAL);
        font_image_quit = render_text("Quit", white, font_quit, game->renderer);
    }

    // Swap buffers.
    SDL_RenderPresent(game->renderer);
}

void MenuState::select_up() {
    if (currently_selected > 0) {
        currently_selected = (currently_selected-1)%items;
    }
}

void MenuState::select_down() {
    if (currently_selected < items-1) {
        currently_selected = (currently_selected+1)%items;
    }
}
