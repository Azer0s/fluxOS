#ifndef __FLUX__INTERRUPTS_H
#define __FLUX__INTERRUPTS_H

#include <stdint.h>
#include <com/port.h>
#include <core/gdt.h>

class InterruptManager;

class InterruptHandler{
    protected:
        uint8_t interruptId;
        InterruptManager* interruptManager;

        InterruptHandler(uint8_t interruptId, InterruptManager* interruptManager);
        ~InterruptHandler();

    public:
        virtual uint32_t handle(uint32_t esp);
};

class InterruptManager{
    friend class InterruptHandler;

    protected:
        struct GateDescriptor{
            uint16_t handlerAddress_lo;
            uint16_t gtd_codeOffset;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAddress_hi;
        } __attribute__((packed));
        struct InterruptDescriptorTablePointer{
            uint16_t size;
            uint32_t base;
        } __attribute__((packed));

        static InterruptManager* instance;
        static GateDescriptor interruptDescriptorTable[256];
        static void setInterruptDescriptorTableEntry(uint8_t interruptId, uint16_t gtd_codeOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType);

        Port8bSlow picMasterCommand;
        Port8bSlow picMasterData;
        Port8bSlow picSlaveCommand;
        Port8bSlow picSlaveData;

        InterruptHandler* handlers[256];

    public:

        InterruptManager(GlobalDescriptorTable* gdt);
        ~InterruptManager();

        void enable();
        void disable();
        
        static uint32_t handle(uint8_t interruptId, uint32_t esp /*current stack pointer*/);
        uint32_t doHandle(uint8_t interruptId, uint32_t esp /*current stack pointer*/);

        static void ignoreInterruptRequest();
        static void handleInterruptRequest0x00();
        static void handleInterruptRequest0x01();
        static void handleInterruptRequest0x0C();
};

#endif