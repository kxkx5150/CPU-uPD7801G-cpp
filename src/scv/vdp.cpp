#include "vdp.h"
#include "upd7801.h"


void VDP::initialize()
{
    for (int i = 0, c = 0x20, p = 0; i < 3; i++, c += 32) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 8; k += 2) {
                uint8 d0 = font_ptr[p++];
                uint8 d1 = font_ptr[p++];
                uint8 d2 = font_ptr[p++];
                uint8 d3 = font_ptr[p++];

                font[c + j][k]          = (d0 & 0xf0) | (d1 >> 4);
                font[c + j + 16][k]     = (d2 & 0xf0) | (d3 >> 4);
                font[c + j][k + 1]      = (d0 << 4) | (d1 & 0x0f);
                font[c + j + 16][k + 1] = (d2 << 4) | (d3 & 0x0f);
            }
        }
    }
    memcpy(font, symbol, sizeof(symbol));
    memcpy(font[0xb], font[0x60], 8);

    vm->regist_vsync_event(this);
}

void VDP::event_vsync(int v, int clock)
{
    if (v == 239)
        dev->write_signal(SIG_UPD7801_INTF2, 0xffffffff, 1);
    else if (v == 261)
        dev->write_signal(SIG_UPD7801_INTF2, 0x00000000, 1);
}

void VDP::draw_screen()
{
    vdc0 = vram1[0x400];
    vdc1 = vram1[0x401];
    vdc2 = vram1[0x402];
    vdc3 = vram1[0x403];

    memset(text, vdc1 & 0xf, sizeof(text));
    draw_text_screen();

    memset(sprite, 0, sizeof(sprite));
    if (vdc0 & 0x10)
        draw_sprite_screen();

    int    ty   = ((vdc0 & 0xf7) == 0x17 && (vdc2 & 0xef) == 0x4f) ? 32 : 0;
    uint16 back = palette_pc[vdc1 & 0xf];

    for (int y = 0; y < ty; y++) {
        uint16_t *d = emu->screen_buffer(y);
        uint8    *t = &text[y + 23][24];

        for (int x = 0; x < SCREEN_WIDTH; x++)
            d[x] = palette_pc[t[x]];
    }
    for (int y = ty; y < SCREEN_HEIGHT; y++) {
        uint16_t *d = emu->screen_buffer(y);
        uint8    *s = &sprite[y + 21][28];
        uint8    *t = &text[y + 23][24];

        for (int x = 0; x < SCREEN_WIDTH; x++)
            d[x] = palette_pc[s[x] ? s[x] : t[x]];
    }
}

void VDP::draw_text_screen()
{
    int xmax = (vdc2 & 0xf) * 2;
    int ymax = vdc2 >> 4;
    int xs   = (vdc0 & 0x40) ? xmax : 0;
    int xe   = (vdc0 & 0x40) ? 32 : xmax;
    int ys   = (vdc0 & 0x80) ? ymax : 0;
    int ye   = (vdc0 & 0x80) ? 16 : ymax;

    uint8 ct = vdc3 >> 4;
    uint8 cb = vdc3 & 0xf;
    uint8 cg = vdc1 >> 4;

    for (int y = 1; y < 16; y++) {
        bool t   = (ys <= y && y < ye) ? true : false;
        int  y32 = y << 5;

        for (int x = 2; x < 29; x++) {
            if (t && (xs <= x && x < xe)) {
                uint8 data = (x < 26) ? (vram1[y32 + x] & 0x7f) : 0;
                draw_text(x, y, data, ct, cb);
            } else if ((vdc0 & 3) == 1) {
                uint8 data = vram1[y32 + x];
                draw_graph(x, y, data, cg);
            } else if ((vdc0 & 3) == 3) {
                uint8 data = vram1[y32 + x];
                draw_block(x, y, data);
            }
        }
    }
}

void VDP::draw_text(int dx, int dy, uint8 data, uint8 tcol, uint8 bcol)
{
    int dx8 = dx << 3, dy16 = dy << 4;

    for (int l = 0; l < 8 && data; l++) {
        uint8 *dest = &text[dy16 + l][dx8];
        uint8  pat  = font[data][l];

        dest[0] = (pat & 0x80) ? tcol : bcol;
        dest[1] = (pat & 0x40) ? tcol : bcol;
        dest[2] = (pat & 0x20) ? tcol : bcol;
        dest[3] = (pat & 0x10) ? tcol : bcol;
        dest[4] = (pat & 0x08) ? tcol : bcol;
        dest[5] = (pat & 0x04) ? tcol : bcol;
        dest[6] = (pat & 0x02) ? tcol : bcol;
        dest[7] = (pat & 0x01) ? tcol : bcol;
    }
    for (int l = (data ? 8 : 0); l < 16; l++)
        memset(&text[dy16 + l][dx8], bcol, 8);
}

void VDP::draw_block(int dx, int dy, uint8 data)
{
    int   dx8 = dx << 3, dy16 = dy << 4;
    uint8 cu = data >> 4, cl = data & 0xf;

    if (cu) {
        for (int l = 0; l < 8; l++)
            memset(&text[dy16 + l][dx8], cu, 8);
    }
    if (cl) {
        for (int l = 8; l < 16; l++)
            memset(&text[dy16 + l][dx8], cl, 8);
    }
}

void VDP::draw_graph(int dx, int dy, uint8 data, uint8 col)
{
    int dx8l = dx << 3, dx8r = (dx << 3) + 4, dy16 = dy << 4;

    if (data & 0x80) {
        for (int l = 0; l < 4; l++)
            memset(&text[dy16 + l][dx8l], col, 4);
    }
    if (data & 0x40) {
        for (int l = 0; l < 4; l++)
            memset(&text[dy16 + l][dx8r], col, 4);
    }
    if (data & 0x20) {
        for (int l = 4; l < 8; l++)
            memset(&text[dy16 + l][dx8l], col, 4);
    }
    if (data & 0x10) {
        for (int l = 4; l < 8; l++)
            memset(&text[dy16 + l][dx8r], col, 4);
    }
    if (data & 0x08) {
        for (int l = 8; l < 12; l++)
            memset(&text[dy16 + l][dx8l], col, 4);
    }
    if (data & 0x04) {
        for (int l = 8; l < 12; l++)
            memset(&text[dy16 + l][dx8r], col, 4);
    }
    if (data & 0x02) {
        for (int l = 12; l < 16; l++)
            memset(&text[dy16 + l][dx8l], col, 4);
    }
    if (data & 0x01) {
        for (int l = 12; l < 16; l++)
            memset(&text[dy16 + l][dx8r], col, 4);
    }
}

void VDP::draw_sprite_screen()
{
    for (int index = 0; index < 128; index++) {
        uint8 atb0 = vram1[0x200 + (index << 2)];
        uint8 atb1 = vram1[0x201 + (index << 2)];
        uint8 atb2 = vram1[0x202 + (index << 2)];
        uint8 atb3 = vram1[0x203 + (index << 2)];

        int   dx   = atb2 & 0xfe;
        int   dy   = atb0 & 0xfe;
        bool  conx = (atb2 & 1) ? true : false;
        bool  cony = (atb0 & 1) ? true : false;
        uint8 col0 = atb1 & 0xf;

        int sx = 0, ex = 4;
        int sy = atb1 >> 4, ey = 8;
        if (atb3 & 0x80) {
            if (atb3 & 0x40) {
                sy   = !cony ? 0 : 4;
                ey   = !cony ? 4 : 8;
                dy   = !cony ? dy : dy - 8;
                cony = false;
            }
            sx   = !conx ? 0 : 2;
            ex   = !conx ? 2 : 4;
            dx   = !conx ? dx : dx - 8;
            conx = false;

            atb3 &= 0x7f;
        }

        if ((index & 0x20) && (vdc0 & 0x20)) {
            uint8 col1 = (index & 0x40) ? color_pair1[col0] : color_pair0[col0];
            int   no1 = atb3, no2 = atb3 ^ ((conx ? 8 : 0) | (cony ? 1 : 0));

            draw_sprite(dx, dy, sx, ex, sy, ey, no1, col0);
            if (conx || cony)
                draw_sprite(dx, dy, sx, ex, sy, ey, no2, col1);
        } else {
            int no1 = atb3, no2 = atb3 | 1, no3 = atb3 | 8, no4 = atb3 | 9;

            draw_sprite(dx, dy, sx, ex, sy, ey, no1, col0);
            if (cony)
                draw_sprite(dx, dy + 16, sx, ex, sy - 8, 8, no2, col0);
            if (conx)
                draw_sprite(dx + 16, dy, 0, 4, sy, ey, no3, col0);
            if (conx && cony)
                draw_sprite(dx + 16, dy + 16, 0, 4, sy - 8, 8, no4, col0);
        }
    }
}

void VDP::draw_sprite(int dx, int dy, int sx, int ex, int sy, int ey, int no, uint8 col)
{
    if (!col)
        return;

    for (int y = (sy < 0 ? 0 : sy), no32 = no << 5; y < ey; y++) {
        int y2u = (y << 1) + dy, y2l = (y << 1) + dy + 1, y4 = (y << 2) + no32;

        for (int x = sx; x < ex; x++) {
            int    x4 = dx + (x << 2);
            uint8 *du = &sprite[y2u][x4];
            uint8 *dl = &sprite[y2l][x4];
            uint8  p  = vram0[y4 + x];

            if (p & 0x80)
                du[0] = col;
            if (p & 0x40)
                du[1] = col;
            if (p & 0x20)
                du[2] = col;
            if (p & 0x10)
                du[3] = col;
            if (p & 0x08)
                dl[0] = col;
            if (p & 0x04)
                dl[1] = col;
            if (p & 0x02)
                dl[2] = col;
            if (p & 0x01)
                dl[3] = col;
        }
    }
}
