
#ifndef _SOUND_H_
#define _SOUND_H_

#include "utils/vm.h"
#include "emu/emu.h"
#include "device.h"

#define SOUND_CLOCK  1522400.0
#define NOISE0_CLOCK 760.0
#define NOISE1_CLOCK 174000.0
#define PCM_PERIOD   170000

#define MAX_VOLUME 24000
#define MAX_NOISE  16000
#define MAX_PCM    20000
#define MAX_PARAM  0x8000


class SOUND : public DEVICE {
  private:
    DEVICE *dev;

    typedef struct
    {
        int count;
        int diff;
        int period;
        int timbre;
        int volume;
        int output;
        int ptr;
    } channel_t;

    channel_t tone;
    channel_t noise0;
    channel_t noise1;
    channel_t pcm;

    void clear_channel(channel_t *ch);

    uint32 cmd_addr;
    int    pcm_len;
    bool   noise1_enb;

    int pcm_table[MAX_PARAM * 8];
    int volume_table[32];
    int detune_table[32];
    int noise1_table[129][256];

    int   param_cnt, param_ptr, regist_id;
    uint8 params[MAX_PARAM];

    void process_pcm(uint8 data);
    void process_cmd();

  public:
    SOUND(VM *parent_vm, EMU *parent_emu) : DEVICE(parent_vm, parent_emu)
    {
    }
    ~SOUND()
    {
    }

    void reset();
    void write_data8(uint32 addr, uint32 data);
    void write_io8(uint32 addr, uint32 data);
    void event_callback(int event_id);
    void mix(int32 *buffer, int cnt);

    void set_context(DEVICE *device)
    {
        dev = device;
    }
    void init(int rate);
};

#endif
