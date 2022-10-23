
#ifndef _SCV_H_
#define _SCV_H_
#include "utils/common.h"
#include "utils/windef.h"


#define DEVICE_NAME "EPOCH SCV"
#define CONFIG_NAME "scv"

#define WINDOW_WIDTH1  192
#define WINDOW_HEIGHT1 222
#define WINDOW_WIDTH2  576
#define WINDOW_HEIGHT2 444

#define USE_CART
#define USE_JOYKEY
#define USE_SCREEN_X2
#define DONT_KEEP_ASPECT

#define FRAMES_PER_10SECS 600
#define FRAMES_PER_SEC    60
#define LINES_PER_FRAME   262
#define CHARS_PER_LINE    1
#define CPU_CLOCKS        4000000
#define SCREEN_WIDTH      192
#define SCREEN_HEIGHT     222


class EMU;
class DEVICE;
class EVENT;

class UPD7801;

class IO;
class MEMORY;
class SOUND;
class VDP;

class VM {
  protected:
    EMU *emu;

    DEVICE *dummy;
    EVENT  *event;

    UPD7801 *cpu;

    IO     *io;
    MEMORY *memory;
    SOUND  *sound;
    VDP    *vdp;

  public:
    VM(EMU *parent_emu);
    ~VM();


    void reset();
    void run();

    void draw_screen();

    void    initialize_sound(int rate, int samples);
    uint16 *create_sound(int samples, bool fill);

    void open_cart(char *filename);
    void close_cart();
    bool now_skip();

    void update_config();

    void regist_event(DEVICE *device, int event_id, int usec, bool loop, int *regist_id);
    void regist_event_by_clock(DEVICE *device, int event_id, int clock, bool loop, int *regist_id);
    void cancel_event(int regist_id);
    void regist_frame_event(DEVICE *dev);
    void regist_vsync_event(DEVICE *dev);
    void regist_hsync_event(DEVICE *dev);

    DEVICE *get_device(int id);
    DEVICE *first_device;
    DEVICE *last_device;
};

#endif
