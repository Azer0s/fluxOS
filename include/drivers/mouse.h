#ifndef __FLUX__MOUSE_H
#define __FLUX__MOUSE_H

#include <stdint.h>
#include <com/interrupts.h>
#include <com/port.h>

class MouseDriver : public InterruptHandler{
    Port8b dataport;
    Port8b commandport;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttonState;

    public:
        MouseDriver(InterruptManager* InterruptManager);
        ~MouseDriver();

        virtual uint32_t handle(uint32_t esp);
};

#endif