#include <com/port.h>

Port::Port(uint16_t portnr){
    this->portnr = portnr;
}

Port::~Port(){

}

#pragma region Port8b

Port8b::Port8b(uint16_t portnr) : Port(portnr) {}

Port8b::~Port8b(){}

void Port8b::write(uint8_t data){
    asm volatile("outb %0, %1" : : "a" (data), "Nd" (portnr));
}

uint8_t Port8b::read(){
    uint8_t result;

    asm volatile("inb %1, %0" : "=a" (result) : "Nd" (portnr));

    return result;
}

#pragma endregion Port8b

#pragma region Port8bSlow

Port8bSlow::Port8bSlow(uint16_t portnr) : Port8b(portnr) {}

Port8bSlow::~Port8bSlow(){}

void Port8bSlow::write(uint8_t data){
    asm volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnr));
}

#pragma endregion Port8bSlow

#pragma region Port16b

Port16b::Port16b(uint16_t portnr) : Port(portnr) {}

Port16b::~Port16b(){}

void Port16b::write(uint16_t data){
    asm volatile("outw %0, %1" : : "a" (data), "Nd" (portnr));
}

uint16_t Port16b::read(){
    uint16_t result;

    asm volatile("inw %1, %0" : "=a" (result) : "Nd" (portnr));

    return result;
}

#pragma endregion Port16b

#pragma region Port32b

Port32b::Port32b(uint16_t portnr) : Port(portnr) {}

Port32b::~Port32b(){}

void Port32b::write(uint32_t data){
    asm volatile("outl %0, %1" : : "a" (data), "Nd" (portnr));
}

uint32_t Port32b::read(){
    uint32_t result;

    asm volatile("inl %1, %0" : "=a" (result) : "Nd" (portnr));

    return result;
}

#pragma endregion Port32b