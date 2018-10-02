#include <drivers/mouse.h>

MouseDriver::MouseDriver(InterruptManager* InterruptManager) : InterruptHandler(0x2C, InterruptManager), dataport(0x60), commandport(0x64){
    offset = 0;
    buttonState = 0;

    uint16_t* vbuffer = (uint16_t*)0xb8000;
    vbuffer[80*12+40] = ((vbuffer[80*12+40] & 0xF000) >> 4)
                        | ((vbuffer[80*12+40] & 0x0F00) << 4)
                        | (vbuffer[80*12+40] & 0x00FF);

    commandport.write(0xA8);
    commandport.write(0x20);

    uint8_t status = dataport.read() | 2;
    commandport.write(0x60);
    dataport.write(status);

    commandport.write(0xD4);
    dataport.write(0xF4);
    dataport.read();
}

MouseDriver::~MouseDriver(){}

uint32_t MouseDriver::handle(uint32_t esp){
    uint8_t status = commandport.read();
    
    if(!(status & 0x20)){
        return esp;
    }

    static int8_t x = 40, y = 12;

    buffer[offset] = dataport.read();
    offset = (offset + 1) % 3;

    if(offset == 0){
        uint16_t* vbuffer = (uint16_t*)0xb8000;

        vbuffer[80*y+x] = ((vbuffer[80*y+x] & 0xF000) >> 4)
                        | ((vbuffer[80*y+x] & 0x0F00) << 4)
                        | (vbuffer[80*y+x] & 0x00FF);

        x += buffer[1];
        if(x < 0){
            x = 0;
        }

        if(x >= 80){
            x = 79;
        }

        y -= buffer[2];
        if(y < 0){
            y = 0;
        }

        if(y >= 25){
            y = 24;
        }

        vbuffer[80*y+x] = ((vbuffer[80*y+x] & 0xF000) >> 4)
                        | ((vbuffer[80*y+x] & 0x0F00) << 4)
                        | (vbuffer[80*y+x] & 0x00FF);
        
        for(uint8_t i = 0; i < 3; i++){
            if((buffer[0] & (0x01 << i)) != (buttonState & (0x01 << i))){
                vbuffer[80*y+x] = ((vbuffer[80*y+x] & 0xF000) >> 4)
                        | ((vbuffer[80*y+x] & 0x0F00) << 4)
                        | (vbuffer[80*y+x] & 0x00FF);
            }
        }

        buttonState = buffer[0];
    }

    return esp;
}