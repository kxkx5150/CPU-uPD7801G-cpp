#include <SDL2/SDL.h>
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
            emu->run(4000000 / 60);
            emu->draw_screen(renderer, tex);
            last_tic = SDL_GetTicks();
        }

        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {
                case SDL_QUIT:
                    Running = 0;
                    break;
                case SDL_KEYDOWN:
                    Key = Event.key.keysym.sym;
                    break;
                case SDL_KEYUP:
                    Key = Event.key.keysym.sym;
                    break;
            }
        }
    }
}
