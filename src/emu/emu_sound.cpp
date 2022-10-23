#include "emu.h"
#include "../utils/vm.h"
#include "../fileio.h"

#define DSOUND_BUFFER_SIZE  (DWORD)(sound_samples * 4)
#define DSOUND_BUFFER_HALF  (DWORD)(sound_samples * 2)
#define WAVEOUT_BUFFER_SIZE (DWORD)(sound_samples * 2)


void EMU::initialize_sound(int rate, int samples)
{
    sound_ok = true;
}
void EMU::release_sound()
{
    stop_rec_sound();
}
void EMU::update_sound()
{
    now_mute = false;
}
void EMU::mute_sound()
{
    now_mute = true;
}
void EMU::start_rec_sound()
{
}
void EMU::stop_rec_sound()
{
}
