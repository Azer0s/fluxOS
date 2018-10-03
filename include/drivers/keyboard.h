#ifndef __FLUX__KEYBOARD_H
#define __FLUX__KEYBOARD_H

#include <stdint.h>
#include <com/interrupts.h>
#include <com/port.h>

#define KEYBOARD_KEYS 256

class KeyboardLayout{
    char* keys[KEYBOARD_KEYS];
    char* shiftedKeys[KEYBOARD_KEYS];

    public:
        KeyboardLayout(char* layout);
        ~KeyboardLayout();
        void loadLayout(char* layout);
        char* getChar(uint8_t code, bool shift);

        #pragma region Special_Chars

        uint8_t delKey;
        uint8_t shiftKey;
        uint8_t space; 

        #pragma region Arrow_Keys

        uint8_t up;
        uint8_t down;
        uint8_t left;
        uint8_t right;

        #pragma endregion Arrow_Keys

        #pragma endregion Special_Chars
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