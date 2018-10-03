#ifndef __FLUX__STDIO_H
#define __FLUX__STDIO_H

void printf(char*);
void clear();
void newLine();
void del();
void setCursor(int, int);
void moveCursorLeft(int);
void moveCursorRight(int);
int getLines();

#endif