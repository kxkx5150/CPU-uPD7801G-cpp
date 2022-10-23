#include "emu.h"
#include "../utils/vm.h"
#include "../utils/config.h"


extern config_t config;
#define FRAME_PITCH (SCREEN_WIDTH * sizeof(uint16_t))


EMU::EMU()
{
    static int    freq_table[4] = {11025, 22050, 44100, 48000};
    static double late_table[4] = {0.1, 0.2, 0.3, 0.4};

    if (!(0 <= config.sound_frequency && config.sound_frequency <= 3))
        config.sound_frequency = 3;
    if (!(0 <= config.sound_latency && config.sound_latency <= 3))
        config.sound_latency = 0;

    int frequency = freq_table[config.sound_frequency];
    int latency   = (int)(frequency * late_table[config.sound_latency]);

    vm = new VM(this);
    initialize_input();
    initialize_sound(frequency, latency);
}
EMU::~EMU()
{
    release_input();
    release_sound();

    if (vm)
        delete vm;
}

void EMU::run_cpu(uint64_t cycles)
{
    for (int i = 0; i < cycles; i++) {
        vm->run();
    }
}
void EMU::run()
{
    update_input();
    // update_sound();
    // update_timer();
}
void EMU::reset()
{
    vm->reset();
}
void EMU::open_cart(char *filename)
{
    vm->open_cart(filename);
}
void EMU::close_cart()
{
    vm->close_cart();
}
bool EMU::now_skip()
{
    return vm->now_skip();
}
void EMU::update_config()
{
    vm->update_config();
}
void EMU::set_screen_size(int width, int height)
{
    window_width  = width;
    window_height = height;
}
void EMU::draw_screen(SDL_Renderer *renderer, SDL_Texture *tex)
{
    vm->draw_screen();
    SDL_UpdateTexture(tex, NULL, lpBMP, FRAME_PITCH);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
}
