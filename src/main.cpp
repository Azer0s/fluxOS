#include <common/types.h>
#include <core/gdt.h>

// Woooh! This is our first OS feature we implement: printing!
// 0xb8000 is the x86 register for Text-Screen-VRAM.
uint16_t* vbuffer = (uint16_t*)0xb8000;
uint8_t x = 0, y = 0; //cursor

void newLine(){
    y++;
    x = 0;
}

void clear(){
    for(y = 0; y < 25; y++){
        for(x = 0; x < 80; x++){
            vbuffer[80*y+x] = (vbuffer[80*y+x] & 0xFF00) | ' ';
        }
    }

    x = 0;
    y = 0;
}

void printf(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i){

        switch(str[i]){
            case '\n':
                newLine();
                break;
            
            case '\t':
                x += 4;
                break;

            default:
                vbuffer[80*y+x] = (vbuffer[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80){
            newLine();
        }

        if(y >= 25){
            clear();
        }
    }
}

// We want to use an object oriented language on bare metal
// The problem we're facing is that the OOP part of C++ is
// tied to it's STDLIB (instantiation with malloc, deconstruction with free)
// So in order to use objects, we have to define what an object is
// For this, we'll just use a void pointer because that's basically what an object can be: anything
typedef void (*constructor)();

// Since we can't let the compiler figure out where everything is located,
// we need to give our kernel the starting address for our constructors
// This will be our global offset table
extern "C" constructor start_ctors;
// Our last constructor address
extern "C" constructor end_ctors;
extern "C" void ctor_init(){
    for (constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)(); // initialize our constructors
    }
}

extern "C" void __main__(const void* multiboot_struct, uint32_t /*multiboot magic*/){
    clear();
    printf("Hello\tworld!\n");
    printf("Hello\tworld!\n");

    GlobalDescriptorTable gdt;

    while(1);
}
