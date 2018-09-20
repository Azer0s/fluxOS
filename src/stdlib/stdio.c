#include <stdio.h>
#include <stdint.h>

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

void del(){
    if(x != 0){
        x--;
        printf(" ");
        x--;
    }
}