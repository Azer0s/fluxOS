#ifndef __FLUX__KEYBOARD_H
#define __FLUX__KEYBOARD_H

#include <stdint.h>
#include <com/interrupts.h>
#include <com/port.h>

#define KEYBOARD_KEYS 256

class KeyboardLayout{
    protected:
        char* keys[KEYBOARD_KEYS];

    public:
        KeyboardLayout(char* layout);
        ~KeyboardLayout();
        void loadLayout(char* layout);
        char* getChar(uint8_t code);
        uint8_t delKey;
        uint8_t shiftKey;
};

class KeyboardDriver : public InterruptHandler{
    Port8b dataport;
    Port8b commandport;
    KeyboardLayout* layout;
    bool shift;

    public:
        KeyboardDriver(InterruptManager* InterruptManager, KeyboardLayout* layout);
        ~KeyboardDriver();

        virtual uint32_t handle(uint32_t esp);
};

#endif