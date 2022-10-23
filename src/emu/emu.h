
#ifndef _EMU_H_
#define _EMU_H_
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#define DIRECTSOUND_VERSION 0x300
#define DIRECTDRAW_VERSION  0x500

#include <stdio.h>
#include "../utils/common.h"
#include "../utils/vm.h"
#include "../utils/windef.h"


class FILEIO;
class EMU {
  protected:
    VM *vm;

  private:
    HWND  main_window_handle;
    int   window_width, window_height;
    void  initialize_input();
    void  release_input();
    void  update_input();
    uint8 key_status[256];
    uint8 joy_status[2];
    int   mouse_status[3];
    bool  mouse_enable;

    int     joy_num;
    uint32  joy_xmin[2], joy_xmax[2];
    uint32  joy_ymin[2], joy_ymax[2];
    JOYCAPS joycaps[2];

    HDC      hdcDIB;
    HBITMAP  hBMP;
    LPBYTE   lpBuf;
    uint16_t lpBMP[SCREEN_WIDTH * SCREEN_HEIGHT];

    void initialize_sound(int rate, int samples);
    void release_sound();
    void update_sound();

    int   sound_rate, sound_samples;
    DWORD sound_buffer_size;
    bool  sound_ok, now_mute;
    bool  first_half;

    typedef struct
    {
        DWORD dwRIFF;
        DWORD dwFileSize;
        DWORD dwWAVE;
        DWORD dwfmt_;
        DWORD dwFormatSize;
        WORD  wFormatTag;
        WORD  wChannels;
        DWORD dwSamplesPerSec;
        DWORD dwAvgBytesPerSec;
        WORD  wBlockAlign;
        WORD  wBitsPerSample;
        DWORD dwdata;
        DWORD dwDataLength;
    } wavheader_t;

    FILEIO *rec;
    DWORD   rec_size;
    bool    now_rec;

    SYSTEMTIME sTime;

  public:
    EMU();
    ~EMU();

    void run();
    void run_cpu(uint64_t cycles);

    void reset();

    void open_cart(char *filename);
    void close_cart();

    bool now_skip();
    void start_rec_sound();
    void stop_rec_sound();
    bool now_rec_sound()
    {
        return now_rec;
    }
    void update_config();
    void key_down(int code);
    void key_up(int code);
    void enable_mouse();
    void disenable_mouse();
    void toggle_mouse();
    void set_screen_size(int width, int height);

    void draw_screen(SDL_Renderer *renderer, SDL_Texture *tex);
    void mute_sound();

    uint8 *key_buffer()
    {
        return key_status;
    }
    uint8 *joy_buffer()
    {
        return joy_status;
    }
    int *mouse_buffer()
    {
        return mouse_status;
    }
    uint16_t *screen_buffer(int y)
    {
        return &lpBMP[SCREEN_WIDTH * (y)];
    }

    void update_timer();
    void get_timer(int time[]);
};
#endif
