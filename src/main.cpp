#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <GL/glew.h>
#include <sys/types.h>
#include <thread>
#include "emu/emu.h"

#define SCALE 3


int main(int argc, char **argv)
{
    bool Running = true;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window   *window   = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH1 * SCALE,
                                              WINDOW_HEIGHT1 * SCALE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture  *tex =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH1, WINDOW_HEIGHT1);
    SDL_RenderSetScale(renderer, SCALE, SCALE);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Event   Event;
    SDL_Keycode Key;
    uint32_t    last_tic = SDL_GetTicks();

    EMU *emu = new EMU();
    emu->set_screen_size(WINDOW_WIDTH1, WINDOW_HEIGHT1);
    emu->open_cart(argv[1]);

    while (Running) {
        if ((SDL_GetTicks() - last_tic) >= 1000.0 / 60.0) {
            emu->run_cpu(2);
            emu->run();
            emu->draw_screen(renderer, tex);
            last_tic = SDL_GetTicks();
        }

        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {
                case SDL_QUIT:
                    Running = 0;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    bool down = SDL_KEYDOWN ? true : false;
                    Key       = Event.key.keysym.sym;

                    switch (Key) {
                        case SDLK_DOWN:
                            if (down)
                                emu->key_down(0x28);
                            else
                                emu->key_up(0x28);
                            break;
                        case SDLK_UP:
                            if (down)
                                emu->key_down(0x26);
                            else
                                emu->key_up(0x26);
                            break;
                        case SDLK_LEFT:
                            if (down)
                                emu->key_down(0x25);
                            else
                                emu->key_up(0x25);
                            break;
                        case SDLK_RIGHT:
                            if (down)
                                emu->key_down(0x27);
                            else
                                emu->key_up(0x27);
                            break;
                        case SDLK_x:
                            if (down)
                                emu->key_down(0x5a);
                            else
                                emu->key_up(0x5a);
                            break;
                        case SDLK_z:
                            if (down)
                                emu->key_down(0x58);
                            else
                                emu->key_up(0x58);
                            break;
                        case SDLK_RETURN:
                            if (down)
                                emu->key_down(0x0d);
                            else
                                emu->key_up(0x0d);
                            break;
                        case SDLK_BACKSPACE:
                            if (down)
                                emu->key_down(0x08);
                            else
                                emu->key_up(0x08);
                            break;
                        case SDLK_0:
                            if (down)
                                emu->key_down(0x30);
                            else
                                emu->key_up(0x30);
                            break;
                        case SDLK_1:
                            if (down)
                                emu->key_down(0x31);
                            else
                                emu->key_up(0x31);
                            break;
                        case SDLK_2:
                            if (down)
                                emu->key_down(0x32);
                            else
                                emu->key_up(0x32);
                            break;
                        case SDLK_3:
                            if (down)
                                emu->key_down(0x33);
                            else
                                emu->key_up(0x33);
                            break;
                        case SDLK_4:
                            if (down)
                                emu->key_down(0x34);
                            else
                                emu->key_up(0x34);
                            break;
                        case SDLK_5:
                            if (down)
                                emu->key_down(0x35);
                            else
                                emu->key_up(0x35);
                            break;
                        case SDLK_6:
                            if (down)
                                emu->key_down(0x36);
                            else
                                emu->key_up(0x36);
                            break;
                        case SDLK_7:
                            if (down)
                                emu->key_down(0x37);
                            else
                                emu->key_up(0x37);
                            break;
                        case SDLK_8:
                            if (down)
                                emu->key_down(0x38);
                            else
                                emu->key_up(0x38);
                            break;
                        case SDLK_9:
                            if (down)
                                emu->key_down(0x39);
                            else
                                emu->key_up(0x39);
                            break;
                    }
                    break;
            }
        }
    }
}
