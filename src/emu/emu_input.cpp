#include "emu.h"

#define KEY_KEEP_FRAMES 3


void EMU::initialize_input()
{
    memset(key_status, 0, sizeof(key_status));
    memset(joy_status, 0, sizeof(joy_status));
    memset(mouse_status, 0, sizeof(mouse_status));
}
void EMU::release_input()
{
    if (mouse_enable)
        disenable_mouse();
}
void EMU::update_input()
{
    for (int i = 0; i < 256; i++) {
        if (key_status[i] & 0x7f)
            key_status[i] = (key_status[i] & 0x80) | ((key_status[i] & 0x7f) - 1);
    }
}
void EMU::key_down(int code)
{
    key_status[code] = 0x80;
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
