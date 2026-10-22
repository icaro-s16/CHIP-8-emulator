#include "chip8_interface.h"

SDL_Window* window = NULL;

SDL_Renderer* renderer = NULL;

SDL_Texture* texture = NULL;

int chip8_update_keys_map(Chip8VM* vm){
    int quit = 0;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = 1;
            } break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        quit = true;
                    } break;

                    case SDLK_x:
                    {
                        vm->keys[0] = 1;
                    } break;

                    case SDLK_1:
                    {
                        vm->keys[1] = 1;
                    } break;

                    case SDLK_2:
                    {
                        vm->keys[2] = 1;
                    } break;

                    case SDLK_3:
                    {
                        vm->keys[3] = 1;
                    } break;

                    case SDLK_q:
                    {
                        vm->keys[4] = 1;
                    } break;

                    case SDLK_w:
                    {
                        vm->keys[5] = 1;
                    } break;

                    case SDLK_e:
                    {
                        vm->keys[6] = 1;
                    } break;

                    case SDLK_a:
                    {
                        vm->keys[7] = 1;
                    } break;

                    case SDLK_s:
                    {
                        vm->keys[8] = 1;
                    } break;

                    case SDLK_d:
                    {
                        vm->keys[9] = 1;
                    } break;

                    case SDLK_z:
                    {
                        vm->keys[0xA] = 1;
                    } break;

                    case SDLK_c:
                    {
                        vm->keys[0xB] = 1;
                    } break;

                    case SDLK_4:
                    {
                        vm->keys[0xC] = 1;
                    } break;

                    case SDLK_r:
                    {
                        vm->keys[0xD] = 1;
                    } break;

                    case SDLK_f:
                    {
                        vm->keys[0xE] = 1;
                    } break;

                    case SDLK_v:
                    {
                        vm->keys[0xF] = 1;
                    } break;
                }
            } break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_x:
                    {
                        vm->keys[0] = 0;
                    } break;

                    case SDLK_1:
                    {
                        vm->keys[1] = 0;
                    } break;

                    case SDLK_2:
                    {
                        vm->keys[2] = 0;
                    } break;

                    case SDLK_3:
                    {
                        vm->keys[3] = 0;
                    } break;

                    case SDLK_q:
                    {
                        vm->keys[4] = 0;
                    } break;

                    case SDLK_w:
                    {
                        vm->keys[5] = 0;
                    } break;

                    case SDLK_e:
                    {
                        vm->keys[6] = 0;
                    } break;

                    case SDLK_a:
                    {
                        vm->keys[7] = 0;
                    } break;

                    case SDLK_s:
                    {
                        vm->keys[8] = 0;
                    } break;

                    case SDLK_d:
                    {
                        vm->keys[9] = 0;
                    } break;

                    case SDLK_z:
                    {
                        vm->keys[0xA] = 0;
                    } break;

                    case SDLK_c:
                    {
                        vm->keys[0xB] = 0;
                    } break;

                    case SDLK_4:
                    {
                        vm->keys[0xC] = 0;
                    } break;

                    case SDLK_r:
                    {
                        vm->keys[0xD] = 0;
                    } break;

                    case SDLK_f:
                    {
                        vm->keys[0xE] = 0;
                    } break;

                    case SDLK_v:
                    {
                        vm->keys[0xF] = 0;
                    } break;
                }
            } break;
        }
    }

    return quit;
}

void chip8_update_display(
    Chip8VM* vm
){
    assert(vm != NULL);

    SDL_UpdateTexture(
        texture, 
        NULL, 
        vm->display, 
        sizeof(vm->display[0]) * CHIP8_DISPLAY_WIDTH
    );
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void chip8_construct_display(){
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(
        "CHIP 8", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        CHIP8_DISPLAY_WIDTH * SCALE, 
        CHIP8_DISPLAY_HEIGHT * SCALE, 
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,  
        SDL_RENDERER_ACCELERATED
    );

    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        CHIP8_DISPLAY_WIDTH, 
        CHIP8_DISPLAY_HEIGHT
    );
}

void chip8_destroy_display(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


