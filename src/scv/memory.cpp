#include "memory.h"
#include "fileio.h"
#include <string>
#include "dat.h"


#define SET_BANK(s, e, w, r)                                                                                           \
    {                                                                                                                  \
        int sb = (s) >> 7, eb = (e) >> 7;                                                                              \
        for (int i = sb; i <= eb; i++) {                                                                               \
            wbank[i] = (w) + 0x80 * (i - sb);                                                                          \
            rbank[i] = (r) + 0x80 * (i - sb);                                                                          \
        }                                                                                                              \
    }

void MEMORY::initialize()
{
    memset(bios, 0xff, sizeof(bios));
    memset(cart, 0xff, sizeof(cart));
    memset(sram, 0xff, sizeof(sram));
    memset(rdmy, 0xff, sizeof(rdmy));

    for (int i = 0; i < 8192; ++i)
        bios[i] = rom_dat[i];


    set_bank(0);
    memset(&header, 0, sizeof(header_t));
}

void MEMORY::release()
{
    close_cart();
}

void MEMORY::reset()
{
    memset(vram, 0, sizeof(vram));
    for (int i = 0x1000; i < 0x1200; i += 32) {
        static uint8 tmp[32] = {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        memcpy(vram + i, tmp, 32);
    }
    memset(wreg, 0, sizeof(wreg));

    set_bank(0);
}

void MEMORY::write_data8(uint32 addr, uint32 data)
{
    if (addr == 0x3600)
        dev->write_data8(addr, data);
    if ((addr & 0xfe00) == 0x3400)
        wbank[0x68][addr & 3] = data;
    else
        wbank[addr >> 7][addr & 0x7f] = data;
}

uint32 MEMORY::read_data8(uint32 addr)
{
    return rbank[addr >> 7][addr & 0x7f];
}

void MEMORY::write_io8(uint32 addr, uint32 data)
{
    set_bank((data >> 5) & 3);
}

void MEMORY::set_bank(uint8 bank)
{
    SET_BANK(0x0000, 0x0fff, wdmy, bios);
    SET_BANK(0x1000, 0x1fff, wdmy, rdmy);
    SET_BANK(0x2000, 0x3fff, vram, vram);
    SET_BANK(0x4000, 0x7fff, wdmy, rdmy);
    SET_BANK(0x8000, 0xff7f, wdmy, cart + 0x8000 * bank);
    if (header.ctype == 1 && (bank & 1)) {
        SET_BANK(0xe000, 0xff7f, sram, sram);
    }
    SET_BANK(0xff80, 0xffff, wreg, wreg);
}

void MEMORY::open_cart(char *filename)
{
    close_cart();

    _tcscpy(save_path, filename);
    int len = _tcslen(save_path);
    if (save_path[len - 4] == ('.')) {
        save_path[len - 3] = ('S');
        save_path[len - 2] = ('A');
        save_path[len - 1] = ('V');
    } else
        _stprintf(save_path, "%s.SAV", filename);

    FILEIO *fio = new FILEIO();

    if (fio->Fopen(filename, FILEIO_READ_BINARY)) {
        fio->Fread(&header, sizeof(header_t), 1);
        if (!(header.id[0] == 'S' && header.id[1] == 'C' && header.id[2] == 'V' && header.id[3] == 0x1a)) {
            memset(&header, 0, sizeof(header_t));
            fio->Fseek(0, FILEIO_SEEK_SET);
        }

        fio->Fread(cart, 0x8000, 1);
        memcpy(cart + 0x8000, cart, 0x8000);

        if (header.ctype == 0)
            fio->Fread(cart + 0xe000, 0x2000, 1);
        else if (header.ctype == 2)
            fio->Fread(cart + 0x8000, 0x8000, 1);
        memcpy(cart + 0x10000, cart, 0x10000);

        if (header.ctype == 2)
            fio->Fread(cart + 0x10000, 0x10000, 1);
        else if (header.ctype == 3) {
            fio->Fread(cart + 0x10000, 0x8000, 1);
            memcpy(cart + 0x18000, cart + 0x10000, 0x8000);
        }

        fio->Fclose();

        if (header.ctype == 1 && cart[1] != 0x48 && fio->Fopen(save_path, FILEIO_READ_BINARY)) {
            fio->Fread(sram, 0x2000, 1);
            fio->Fclose();
        }
    }
    delete fio;
}

void MEMORY::close_cart()
{
    if (header.ctype == 1 && cart[1] != 0x48) {
        FILEIO *fio = new FILEIO();
        if (fio->Fopen(save_path, FILEIO_WRITE_BINARY)) {
            fio->Fwrite(sram, 0x2000, 1);
            fio->Fclose();
        }
        delete fio;
    }

    memset(&header, 0, sizeof(header_t));
    memset(cart, 0xff, sizeof(cart));
    memset(sram, 0xff, sizeof(sram));
}
