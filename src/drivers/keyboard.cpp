#include <drivers/keyboard.h>

extern "C"{
    #include <stdio.h>
    #include <string.h>
}

#pragma region KeyboardLayout

KeyboardLayout::KeyboardLayout(char* countrycode){
    loadLayout(countrycode);
}

KeyboardLayout::~KeyboardLayout(){}

void KeyboardLayout::loadLayout(char* countrycode){
    char* de_DE = "de-DE";
    if(strcmp(countrycode, de_DE) == 0){
        printf("Countrycode set to de-DE\n");

        delKey = 0x0E;
        shiftKey = 0x2A;

        keys[0xFA] = "";
        keys[0xC5] = "";
        keys[0x45] = "";
        keys[0x1C] = "\n";

        keys[0x2] = "1";
        keys[0x3] = "2";
        keys[0x4] = "3";
        keys[0x5] = "4";
        keys[0x6] = "5";
        keys[0x7] = "6";
        keys[0x8] = "7";
        keys[0x9] = "8";
        keys[0x0A] = "9";
        keys[0x0B] = "0";

        keys[0x0C] = "ß";
        keys[0x0D] = "´";
        keys[0x33] = ",";
        keys[0x34] = ".";
        keys[0x35] = "-";
        keys[0x1B] = "+";
        keys[0x2B] = "#";

        keys[0x10] = "q";
        keys[0x11] = "w";
        keys[0x12] = "e";
        keys[0x13] = "r";
        keys[0x14] = "t";
        keys[0x15] = "z";
        keys[0x16] = "u";
        keys[0x17] = "i";
        keys[0x18] = "o";
        keys[0x19] = "p";
        keys[0x1E] = "a";
        keys[0x1F] = "s";
        keys[0x20] = "d";
        keys[0x21] = "f";
        keys[0x22] = "g";
        keys[0x23] = "h";
        keys[0x24] = "j";
        keys[0x25] = "k";
        keys[0x26] = "l";
        keys[0x2C] = "y";
        keys[0x2D] = "x";
        keys[0x2E] = "c";
        keys[0x2F] = "v";
        keys[0x30] = "b";
        keys[0x31] = "n";
        keys[0x32] = "m";
    }
}

char* KeyboardLayout::getChar(uint8_t code){
    if(code < KEYBOARD_KEYS){
        if(keys[code] != 0x0){
            return keys[code];
        }
    }
    return "\0";
}

#pragma endregion KeyboardLayout

#pragma region KeyboardDriver

KeyboardDriver::KeyboardDriver(InterruptManager* InterruptManager, KeyboardLayout* layout) : InterruptHandler(0x21, InterruptManager), dataport(0x60), commandport(0x64){
    while(commandport.read() & 0x1){
        dataport.read();
    }

    this->layout = layout;

    commandport.write(0xAE);
    commandport.write(0x20);

    uint8_t status = (dataport.read() | 1) & ~0x10;
    commandport.write(0x60);
    dataport.write(status);

    dataport.write(0xF4);
}

KeyboardDriver::~KeyboardDriver(){}

uint32_t KeyboardDriver::handle(uint32_t esp){
    uint8_t key = dataport.read();

    if(key == layout->delKey){
        del();
    }else if(key == layout->shiftKey){
        this->shift = true;
    }else if((key == 0xAA) && (this->shift == true)){
        this->shift = false;
    }else if(key < 0x80){
        char* keyChar = layout->getChar(key);

        if(this->shift == true){
            //TODO: Handle shift
            printf(" shift+");
            printf(keyChar);
            printf(" ");
        }else{
            printf(keyChar);
        }  
    }

    return esp;
}

#pragma endregion KeyboardDriver