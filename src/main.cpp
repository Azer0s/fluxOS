#include <stdint.h>
#include <core/gdt.h>
#include <com/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

extern "C"{
    #include <stdio.h>
    #include <string.h>
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
    printf("\n\n .o88o. oooo                            .oooooo.    .oooooo..o\n"); 
    printf(" 888 `  `888                           d8P'  `Y8b  d8P'    `Y8\n"); 
    printf("o888oo   888  oooo  oooo  oooo    ooo 888      888 Y88bo.     \n"); 
    printf(" 888     888  `888  `888   `88b..8P'  888      888  ` Y8888o. \n"); 
    printf(" 888     888   888   888     Y888'    888      888      ` Y88b\n"); 
    printf(" 888     888   888   888   .o8 '88b   `88b    d88' oo     .d8P\n"); 
    printf("o888o   o888o  `V88V V8P' o88'   888o  `Y8bood8P'  8oo8888888P\n\n\n");

    GlobalDescriptorTable gdt;

    #pragma region Drivers
    InterruptManager interrupts(&gdt);

    //Activate keyboard driver
    KeyboardLayout layout("de-DE");
    KeyboardDriver keyboard(&interrupts,&layout);

    //Activate mouse driver
    #ifndef NO_MOUSE

    MouseDriver mouse(&interrupts);
    
    #endif

    interrupts.enable();

    #pragma endregion Drivers

    printf("\nflux>");

    while(1);
}
