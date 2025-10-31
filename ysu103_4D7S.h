//DEMO 1 https://www.youtube.com/watch?v=2Reih4ck2CU
//DEMO 2 https://youtu.be/osiF3AG_7-I
#ifndef D7S_H
#define D7S_H
#include "spiAVR.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char nums[11] = {
    0b0111111, //0
    0b0000110, //1
    0b1011011, //2
    0b1001111, //3
    0b1100110, //4
    0b1101101, //5
    0b1111101, //6
    0b0000111, //7
    0b1111111, //8
    0b1100111, //9
    0b0000000 // off
};

void shiftRegister(char data) {
    PORTB &= 0b11111011; 
    SPI_SEND(data);           
    PORTB |= 0b00000100;
}
#endif