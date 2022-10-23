
#include "event.h"
#include "utils/config.h"

extern config_t config;



void EVENT::initialize()
{
    if (!(0 <= config.cpu_power && config.cpu_power <= 4))
        config.cpu_power = 0;

    power = config.cpu_power;

    int  sum    = (int)((float)CPU_CLOCKS / (float)FRAMES_PER_SEC + 0.5);
    int  remain = sum;
    int *tmp    = &hclocks[0][0];

    for (int i = 0; i < LINES_PER_FRAME * CHARS_PER_LINE; i++) {
        tmp[i] = (int)(sum / LINES_PER_FRAME / CHARS_PER_LINE);
        remain -= tmp[i];
    }

    for (int i = 0; i < remain; i++)
        tmp[(int)(LINES_PER_FRAME * CHARS_PER_LINE * i / remain)]++;

    for (int i = 0; i < LINES_PER_FRAME; i++) {
        vclocks[i] = 0;
        for (int j = 0; j < CHARS_PER_LINE; j++)
            vclocks[i] += hclocks[i][j];
    }

    for (int i = 0; i < MAX_EVENT; i++) {
        event[i].enable = false;
        event[i].device = NULL;
    }

    next = -1;
    past = event_cnt = frame_event_cnt = vsync_event_cnt = hsync_event_cnt = 0;
}

void EVENT::initialize_sound(int rate, int samples)
{
    sound_samples  = samples;
    update_samples = (int)(1024. * rate / FRAMES_PER_SEC / LINES_PER_FRAME + 0.5);

    sound_buffer = (uint16 *)malloc(samples * sizeof(uint16));
    memset(sound_buffer, 0, samples * sizeof(uint16));
    sound_tmp = (int32 *)malloc(samples * sizeof(int32));
    memset(sound_tmp, 0, samples * sizeof(int32));
    buffer_ptr = accum_samples = 0;
}

void EVENT::release()
{
    if (sound_buffer)
        free(sound_buffer);
    if (sound_tmp)
        free(sound_tmp);
}

void EVENT::reset()
{
    for (int i = 0; i < event_cnt; i++) {
        if (!(event[i].enable && event[i].loop)) {
            event[i].enable = false;
            event[i].device = NULL;
        }
    }

    next = -1;

    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable && (event[i].clock < next || next == -1))
            next = event[i].clock;
    }

    if (next == -1)
        past = 0;

    if (sound_buffer)
        memset(sound_buffer, 0, sound_samples * sizeof(uint16));

    if (sound_tmp)
        memset(sound_tmp, 0, sound_samples * sizeof(int32));

    buffer_ptr = 0;
}

void EVENT::drive()
{
    for (int i = 0; i < frame_event_cnt; i++)
        frame_event[i]->event_frame();

    if (hsync_event_cnt) {
        for (int v = 0; v < LINES_PER_FRAME; v++) {
            for (int i = 0; i < vsync_event_cnt; i++)
                vsync_event[i]->event_vsync(v, vclocks[v]);

            for (int h = 0; h < CHARS_PER_LINE; h++) {
                for (int i = 0; i < hsync_event_cnt; i++)
                    hsync_event[i]->event_hsync(v, h, hclocks[v][h]);

                update_event(hclocks[v][h]);
            }

            update_sound();
        }
    } else {
        for (int v = 0; v < LINES_PER_FRAME; v++) {
            for (int i = 0; i < vsync_event_cnt; i++)
                vsync_event[i]->event_vsync(v, vclocks[v]);
            update_event(vclocks[v]);
            update_sound();
        }
    }
}

void EVENT::update_event(int clock)
{
    while (clock) {
        if (clock < next || next == -1) {
            for (int i = 0; i < cpu_cnt; i++)
                cpu[i]->run(clock << power);

            if (next != -1) {
                next -= clock;
                past += clock;
            }
            return;
        }

        for (int i = 0; i < cpu_cnt; i++)
            cpu[i]->run(next << power);

        clock -= next;
        past += next;

        for (int i = 0; i < event_cnt; i++) {
            if (!event[i].enable)
                continue;

            event[i].clock -= past;
            while (event[i].clock <= 0) {
                event[i].device->event_callback(event[i].event_id);

                if (event[i].loop) {
                    event[i].clock += event[i].loop;
                } else {
                    event[i].device = NULL;
                    event[i].enable = false;
                    break;
                }
            }
        }

        past = 0;
        next = -1;
        for (int i = 0; i < event_cnt; i++) {
            if (event[i].enable && (event[i].clock < next || next == -1))
                next = event[i].clock;
        }
    }
}

void EVENT::update_sound()
{
    accum_samples += update_samples;
    int samples = accum_samples >> 10;
    accum_samples -= samples << 10;
    create_sound(samples, false);
}

void EVENT::regist_event(DEVICE *dev, int event_id, int usec, bool loop, int *regist_id)
{
    *regist_id = -1;

    bool no_exist = true;
    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable) {
            no_exist = false;
            break;
        }
    }
    if (no_exist)
        past = 0;

    for (int i = 0; i < MAX_EVENT; i++) {
        if (!event[i].enable) {
            if (event_cnt < i + 1)
                event_cnt = i + 1;
            int clock         = (int)(CPU_CLOCKS / 1000000. * usec + 0.5);
            event[i].enable   = true;
            event[i].device   = dev;
            event[i].event_id = event_id;
            event[i].clock    = clock + past;
            event[i].loop     = loop ? clock : 0;
            *regist_id        = i;
            break;
        }
    }

    next = -1;
    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable && (event[i].clock < next || next == -1))
            next = event[i].clock;
    }
    if (next == -1)
        past = 0;
}

void EVENT::regist_event_by_clock(DEVICE *dev, int event_id, int clock, bool loop, int *regist_id)
{
    *regist_id = -1;

    bool no_exist = true;
    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable) {
            no_exist = false;
            break;
        }
    }
    if (no_exist)
        past = 0;

    for (int i = 0; i < MAX_EVENT; i++) {
        if (!event[i].enable) {
            if (event_cnt < i + 1)
                event_cnt = i + 1;

            event[i].enable   = true;
            event[i].device   = dev;
            event[i].event_id = event_id;
            event[i].clock    = clock + past;
            event[i].loop     = loop ? clock : 0;
            *regist_id        = i;
            break;
        }
    }

    next = -1;
    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable && (event[i].clock < next || next == -1))
            next = event[i].clock;
    }
    if (next == -1)
        past = 0;
}

void EVENT::cancel_event(int regist_id)
{
    if (0 <= regist_id && regist_id < MAX_EVENT) {
        event[regist_id].device = NULL;
        event[regist_id].enable = false;
    }

    next = -1;
    for (int i = 0; i < event_cnt; i++) {
        if (event[i].enable && (event[i].clock < next || next == -1))
            next = event[i].clock;
    }
    if (next == -1)
        past = 0;
}

void EVENT::regist_frame_event(DEVICE *dev)
{
    frame_event[frame_event_cnt++] = dev;
}

void EVENT::regist_vsync_event(DEVICE *dev)
{
    vsync_event[vsync_event_cnt++] = dev;
}

void EVENT::regist_hsync_event(DEVICE *dev)
{
    hsync_event[hsync_event_cnt++] = dev;
}

uint16 *EVENT::create_sound(int samples, bool fill)
{
    int cnt = 0;
    if (fill)
        cnt = sound_samples - buffer_ptr;
    else
        cnt = (sound_samples - buffer_ptr < samples) ? sound_samples - buffer_ptr : samples;

    if (cnt) {
        memset(&sound_tmp[buffer_ptr], 0, cnt * sizeof(int32));
        for (int i = 0; i < sound_cnt; i++)
            sound[i]->mix(&sound_tmp[buffer_ptr], cnt);
    }

    if (fill) {
        for (int i = 0; i < sound_samples; i++) {
            int    dat     = sound_tmp[i];
            uint16 highlow = (uint16)(dat & 0x0000ffff);

            if ((dat > 0) && (highlow >= 0x8000)) {
                sound_buffer[i] = 0x7fff;
                continue;
            }
            if ((dat < 0) && (highlow < 0x8000)) {
                sound_buffer[i] = 0x8000;
                continue;
            }
            sound_buffer[i] = highlow;
        }
    }
    buffer_ptr = fill ? 0 : (buffer_ptr + cnt);
    return sound_buffer;
}

void EVENT::update_config()
{
    power = config.cpu_power;
}
