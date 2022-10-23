#include "io.h"
#include "upd7801.h"


void IO::initialize()
{
    key = emu->key_buffer();
    joy = emu->joy_buffer();
}

void IO::reset()
{
    pa = 0xff;
    pb = pc = si = so = 0;
}

void IO::write_io8(uint32 addr, uint32 data)
{
    switch (addr) {
        case P_A:
            pa = data;
            break;
        case P_B:
            pb = data;
            break;
        case P_C:
            if ((uint8)(data & 0x60) != (pc & 0x60))
                memory->write_io8(addr, data);
            if ((uint8)(data & 0x8) != (pc & 0x8))
                sound->write_io8(addr, data);
            pc = data;
            break;
        case P_SI:
            si = data;
            break;
        case P_SO:
            so = data;
            break;
    }
}

uint32 IO::read_io8(uint32 addr)
{
    uint32 val = 0xff;

    switch (addr) {
        case P_A:
            return pa;
        case P_B:
            if (!(pa & 0x01)) {
                if ((joy[0] & 0x04) || key[0x25])
                    val &= ~0x01;
                if ((joy[0] & 0x01) || key[0x26])
                    val &= ~0x02;
                if ((joy[0] & 0x10) || key[0x5a])
                    val &= ~0x04;
                if (joy[1] & 0x04)
                    val &= ~0x08;
                if (joy[1] & 0x01)
                    val &= ~0x10;
                if (joy[1] & 0x10)
                    val &= ~0x20;
            }
            if (!(pa & 0x02)) {
                if ((joy[0] & 0x02) || key[0x28])
                    val &= ~0x01;
                if ((joy[0] & 0x08) || key[0x27])
                    val &= ~0x02;
                if ((joy[0] & 0x20) || key[0x58])
                    val &= ~0x04;
                if (joy[1] & 0x02)
                    val &= ~0x08;
                if (joy[1] & 0x08)
                    val &= ~0x10;
                if (joy[1] & 0x20)
                    val &= ~0x20;
            }
            if (!(pa & 0x04)) {
                if (key[0x30] || key[0x60])
                    val &= ~0x40;
                if (key[0x31] || key[0x61])
                    val &= ~0x80;
            }
            if (!(pa & 0x08)) {
                if (key[0x32] || key[0x62])
                    val &= ~0x40;
                if (key[0x33] || key[0x63])
                    val &= ~0x80;
            }
            if (!(pa & 0x10)) {
                if (key[0x34] || key[0x64])
                    val &= ~0x40;
                if (key[0x35] || key[0x65])
                    val &= ~0x80;
            }
            if (!(pa & 0x20)) {
                if (key[0x36] || key[0x66])
                    val &= ~0x40;
                if (key[0x37] || key[0x67])
                    val &= ~0x80;
            }
            if (!(pa & 0x40)) {
                if (key[0x38] || key[0x68])
                    val &= ~0x40;
                if (key[0x39] || key[0x69])
                    val &= ~0x80;
            }
            if (!(pa & 0x80)) {
                if (key[0x08])
                    val &= ~0x40;
                if (key[0x0d])
                    val &= ~0x80;
            }
            return val;
        case P_C:
            return (pc & 0x78) | 0x82 | (key[0x20] ? 0 : 1);
    }
    return 0xff;
}
