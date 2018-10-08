#ifndef _FLUX_CONSOLE_H
#define _FLUX_CONSOLE_H

#include <stdint.h>

//Outputs a single deletable char to the console
void _flux_putChar(char);
//Outputs a single non-deletable char to the console
void _flux_putFixedChar(char);
//Outputs a deletable string to the console
void _flux_putString(char*);
//Outputs a non-deletable string to the console
void _flux_putFixedString(char*);

//Reads a single char from the console
void _flux_readChar(char*);
//Reads a string from the console
void _flux_readString(char*);
//Reads a fixed size string from the console
void _flux_readStringWithSize(char*, int);

//Clears the screen
void _flux_cls();
//Moves to a new line
void _flux_nl();
//Deletes a single character
void _flux_del();

//Sets the cursor to a position x y
void _flux_setCursor(int,int);
//Moves the cursor to the left
void _flux_mvCtL();
//Moves the cursor to the right
void _flux_mvCtR();

#endif