
void printf(char* str)
{
    // Woooh! This is our first OS feature we implement: printing!
    // 0xb8000 is the x86 register for VRAM. When we shift bits into it,
    // It will try to show said bits on the screen (no matter if these are
    // ASCII or binary)
    static unsigned short* vbuffer = (unsigned short*)0xb8000;

    for(int i = 0; str[i] != '\0'; ++i)
        vbuffer[i] = (vbuffer[i] & 0xFF00) | str[i];
}

// We want to use an object oriented language on bare metal
// The problem we're facing is that the OOP part of C++ is
// Tied to it's STDLIB (instantiation with malloc, deconstruction with free)
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

extern "C" void __main__(const void* multiboot_struct, unsigned int /*multiboot magic*/){
    printf("Hello world!");
    
    while(1);
}