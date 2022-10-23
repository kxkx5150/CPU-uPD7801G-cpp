#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "utils/vm.h"
#include "emu/emu.h"
#include "fileio.h"
#include "utils/windef.h"

#define MAX_OUTPUT     8
#define SIG_CPU_DO_INT 0
#define SIG_CPU_DO_NMI 1


class DEVICE {
  protected:
    VM  *vm;
    EMU *emu;

  public:
    DEVICE(VM *parent_vm, EMU *parent_emu) : vm(parent_vm), emu(parent_emu)
    {
        prev_device = vm->last_device;
        next_device = NULL;
        if (!vm->first_device) {
            vm->first_device = this;
            this_device_id   = 0;
        } else {
            vm->last_device->next_device = this;
            this_device_id               = vm->last_device->this_device_id + 1;
        }
        vm->last_device = this;
    }
    ~DEVICE(void)
    {
    }

    virtual void initialize()
    {
    }
    virtual void release()
    {
    }

    virtual void update_config()
    {
    }
    virtual void save_state(FILEIO *fio)
    {
    }
    virtual void load_state(FILEIO *fio)
    {
    }

    virtual void reset()
    {
    }
    virtual void soft_reset()
    {
        reset();
    }
    virtual void ipl_reset()
    {
        reset();
    }

    virtual void write_data8(uint32 addr, uint32 data)
    {
    }
    virtual uint32 read_data8(uint32 addr)
    {
        return 0xff;
    }
    virtual void write_data16(uint32 addr, uint32 data)
    {
        write_data8(addr, data & 0xff);
        write_data8(addr + 1, data >> 8);
    }
    virtual uint32 read_data16(uint32 addr)
    {
        return read_data8(addr) | (read_data8(addr + 1) << 8);
    }

    virtual void write_io8(uint32 addr, uint32 data)
    {
    }
    virtual uint32 read_io8(uint32 addr)
    {
        return 0xff;
    }

    virtual void write_signal(int id, uint32 data, uint32 mask)
    {
    }
    virtual uint32 read_signal(int ch)
    {
        return 0;
    }

    virtual void request_int(int pri, uint32 vector, bool pending)
    {
    }
    virtual void cancel_int(int pri)
    {
    }

    virtual void do_reti()
    {
    }
    virtual void do_ei()
    {
    }
    virtual bool accept_int()
    {
        return false;
    }

    virtual void run(int clock)
    {
    }
    virtual uint32 get_prv_pc()
    {
        return 0;
    }

    virtual void mix(int32 *buffer, int cnt)
    {
    }

    virtual void event_callback(int event_id)
    {
    }
    virtual void event_frame()
    {
    }
    virtual void event_vsync(int v, int clock)
    {
    }
    virtual void event_hsync(int v, int h, int clock)
    {
    }

    DEVICE *prev_device;
    DEVICE *next_device;
    int     this_device_id;
};

#endif
