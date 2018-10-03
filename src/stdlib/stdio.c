#include <stdio.h>
#include <stdint.h>

// Woooh! This is our first OS feature we implement: printing!
// 0xb8000 is the x86 register for Text-Screen-VRAM.
uint16_t* vbuffer = (uint16_t*)0xb8000;
uint8_t x = 0, y = 0; //cursor
int init = 0;
int lines = 0;

void newLine(){
    lines++;
    setCursor(0, y+1);
}

void clear(){
    for(y = 0; y < 25; y++){
        for(x = 0; x < 80; x++){
            vbuffer[80*y+x] = (vbuffer[80*y+x] & 0xFF00) | ' ';
        }
    }

    init = 0;
    setCursor(0, 0);
}

void printf(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i){

        switch(str[i]){
            case '\n':
                newLine();
                lines = 0;
                break;
            
            case '\t':
                setCursor(x+4, y);
                break;

            default:
                vbuffer[80*y+x] = (vbuffer[80*y+x] & 0xFF00) | str[i];
                setCursor(x+1, y);
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
    if(x != 5){
        setCursor(x-1, y);
        printf(" ");
        setCursor(x-1, y);
    }
}

void setCursor(int new_x, int new_y){
    if(init != 0){
        vbuffer[80*y+x] = ((vbuffer[80*y+x] & 0xF000) >> 4)
                        | ((vbuffer[80*y+x] & 0x0F00) << 4)
                        | (vbuffer[80*y+x] & 0x00FF);
    }else{
        init = 1;
    }

    x = new_x;
    y = new_y;

    vbuffer[80*y+x] = ((vbuffer[80*y+x] & 0xF000) >> 4)
                        | ((vbuffer[80*y+x] & 0x0F00) << 4)
                        | (vbuffer[80*y+x] & 0x00FF);
}

void moveCursorLeft(int distance_left){
    if(x == 0 && y > 0){
        if(lines > 0){
            setCursor(80,y-1);
        }
    }

    if((x-1) >= distance_left){
        setCursor(x-1, y);
    }
}

void moveCursorRight(int distance_right){
    if((x+1) <= (80-distance_right)){
        if((x+1) <= 79){
            setCursor(x+1,y);
        }
    }
}

int getLines(){
    return lines;
}