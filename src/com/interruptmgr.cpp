#include <com/interruptmgr.h>

void printf(char* str);

#pragma region InterruptHandler

InterruptHandler::InterruptHandler(uint8_t interruptId, InterruptManager* interruptManager){
    this->interruptId = interruptId;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptId] = this;
}

InterruptHandler::~InterruptHandler(){
    if(interruptManager->handlers[interruptId] == this){
        interruptManager->handlers[interruptId] = 0x0;
    }
}

uint32_t InterruptHandler::handle(uint32_t esp){
    return esp;
}

#pragma endregion InterruptHandler

#pragma region InterruptManager

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::instance = 0x0;

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) : picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1){
    uint16_t codeSegment = gdt->getCodeSegmentOffset();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint16_t i = 0; i < 256; i++){
        handlers[i] = 0x0;
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
    if(instance != 0x0){
        instance->disable();
    }
    instance = this;
    asm("sti");
}

void InterruptManager::disable(){
    if(instance == this){
        instance = 0x0;
        asm("cli");
    }   
}

uint32_t InterruptManager::handle(uint8_t interruptId, uint32_t esp){
    if(instance != 0x0){
        return instance->doHandle(interruptId, esp);
    }

    return esp;
}

uint32_t InterruptManager::doHandle(uint8_t interruptId, uint32_t esp){

    if(handlers[interruptId] != 0x0){
        esp = handlers[interruptId]->handle(esp);
    }else if(interruptId != 0x20){
        char* hex = "0123456789ABCDEF";
        char* msg = "UNHANDLED INTERRUPT 0x00";

        msg[22] = hex[(interruptId >> 4) & 0x0F];
        msg[23] = hex[interruptId & 0x0F];

        printf(msg);
    }

    if(0x20 <= interruptId && interruptId <= 0x30){
        picMasterCommand.write(0x20);

        if(0x28 <= interruptId){
            picSlaveCommand.write(0x20);
        }
    }

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

#pragma endregion InterruptManager