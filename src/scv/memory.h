#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <string.h>

#include "utils/vm.h"
#include "emu/emu.h"
#include "device.h"


class MEMORY : public DEVICE {
  private:
    DEVICE *dev;

    char save_path[MAX_PATH];

    typedef struct
    {
        char  id[4];
        uint8 ctype;
        uint8 dummy[11];
    } header_t;

    header_t header;

    uint8 *wbank[0x200];
    uint8 *rbank[0x200];
    uint8  bios[0x1000];
    uint8  vram[0x2000];
    uint8  wreg[0x80];
    uint8  cart[0x8000 * 4];
    uint8  sram[0x2000];
    uint8  wdmy[0x10000];
    uint8  rdmy[0x10000];

    void set_bank(uint8 bank);

  public:
    MEMORY(VM *parent_vm, EMU *parent_emu) : DEVICE(parent_vm, parent_emu)
    {
    }
    ~MEMORY()
    {
    }

    void   initialize();
    void   release();
    void   reset();
    void   write_data8(uint32 addr, uint32 data);
    uint32 read_data8(uint32 addr);
    void   write_io8(uint32 addr, uint32 data);

    void open_cart(char *filename);
    void close_cart();
    void set_context(DEVICE *device)
    {
        dev = device;
    }
    uint8 *get_font()
    {
        return bios + 0x200;
    }
    uint8 *get_vram()
    {
        return vram;
    }
};

#endif
