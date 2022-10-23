#ifndef _IO_H_
#define _IO_H_

#include "utils/vm.h"
#include "emu/emu.h"
#include "device.h"


class IO : public DEVICE {
  private:
    DEVICE *memory;
    DEVICE *sound;

    uint8 *key;
    uint8 *joy;
    uint8  pa, pb, pc, si, so;

  public:
    IO(VM *parent_vm, EMU *parent_emu) : DEVICE(parent_vm, parent_emu)
    {
    }
    ~IO()
    {
    }

    void   initialize();
    void   reset();
    void   write_io8(uint32 addr, uint32 data);
    uint32 read_io8(uint32 addr);

    void set_context_mem(DEVICE *device)
    {
        memory = device;
    }
    void set_context_sound(DEVICE *device)
    {
        sound = device;
    }
};

#endif
