#include <drivers/keyboard.h>

extern "C"{
    #include <stdio.h>
}

KeyboardDriver::KeyboardDriver(InterruptManager* InterruptManager) : InterruptHandler(0x21, InterruptManager), dataport(0x60), commandport(0x64){
    while(commandport.read() & 0x1){
        dataport.read();
    }

    commandport.write(0xAE);
    commandport.write(0x20);

    uint8_t status = (dataport.read() | 1) & ~0x10;
    commandport.write(0x60);
    dataport.write(status);

    dataport.write(0xF4);
}

KeyboardDriver::~KeyboardDriver(){

}

uint32_t KeyboardDriver::handle(uint32_t esp){
    uint8_t key = dataport.read();

    if(key < 0x80){
        switch(key){
            case 0xFA:
            case 0xC5:
            case 0x45:
                break;

            case 0x2: printf("1"); break;
            case 0x3: printf("2"); break;
            case 0x4: printf("3"); break;
            case 0x5: printf("4"); break;
            case 0x6: printf("5"); break;
            case 0x7: printf("6"); break;
            case 0x8: printf("7"); break;
            case 0x9: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;
            case 0x0E: del(); break;
            case 0x1C: printf("\n"); break;

            default:
                char* hex = "0123456789ABCDEF";
                char* msg = "KEYBOARD 0x00\n";

                msg[11] = hex[(key >> 4) & 0x0F];
                msg[12] = hex[key & 0x0F];
                printf(msg);
                break;
        }
    }

    return esp;
}