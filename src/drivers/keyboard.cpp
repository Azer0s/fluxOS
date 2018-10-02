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

        keys[0xFA] = ""  ; shiftedKeys[0xFA] = ""  ; 
        keys[0xC5] = ""  ; shiftedKeys[0xC5] = ""  ; 
        keys[0x45] = ""  ; shiftedKeys[0x45] = ""  ; //TODO: Numlock
        keys[0x1C] = "\n"; shiftedKeys[0x1C] = "\n"; 

        keys[0x2]  = "1"; shiftedKeys[0x2]  = "!";
        keys[0x3]  = "2"; shiftedKeys[0x3]  = "\"";
        keys[0x4]  = "3"; shiftedKeys[0x4]  = ""; //TODO: Add support for §
        keys[0x5]  = "4"; shiftedKeys[0x5]  = "$";
        keys[0x6]  = "5"; shiftedKeys[0x6]  = "%";
        keys[0x7]  = "6"; shiftedKeys[0x7]  = "&";
        keys[0x8]  = "7"; shiftedKeys[0x8]  = "/";
        keys[0x9]  = "8"; shiftedKeys[0x9]  = "("; 
        keys[0x0A] = "9"; shiftedKeys[0x0A] = ")";
        keys[0x0B] = "0"; shiftedKeys[0x0B] = "=";

        keys[0x0C] = "ß"; shiftedKeys[0x0C] = "?";
        keys[0x0D] = "´"; shiftedKeys[0x0D] = "`";
        keys[0x33] = ","; shiftedKeys[0x33] = ";";
        keys[0x34] = "."; shiftedKeys[0x34] = ":";
        keys[0x35] = "-"; shiftedKeys[0x35] = "_";
        keys[0x1B] = "+"; shiftedKeys[0x1B] = "*";
        keys[0x2B] = "#"; shiftedKeys[0x2B] = "'";

        keys[0x10] = "q"; shiftedKeys[0x10] = "Q";
        keys[0x11] = "w"; shiftedKeys[0x11] = "W";
        keys[0x12] = "e"; shiftedKeys[0x12] = "E";
        keys[0x13] = "r"; shiftedKeys[0x13] = "R";
        keys[0x14] = "t"; shiftedKeys[0x14] = "T";
        keys[0x15] = "z"; shiftedKeys[0x15] = "Z";
        keys[0x16] = "u"; shiftedKeys[0x16] = "U";
        keys[0x17] = "i"; shiftedKeys[0x17] = "I";
        keys[0x18] = "o"; shiftedKeys[0x18] = "O";
        keys[0x19] = "p"; shiftedKeys[0x19] = "P";
        keys[0x1E] = "a"; shiftedKeys[0x1E] = "A";
        keys[0x1F] = "s"; shiftedKeys[0x1F] = "S";
        keys[0x20] = "d"; shiftedKeys[0x20] = "D";
        keys[0x21] = "f"; shiftedKeys[0x21] = "F";
        keys[0x22] = "g"; shiftedKeys[0x22] = "G";
        keys[0x23] = "h"; shiftedKeys[0x23] = "H";
        keys[0x24] = "j"; shiftedKeys[0x24] = "J";
        keys[0x25] = "k"; shiftedKeys[0x25] = "K";
        keys[0x26] = "l"; shiftedKeys[0x26] = "L";
        keys[0x2C] = "y"; shiftedKeys[0x2C] = "Y";
        keys[0x2D] = "x"; shiftedKeys[0x2D] = "X";
        keys[0x2E] = "c"; shiftedKeys[0x2E] = "C";
        keys[0x2F] = "v"; shiftedKeys[0x2F] = "V";
        keys[0x30] = "b"; shiftedKeys[0x30] = "B";
        keys[0x31] = "n"; shiftedKeys[0x31] = "N";
        keys[0x32] = "m"; shiftedKeys[0x32] = "M";
    }
}

char* KeyboardLayout::getChar(uint8_t code, bool shift){
    if(code < KEYBOARD_KEYS){
        if(shift == true){
            if(shiftedKeys[code] != 0x0){
                return shiftedKeys[code];
            }
        }else{
            if(keys[code] != 0x0){
                return keys[code];
            }
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
    }else if(key == 0xAA){ //keyup
        if(this->shift == true){
            this->shift = false;
        }
    }else if(key < 0x80){
        char* keyChar;
        if(this->shift == true){
            keyChar = layout->getChar(key, true);
        }else{
            keyChar = layout->getChar(key, false);
        } 

        if(keyChar != "\0"){
            printf(keyChar);
        }
    }

    return esp;
}

#pragma endregion KeyboardDriver