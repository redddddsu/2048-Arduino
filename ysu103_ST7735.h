//DEMO 1 https://www.youtube.com/watch?v=2Reih4ck2CU
//DEMO 2 https://youtu.be/osiF3AG_7-I

#ifndef ST7_H
#define ST7_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spiAVR.h"
#include "ysu103_tiles.h"

void reset() {
    PORTD &= 0xEF;
    _delay_ms(200);
    PORTD |= 0x10;
    _delay_ms(200);
}
void send_command(char cmd) {
    PORTD &= 0b11011111; // Set CS to zero
    PORTD &= 0b10111111; // Set A0 to command 
    SPI_SEND(cmd);
    PORTD |= 0b00100000; // Set CS to 1
}
void send_data(char data) {
    PORTD &= 0b11011111; // Set CS to zero
    PORTD |= 0b01000000; // Set A0 to data 
    SPI_SEND(data);
    PORTD |= 0b00100000; // Set CS to 1
}
void ST7735_init() {
    reset();
    send_command(0x01);
    _delay_ms(150);
    send_command(0x11);
    _delay_ms(200);
    send_command(0x3A);
    send_data(0x06);
    _delay_ms(10);
    send_command(0x29);
    _delay_ms(200);
}
void set_screen() {
    send_command(0x2A);
    send_data(0x00);
    send_data(0x00); 
    send_data(0x00);
    send_data(129);

    send_command(0x2B);
    send_data(0x00);
    send_data(0x00);
    send_data(0x00);
    send_data(128);      

    send_command(0x2C);

    for (int i = 0; i < 128 * 160; i++) {
        send_data(0xFF); // blue
        send_data(0xFF);  // green
        send_data(0xFF);  // red

    }
}

void drawNumHelper(char col, char row) {
    send_command(0x2A);
    send_data(0x00);
    send_data(col);
    send_data(0x00);
    send_data(col);

    send_command(0x2B);
    send_data(0x00);
    send_data(row);
    send_data(0x00);
    send_data(row);

    send_command(0x2C);

    for (int i = 0; i < 32*33; i++) {
        send_data(0x00); // blue
        send_data(0x00);  // green
        send_data(0x00);  // red
    }
}


void drawNum(unsigned char x, unsigned char y, unsigned int tileNum) {
    unsigned char col;
    unsigned char row;
    unsigned char num;
    unsigned digits;

    if (tileNum > 999) 
        digits = 4;
    else if (tileNum > 99) 
        digits = 3;
    else if (tileNum > 9) 
        digits = 2;
    else
        digits = 1;

    col = ((((32 - 5) / 2 ) + 1) / digits) + grid[y][x].colStart;
    row = ((((31 - 8) / 2) + 1) + grid[y][x].rowStart);

    for (unsigned char i = digits; i > 0; i--) {
        if (i == 4)
            num = (tileNum / 1000) % 10;
        else if (i == 3) 
            num = (tileNum / 100) % 10;
        else if (i == 2) 
            num = (tileNum / 10) % 10;
        else 
            num = (tileNum % 10);
    
        for (unsigned char y = 0; y < 5; y++) {
            for (unsigned char bit = 0; bit < 8; bit++) {
                if (numbers[num][y] & (1 << bit)) {
                    drawNumHelper(col + y, row + bit);
                }
            }
        }
        col += 7;
    }
}

void drawTile(unsigned char col, unsigned char row, tile tile) {
    send_command(0x2A);
    send_data(0x00);
    send_data(grid[row][col].colStart);
    send_data(0x00);
    send_data(grid[row][col].colEnd);

    send_command(0x2B);
    send_data(0x00);
    send_data(grid[row][col].rowStart);
    send_data(0x00);
    send_data(grid[row][col].rowEnd);

    send_command(0x2C);

    for (int i = 0; i < 32*33; i++) {
        send_data(tile.b); // blue
        send_data(tile.g);  // green
        send_data(tile.r);  // red

    }
    if (tile.tileNum != 0) {
        drawNum(col, row, tile.tileNum);   
    }
    grid[row][col].whatTile = tile;
}

void drawGrid(unsigned char colStart, unsigned char colEnd, unsigned char rowStart, unsigned char rowEnd) {
    send_command(0x2A);
    send_data(0x00);
    send_data(colStart);
    send_data(0x00);
    send_data(colEnd);

    send_command(0x2B);
    send_data(0x00);
    send_data(rowStart);
    send_data(0x00);
    send_data(rowEnd);

    send_command(0x2C);

    for (int i = 0; i < 32*33; i++) {
        send_data(0x00); // blue
        send_data(0x00);  // green
        send_data(0x00);  // red

    }
}


void setGrid() {
    drawGrid(33, 33, 0, 128);
    drawGrid(65, 65, 0, 128);
    drawGrid(98, 98, 0, 128);

    drawGrid(0, 129, 32, 32);
    drawGrid(0, 129, 64, 64);
    drawGrid(0, 129, 96, 96);

}

//8x8 font
unsigned char win[3][8] {
    {0x7F, 0x80, 0x40, 0x38, 0x38, 0x40, 0x80, 0x7F}, //W
    {0x00, 0x81, 0x81, 0xFF, 0xFF, 0x81, 0x81, 0x00}, //I
    {0xFF, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFF} //N
};


void winScreen() {
    unsigned char col = 52;
    unsigned char row = 60;

    for (unsigned char y = 0; y < 3; y++) {
        for (unsigned char x = 0; x < 8; x++) {
            for (unsigned char bit = 0; bit < 8; bit++) {
                if (win[y][x] & (1 << bit)) {
                    drawNumHelper(col + x, row + bit);
                }
            }
        }
        col += 9;
    }
}

//8x8 
unsigned char title[4][8] {
    {0xE6, 0xA3, 0xB1, 0x91, 0x91, 0x91, 0x9F, 0x9E}, //2
    {0x00, 0x7E, 0xC3, 0x81, 0x81, 0xC3, 0x7E, 0x00}, // 0
    {0x18, 0x14, 0x12, 0x11, 0xFF, 0x10, 0x10, 0x00}, // 4
    {0x00, 0x6E, 0x91, 0x91, 0x91, 0x91, 0x6E, 0x00}, // 8
};

void titleScreen() {
    unsigned char col = 50;
    unsigned char row = 30;

    for (unsigned char y = 0; y < 4; y++) {
        for (unsigned char x = 0; x < 8; x++) {
            for (unsigned char bit = 0; bit < 8; bit++) {
                if (title[y][x] & (1 << bit)) {
                    drawNumHelper(col + x, row + bit);
                }
            }
        }
        col += 9;
    }
}

//8x8
unsigned char startTitle[6][8] = {
    {0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00},// dot
    {0x4E, 0x93, 0x91, 0x91, 0x91, 0x91, 0x93, 0x66}, //S
    {0x01, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x01}, //T
    {0xF0, 0x0C, 0x0A, 0x09, 0x09, 0x0A, 0x0C, 0xF0}, //A
    {0x00, 0xFF, 0x11, 0x31, 0x51, 0x91, 0x92, 0x0C}, //R
    {0x01, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x01} // T
};

void startBtScreen() {
    unsigned char col = 38;
    unsigned char row = 80;

    for (unsigned char y = 0; y < 6; y++) {
        for (unsigned char x = 0; x < 8; x++) {
            for (unsigned char bit = 0; bit < 8; bit++) {
                if (startTitle[y][x] & (1 << bit)) {
                    drawNumHelper(col + x, row + bit);
                }
            }
        }
        col += 9;
    }
}


//8x8
unsigned char lose[4][8] {
    {0x00, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00}, // L
    {0x00, 0xFF, 0x81, 0x81, 0x81, 0x81, 0xFF, 0x00}, //O
    {0x4E, 0x93, 0x91, 0x91, 0x91, 0x91, 0x93, 0x66}, //S
    {0x00, 0xFF, 0x91, 0x91, 0x91, 0x91, 0x81, 0x00} //E
};

void loseScreen() {
    unsigned char col = 46;
    unsigned char row = 60;

    for (unsigned char y = 0; y < 4; y++) {
        for (unsigned char x = 0; x < 8; x++) {
            for (unsigned char bit = 0; bit < 8; bit++) {
                if (lose[y][x] & (1 << bit)) {
                    drawNumHelper(col + x, row + bit);
                }
            }
        }
        col += 9;
    }
}


#endif