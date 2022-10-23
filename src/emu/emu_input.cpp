#include "emu.h"

#define KEY_KEEP_FRAMES 3

void EMU::initialize_input()
{
}

void EMU::release_input()
{
    if (mouse_enable)
        disenable_mouse();
}

void EMU::update_input()
{
}

void EMU::key_down(int code)
{
}

void EMU::key_up(int code)
{
    key_status[code] &= 0x7f;
}

void EMU::enable_mouse()
{
    mouse_enable = true;
}

void EMU::disenable_mouse()
{
    mouse_enable = false;
}

void EMU::toggle_mouse()
{
}
