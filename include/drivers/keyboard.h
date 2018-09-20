#ifndef __FLUX__KEYBOARD_H
#define __FLUX__KEYBOARD_H

#include <stdint.h>
#include <com/interrupts.h>
#include <com/port.h>

class KeyboardDriver : public InterruptHandler{
    Port8b dataport;
    Port8b commandport;

    public:
        KeyboardDriver(InterruptManager* InterruptManager);
        ~KeyboardDriver();

        virtual uint32_t handle(uint32_t esp);
};

#endif