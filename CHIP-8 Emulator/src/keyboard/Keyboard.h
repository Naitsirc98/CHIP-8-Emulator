#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>


void setKeyPressed(unsigned int key);

void setKeyReleased(unsigned int key);

uint8_t getKey(unsigned int key);

int getKeyPressed();


#endif
