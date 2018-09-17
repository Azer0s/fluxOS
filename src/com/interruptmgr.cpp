#include <com/interruptmgr.h>

void printf(char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) : picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1){
    uint16_t codeSegment = gdt->getCodeSegmentOffset();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint16_t i = 0; i < 256; i++){
        setInterruptDescriptorTableEntry(i, codeSegment, &ignoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    setInterruptDescriptorTableEntry(0x20, codeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, codeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.write(0x11);
    picSlaveCommand.write(0x11);

    picMasterData.write(0x20);
    picSlaveData.write(0x28);

    picMasterData.write(0x04);
    picSlaveData.write(0x02);

    picMasterData.write(0x01);
    picSlaveData.write(0x01);

    picMasterData.write(0x00);
    picSlaveData.write(0x08);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager(){}

void InterruptManager::enable(){
    asm("sti");
}

uint32_t InterruptManager::handle(uint8_t interruptId, uint32_t esp){
    printf("INTERRUPT\n");

    return esp;
}

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interruptId, uint16_t gtd_codeOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType){
    interruptDescriptorTable[interruptId].handlerAddress_lo = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptId].handlerAddress_hi = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptId].gtd_codeOffset = gtd_codeOffset;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptId].access = IDT_DESC_PRESENT |descriptorType | ((descriptorPrivilegeLevel & 3) << 5);
    interruptDescriptorTable[interruptId].reserved = 0;
}