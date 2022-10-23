#include "scv.h"
#include "emu/emu.h"
#include "device.h"
#include "event.h"
#include "upd7801.h"
#include "io.h"
#include "memory.h"
#include "sound.h"
#include "vdp.h"


VM::VM(EMU *parent_emu) : emu(parent_emu)
{
    first_device = last_device = NULL;

    dummy  = new DEVICE(this, emu);
    event  = new EVENT(this, emu);
    cpu    = new UPD7801(this, emu);
    io     = new IO(this, emu);
    memory = new MEMORY(this, emu);
    sound  = new SOUND(this, emu);
    vdp    = new VDP(this, emu);

    event->initialize();
    event->set_context_cpu(cpu);
    event->set_context_sound(sound);

    cpu->set_context_mem(memory);
    cpu->set_context_io(io);

    io->set_context_mem(memory);
    io->set_context_sound(sound);
    memory->set_context(sound);
    sound->set_context(cpu);

    vdp->set_context(cpu);
    vdp->set_font_ptr(memory->get_font());
    vdp->set_vram_ptr(memory->get_vram());

    for (DEVICE *device = first_device; device; device = device->next_device) {
        if (device->this_device_id != event->this_device_id)
            device->initialize();
    }
    for (DEVICE *device = first_device; device; device = device->next_device) {
        if (device->this_device_id != event->this_device_id)
            device->reset();
    }
}
VM::~VM()
{
    for (DEVICE *device = first_device; device; device = device->next_device)
        device->release();
}
DEVICE *VM::get_device(int id)
{
    for (DEVICE *device = first_device; device; device = device->next_device) {
        if (device->this_device_id == id)
            return device;
    }
    return NULL;
}
void VM::reset()
{
    for (DEVICE *device = first_device; device; device = device->next_device)
        device->reset();
}
void VM::run()
{
    event->drive();
}
void VM::regist_event(DEVICE *dev, int event_id, int usec, bool loop, int *regist_id)
{
    event->regist_event(dev, event_id, usec, loop, regist_id);
}
void VM::regist_event_by_clock(DEVICE *dev, int event_id, int clock, bool loop, int *regist_id)
{
    event->regist_event_by_clock(dev, event_id, clock, loop, regist_id);
}
void VM::cancel_event(int regist_id)
{
    event->cancel_event(regist_id);
}
void VM::regist_frame_event(DEVICE *dev)
{
    event->regist_frame_event(dev);
}
void VM::regist_vsync_event(DEVICE *dev)
{
    event->regist_vsync_event(dev);
}
void VM::regist_hsync_event(DEVICE *dev)
{
    event->regist_hsync_event(dev);
}
void VM::draw_screen()
{
    vdp->draw_screen();
}
void VM::initialize_sound(int rate, int samples)
{
    event->initialize_sound(rate, samples);
    sound->init(rate);
}
uint16 *VM::create_sound(int samples, bool fill)
{
    return event->create_sound(samples, fill);
}
void VM::open_cart(char *filename)
{
    memory->open_cart(filename);
    reset();
}
void VM::close_cart()
{
    memory->close_cart();
    reset();
}
bool VM::now_skip()
{
    return false;
}
void VM::update_config()
{
    for (DEVICE *device = first_device; device; device = device->next_device)
        device->update_config();
}
